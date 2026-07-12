function [oo_out, options_out, window, info] = frbus_solve_perfect_foresight(D, addf, qid, start_yq, end_yq, M_, oo_in, options_in, varargin)
%FRBUS_SOLVE_PERFECT_FORESIGHT Fill arrays and solve one deterministic FRB/US path.
%
% This wrapper centralizes solver settings. It deliberately re-initializes Dynare's
% endogenous simulation array from the baseline D each call; this mirrors the
% pyfrbus/BIMETS workflow where each scenario starts from the same baseline path
% plus a modified add-factor/instrument path.

p = inputParser;
p.addParameter('MaxIter', 500, @(x) isnumeric(x) && isscalar(x) && x > 0);
p.addParameter('TolF', 1e-10, @(x) isnumeric(x) && isscalar(x) && x > 0);
p.addParameter('TolX', 1e-10, @(x) isnumeric(x) && isscalar(x) && x > 0);
p.addParameter('SolveAlgo', [], @(x) isempty(x) || (isnumeric(x) && isscalar(x)));
p.addParameter('ThrowOnFail', true, @(x) islogical(x) || isnumeric(x));
p.addParameter('InitialGuess', [], @(x) isempty(x) || isstruct(x));
p.parse(varargin{:});

M_ = frbus_sanitize_dynare_model(M_);

% Scripts in main.m are intentionally run in one MATLAB workspace. Clear
% model-specific simulation arrays from the prior exercise before rebuilding
% them for the current M_ structure; otherwise a preceding MCE run can leave
% arrays with a different number of endogenous variables or periods.
oo_in.endo_simul = [];
oo_in.exo_simul = [];
oo_in.deterministic_simulation = struct();
if numel(M_.endo_names) ~= M_.endo_nbr
    error('Inconsistent Dynare model: endo_nbr=%d but endo_names=%d.', ...
        M_.endo_nbr, numel(M_.endo_names));
end
if ~isempty(M_.aux_vars) && max([M_.aux_vars.endo_index]) > M_.endo_nbr
    error('Inconsistent Dynare model: auxiliary endo index exceeds endo_nbr (%d).', M_.endo_nbr);
end
[oo_out, options_out, window] = frbus_prepare_perfect_foresight_arrays( ...
    D, addf, qid, start_yq, end_yq, M_, oo_in, options_in);
options_out.simul.maxit = p.Results.MaxIter;
options_out.dynatol.f = p.Results.TolF;
options_out.dynatol.x = p.Results.TolX;
if ~isempty(p.Results.SolveAlgo)
    options_out.solve_algo = p.Results.SolveAlgo;
end

expected_endo_size = [M_.endo_nbr, M_.maximum_lag + options_out.periods + M_.maximum_lead];
expected_exo_size = [expected_endo_size(2), M_.exo_nbr];
if ~isequal(size(oo_out.endo_simul), expected_endo_size)
    error('FRBUS solver array mismatch: endo actual %s, expected %s (periods=%d, lag=%d, lead=%d).', ...
        mat2str(size(oo_out.endo_simul)), mat2str(expected_endo_size), ...
        options_out.periods, M_.maximum_lag, M_.maximum_lead);
end
if M_.exo_nbr > 0 && ~isequal(size(oo_out.exo_simul), expected_exo_size)
    error('FRBUS solver array mismatch: exo actual %s, expected %s.', ...
        mat2str(size(oo_out.exo_simul)), mat2str(expected_exo_size));
end
if ~isempty(p.Results.InitialGuess)
    guess = p.Results.InitialGuess;
    if ~isfield(guess, 'endo_simul') || ~isequal(size(guess.endo_simul), expected_endo_size)
        error('InitialGuess.endo_simul has size %s; expected %s.', ...
            mat2str(size(guess.endo_simul)), mat2str(expected_endo_size));
    end
    % Keep baseline history/terminal values from the current add-factor path,
    % but use the previous continuation solution for the simulated quarters.
    oo_out.endo_simul(:, window.sim_cols) = guess.endo_simul(:, window.sim_cols);
end

info = struct();
info.status = false;
info.message = '';
try
    [oo_out, ~] = perfect_foresight_solver(M_, options_out, oo_out);
    if isfield(oo_out, 'deterministic_simulation') && ...
            isfield(oo_out.deterministic_simulation, 'status')
        info.status = logical(oo_out.deterministic_simulation.status);
    else
        info.status = true;
    end
    if ~info.status
        info.message = 'perfect_foresight_solver returned unsuccessful status.';
        if p.Results.ThrowOnFail
            error(info.message);
        end
    end
catch ME
    info.status = false;
    info.message = ME.message;
    if p.Results.ThrowOnFail
        rethrow(ME);
    end
end
end
