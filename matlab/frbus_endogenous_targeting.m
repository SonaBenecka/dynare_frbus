function [addf_out, oo_out, target_report, info] = frbus_endogenous_targeting( ...
    D, addf_base, qid, start_yq, end_yq, M_, oo_template, options_template, ...
    target_names, target_paths, instrument_names, varargin)
%FRBUS_ENDOGENOUS_TARGETING Small Newton control loop for Dynare FRB/US paths.
%
% target_paths is T-by-K in the same order as target_names. instrument_names are
% add-factor fields, e.g. {'eco','lhp','picxfe','rff','rg10p'}. The function
% solves for additive changes to those add-factors over the target window so
% the simulated target variables match target_paths.

p = inputParser;
p.addParameter('MaxIter', 8, @(x) isnumeric(x) && isscalar(x) && x > 0);
p.addParameter('Tol', 1e-6, @(x) isnumeric(x) && isscalar(x) && x > 0);
p.addParameter('FiniteDiffStep', 1e-4, @(x) isnumeric(x) && ~isempty(x));
p.addParameter('MaxNewtonStep', 20, @(x) isnumeric(x) && isscalar(x) && x > 0);
p.addParameter('SolverMaxIter', 700, @(x) isnumeric(x) && isscalar(x) && x > 0);
p.addParameter('SolverTolF', 1e-9, @(x) isnumeric(x) && isscalar(x) && x > 0);
p.addParameter('SolverTolX', 1e-9, @(x) isnumeric(x) && isscalar(x) && x > 0);
p.parse(varargin{:});

start_idx = frbus_find_period(qid, start_yq(1), start_yq(2));
end_idx   = frbus_find_period(qid, end_yq(1), end_yq(2));
idx_range = start_idx:end_idx;
nT = numel(idx_range);
K = numel(target_names);
nI = numel(instrument_names);
if size(target_paths, 1) ~= nT || size(target_paths, 2) ~= K
    error('target_paths must be %d-by-%d.', nT, K);
end

for i = 1:nI
    if ~isfield(addf_base, instrument_names{i})
        error('Missing add-factor field for instrument %s.', instrument_names{i});
    end
end

fd = p.Results.FiniteDiffStep(:);
if numel(fd) == 1
    fd = repmat(fd, nI, 1);
elseif numel(fd) ~= nI
    error('FiniteDiffStep must be scalar or have one entry per instrument.');
end
fd_vec = zeros(nT*nI, 1);
for i = 1:nI
    fd_vec((i-1)*nT + (1:nT)) = fd(i);
end

scale = max(1, abs(target_paths));
scale_vec = reshape(scale, [], 1);
u = zeros(nT*nI, 1);
info = struct();
info.iterations = [];
info.converged = false;
info.message = '';

[err, oo_out, window, solve_info] = eval_control(u, true);
if ~solve_info.status
    error('Initial endogenous-targeting solve failed: %s', solve_info.message);
end
best_norm = max(abs(err ./ scale_vec));

for it = 1:p.Results.MaxIter
    fprintf('Targeting iteration %d: max scaled error = %.6g\n', it, best_norm);
    info.iterations(end+1).iter = it; %#ok<AGROW>
    info.iterations(end).max_scaled_error = best_norm;
    if best_norm < p.Results.Tol
        info.converged = true;
        info.message = 'Converged.';
        break
    end

    J = nan(numel(err), numel(u));
    for j = 1:numel(u)
        up = u;
        up(j) = up(j) + fd_vec(j);
        [errp, ~, ~, infop] = eval_control(up, false);
        if ~infop.status
            warning('Finite-difference solve failed for column %d: %s', j, infop.message);
            continue
        end
        J(:, j) = (errp - err) / fd_vec(j);
    end
    good_cols = all(isfinite(J), 1);
    if nnz(good_cols) < numel(u)
        warning('Dropping %d failed finite-difference columns from control update.', nnz(~good_cols));
    end
    Js = J(:, good_cols) ./ scale_vec;
    es = err ./ scale_vec;
    if isempty(Js) || rank(Js) < min(size(Js))
        du_good = -pinv(Js) * es;
    else
        du_good = -Js \ es;
    end
    du = zeros(size(u));
    du(good_cols) = du_good;

    max_step = max(abs(du));
    if max_step > p.Results.MaxNewtonStep
        du = du * (p.Results.MaxNewtonStep / max_step);
    end

    accepted = false;
    lambda = 1.0;
    for ls = 1:8
        uc = u + lambda * du;
        [errc, oo_c, window_c, infoc] = eval_control(uc, false);
        if infoc.status
            normc = max(abs(errc ./ scale_vec));
            if isfinite(normc) && normc < best_norm
                u = uc;
                err = errc;
                oo_out = oo_c;
                window = window_c;
                best_norm = normc;
                accepted = true;
                break
            end
        end
        lambda = lambda / 2;
    end
    if ~accepted
        info.message = 'Line search failed to improve the target error.';
        warning('%s', info.message);
        break
    end
end

if ~info.converged && best_norm < p.Results.Tol
    info.converged = true;
    info.message = 'Converged.';
elseif ~info.converged && isempty(info.message)
    info.message = 'Maximum iterations reached before convergence.';
end

addf_out = apply_control(u);
sim_targets = frbus_extract_sim_values(oo_out, M_, window, target_names);
target_report = table();
target_report.qid = qid(idx_range);
target_report.qlabel = frbus_qid_to_label(target_report.qid);
for k = 1:K
    nm = lower(target_names{k});
    target_report.([nm '_target']) = target_paths(:, k);
    target_report.([nm '_sim']) = sim_targets(:, k);
    target_report.([nm '_error']) = sim_targets(:, k) - target_paths(:, k);
end
for i = 1:nI
    inst = lower(instrument_names{i});
    target_report.(['dadd_' inst]) = u((i-1)*nT + (1:nT));
    target_report.(['addf_' inst]) = addf_out.(inst)(idx_range);
end
fprintf('Endogenous targeting finished: %s max scaled error = %.6g\n', info.message, best_norm);

    function addf = apply_control(uvec)
        addf = addf_base;
        for ii = 1:nI
            rows = (ii-1)*nT + (1:nT);
            inst = lower(instrument_names{ii});
            addf.(inst)(idx_range) = addf_base.(inst)(idx_range) + uvec(rows);
        end
    end

    function [errvec, oo_s, window_s, solve_s] = eval_control(uvec, throw_on_fail)
        addf = apply_control(uvec);
        [oo_s, ~, window_s, solve_s] = frbus_solve_perfect_foresight( ...
            D, addf, qid, start_yq, end_yq, M_, oo_template, options_template, ...
            'MaxIter', p.Results.SolverMaxIter, ...
            'TolF', p.Results.SolverTolF, 'TolX', p.Results.SolverTolX, ...
            'ThrowOnFail', throw_on_fail);
        if solve_s.status
            Y = frbus_extract_sim_values(oo_s, M_, window_s, target_names);
            errvec = reshape(Y - target_paths, [], 1);
        else
            errvec = nan(numel(target_paths), 1);
        end
    end
end
