function [oo_, options_, window] = frbus_prepare_perfect_foresight_arrays(D, addf, qid, start_yq, end_yq, M_, oo_, options_)
%FRBUS_PREPARE_PERFECT_FORESIGHT_ARRAYS Populate Dynare simulation arrays from baseline paths.
%
% start_yq and end_yq are two-element vectors, e.g. [2040 1], [2045 4].
% D contains original variables; addf contains a_<var> paths as fields without
% the a_ prefix. The function fills oo_.endo_simul and oo_.exo_simul with the
% history, simulation, and terminal columns expected by perfect_foresight_solver.
start_idx = frbus_find_period(qid, start_yq(1), start_yq(2));
end_idx   = frbus_find_period(qid, end_yq(1), end_yq(2));
if end_idx < start_idx
    error('End period is before start period.');
end
lag = M_.maximum_lag;
lead = M_.maximum_lead;
idx_window = (start_idx-lag):(end_idx+lead);
if idx_window(1) < 1 || idx_window(end) > numel(qid)
    error('Data do not cover required history/terminal window. Need rows %d:%d.', idx_window(1), idx_window(end));
end
periods = end_idx - start_idx + 1;
options_.periods = periods;
% The model-only .mod file does not run Dynare's setup command. Dynare 7.x
% nevertheless expects this container when the low-level solver is called.
if ~isfield(oo_, 'deterministic_simulation')
    oo_.deterministic_simulation = struct();
end
oo_.deterministic_simulation.controlled_paths_by_period = [];
window.idx_window = idx_window;
window.start_idx = start_idx;
window.end_idx = end_idx;
window.sim_cols = (lag+1):(lag+periods);
window.qid = qid(idx_window);

endo_names = frbus_dynare_names(M_.endo_names);
exo_names  = frbus_dynare_names(M_.exo_names);

oo_.endo_simul = zeros(M_.endo_nbr, numel(idx_window));
for j = 1:M_.endo_nbr
    nm = lower(endo_names{j});
    if isfield(D, nm)
        oo_.endo_simul(j,:) = D.(nm)(idx_window).';
    end
end

% Dynare adds auxiliary endogenous variables when it substitutes lags/leads.
% Their baseline values are reconstructed from the original FRB-US paths;
% they are not columns in LONGBASE.
for k = 1:numel(M_.aux_vars)
    [base_name, shift] = frbus_aux_baseline_source(k, M_, endo_names);
    if ~isfield(D, base_name)
        error('Auxiliary variable %s depends on missing data variable %s.', ...
            endo_names{M_.aux_vars(k).endo_index}, base_name);
    end
    source_idx = idx_window + shift;
    if min(source_idx) < 1 || max(source_idx) > numel(D.(base_name))
        error('Data do not cover auxiliary variable %s at shift %+d.', ...
            endo_names{M_.aux_vars(k).endo_index}, shift);
    end
    oo_.endo_simul(M_.aux_vars(k).endo_index,:) = D.(base_name)(source_idx).';
end

oo_.exo_simul = zeros(numel(idx_window), M_.exo_nbr);
for j = 1:M_.exo_nbr
    nm = lower(exo_names{j});
    if startsWith(nm, 'a_')
        base = extractAfter(nm, 2);
        base = char(base);
        if isfield(addf, base)
            vals = addf.(base)(idx_window);
            vals(isnan(vals)) = 0;
            oo_.exo_simul(:,j) = vals(:);
        else
            oo_.exo_simul(:,j) = 0;
        end
    elseif isfield(D, nm)
        oo_.exo_simul(:,j) = D.(nm)(idx_window);
    else
        % Some compatibility variables (for example dmpgen/pcstar) may be listed
        % but not used in the equations. Leave them at zero unless data provide them.
        oo_.exo_simul(:,j) = 0;
    end
end
end

function [base_name, total_shift] = frbus_aux_baseline_source(aux_idx, M_, endo_names)
%FRBUS_AUX_BASELINE_SOURCE Resolve a Dynare auxiliary into an original path.
a = M_.aux_vars(aux_idx);
if a.type == 1
    % Endogenous lag auxiliary.
    base_name = lower(endo_names{a.orig_index});
    total_shift = a.orig_lead_lag;
    return
end
if a.type == 3
    % Exogenous lag auxiliary. The original exogenous variables are the first
    % entries in M_.exo_names; add-factors follow them in the generated model.
    exo_names = frbus_dynare_names(M_.exo_names);
    base_name = lower(exo_names{a.orig_index});
    total_shift = a.orig_lead_lag;
    return
end
if a.type == 0
    % Endogenous lead auxiliary. Dynare stores a one-step expression such as
    % pic4(1) or AUX_ENDO_LEAD_x(1); follow the chain to the original series.
    expr = strtrim(char(a.orig_expr));
    tok = regexp(expr, '^([A-Za-z_]\w*)\s*\(\s*([+-]?\d+)\s*\)$', 'tokens', 'once');
    if isempty(tok)
        error('Unsupported Dynare auxiliary expression: %s', expr);
    end
    source_name = lower(tok{1});
    step = str2double(tok{2});
    source_idx = find(strcmpi(endo_names, source_name), 1);
    if isempty(source_idx)
        base_name = source_name;
        total_shift = step;
        return
    end
    nested_idx = find([M_.aux_vars.endo_index] == source_idx, 1);
    if isempty(nested_idx)
        base_name = source_name;
        total_shift = step;
    else
        [base_name, nested_shift] = frbus_aux_baseline_source(nested_idx, M_, endo_names);
        total_shift = step + nested_shift;
    end
    return
end
error('Unsupported Dynare auxiliary type %d.', a.type);
end
