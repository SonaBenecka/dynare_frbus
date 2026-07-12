%RUN_STOCHASTIC_SIMULATION_BACKWARD FRB/US exercise 5: stochastic simulation.
%
% This implements the pyfrbus/BIMETS bootstrap idea in Dynare: draw historical
% quarters from the tracking-residual period, add the 64 stochastic residuals to
% the simulation add-factors, solve each deterministic path, and plot central
% 70/90 percent intervals. This is computationally heavier in Dynare than in
% BIMETS/pyfrbus, so set environment variable FRBUS_NREPL for a quick smoke test.
clearvars;
this_file = mfilename('fullpath');
bundle_root = fileparts(fileparts(this_file));
addpath(fullfile(bundle_root, 'matlab'));
frbus_setup_dynare();
clear M_ oo_ options_ estim_params_ bayestopt_

old_dir = pwd;
cd(fullfile(bundle_root, 'dynare'));
dynare frbus_backward.mod noclearall nolog nostrict
cd(old_dir);
addpath(fullfile(bundle_root, 'dynare'));
M_ = frbus_sanitize_dynare_model(M_);

data_file = fullfile(bundle_root, 'data', 'LONGBASE.TXT');
if ~isfile(data_file)
    data_file = fullfile(bundle_root, 'data', 'frbus_data.csv');
end
[D, qid] = frbus_load_longbase(data_file, [1962 1]);

residstart_yq = [1975 1];
residend_yq   = [2018 4];
simstart_yq   = [2040 1];
simend_yq     = [2045 4];
residstart_idx = frbus_find_period(qid, residstart_yq(1), residstart_yq(2));
residend_idx   = frbus_find_period(qid, residend_yq(1), residend_yq(2));
simstart_idx   = frbus_find_period(qid, simstart_yq(1), simstart_yq(2));
simend_idx     = frbus_find_period(qid, simend_yq(1), simend_yq(2));
resid_idx = residstart_idx:residend_idx;
sim_idx = simstart_idx:simend_idx;

nrepl_env = str2double(getenv('FRBUS_NREPL'));
if isfinite(nrepl_env) && nrepl_env > 0
    nrepl = round(nrepl_env);
else
    nrepl = 1000;
end
nextra = 5;
rng(9);

% Policy settings over the simulation range only, matching the public demo.
D.dfpdbt(sim_idx) = 0;
D.dfpsrp(sim_idx) = 1;

% Compute add-factors over both the historical residual window and the future
% simulation window. Historical add-factors are reused as sampled shocks.
addf_base = frbus_compute_addfactors_backward(D, residstart_idx:simend_idx);
stochastic_vars = frbus_stochastic_vars_backward();
for k = 1:numel(stochastic_vars)
    v = stochastic_vars{k};
    if ~isfield(addf_base, v)
        error('Stochastic variable %s has no add-factor field.', v);
    end
end

% Baseline solve and reporting range.
[oo_base, ~, window_base] = frbus_solve_perfect_foresight( ...
    D, addf_base, qid, simstart_yq, simend_yq, M_, oo_, options_, ...
    'MaxIter', 700, 'TolF', 1e-9, 'TolX', 1e-9);
back_pad = max(min(round((simend_idx - simstart_idx) / 4), 6), 2);
plot_start_yq = frbus_shift_yq(simstart_yq, -back_pad);
Pbase = frbus_build_pyfrbus_plot_table(D, qid, oo_base, M_, window_base, plot_start_yq, simend_yq);
Tplot = height(Pbase);

gdp4_sims = nan(Tplot, nrepl);
lur_sims = nan(Tplot, nrepl);
pcxfe4_sims = nan(Tplot, nrepl);
rff_sims = nan(Tplot, nrepl);

% Draw historical residual quarters once for all stochastic variables, as in
% pyfrbus: in each simulated quarter of a replication, all stochastic variables
% receive residuals from the same sampled historical quarter.
sim_len = numel(sim_idx);
max_attempts = nrepl + nextra;
draw_pos = randi(numel(resid_idx), sim_len, max_attempts);
draw_idx = resid_idx(draw_pos);

shock_mats = struct();
for k = 1:numel(stochastic_vars)
    v = stochastic_vars{k};
    raw = addf_base.(v)(draw_idx);
    finite_vals = raw(isfinite(raw));
    if isempty(finite_vals)
        error('No finite historical residuals for stochastic variable %s.', v);
    end
    raw = raw - mean(finite_vals);
    shock_mats.(v) = raw;
end

success = 0;
attempt = 0;
failed_messages = {};
while success < nrepl && attempt < max_attempts
    attempt = attempt + 1;
    addf = addf_base;
    for k = 1:numel(stochastic_vars)
        v = stochastic_vars{k};
        addf.(v)(sim_idx) = addf_base.(v)(sim_idx) + shock_mats.(v)(:, attempt);
    end

    [oo_rep, ~, window_rep, solve_info] = frbus_solve_perfect_foresight( ...
        D, addf, qid, simstart_yq, simend_yq, M_, oo_, options_, ...
        'MaxIter', 700, 'TolF', 1e-8, 'TolX', 1e-8, 'ThrowOnFail', false);
    if ~solve_info.status
        failed_messages{end+1,1} = sprintf('attempt %d: %s', attempt, solve_info.message); %#ok<AGROW>
        fprintf('Stochastic replication attempt %d failed: %s\n', attempt, solve_info.message);
        continue
    end

    P = frbus_build_pyfrbus_plot_table(D, qid, oo_rep, M_, window_rep, plot_start_yq, simend_yq);
    success = success + 1;
    gdp4_sims(:, success) = P.sim_gdp4;
    lur_sims(:, success) = P.sim_lur;
    pcxfe4_sims(:, success) = P.sim_pcxfe4;
    rff_sims(:, success) = P.sim_rff;
    if mod(success, 25) == 0 || success == nrepl
        fprintf('Successful stochastic replications: %d/%d\n', success, nrepl);
    end
end

if success == 0
    nshow = min(numel(failed_messages), 3);
    first_failures = strjoin(failed_messages(1:nshow), ' | ');
    error('No stochastic simulation replication converged. First failures: %s', first_failures);
end
if success < nrepl
    warning('Only %d/%d requested replications converged after %d attempts.', success, nrepl, attempt);
end

gdp4_sims = gdp4_sims(:, 1:success);
lur_sims = lur_sims(:, 1:success);
pcxfe4_sims = pcxfe4_sims(:, 1:success);
rff_sims = rff_sims(:, 1:success);
probs = [5 15 50 85 95];
Qgdp = frbus_quantile_columns(gdp4_sims, probs);
Qlur = frbus_quantile_columns(lur_sims, probs);
Qpc  = frbus_quantile_columns(pcxfe4_sims, probs);
Qrff = frbus_quantile_columns(rff_sims, probs);

B = table();
B.qid = Pbase.qid;
B.qlabel = Pbase.qlabel;
B.base_gdp4 = Pbase.base_gdp4;
B.base_lur = Pbase.base_lur;
B.base_pcxfe4 = Pbase.base_pcxfe4;
B.base_rff = Pbase.base_rff;
B.gdp4_p05 = Qgdp(:,1); B.gdp4_p15 = Qgdp(:,2); B.gdp4_p50 = Qgdp(:,3); B.gdp4_p85 = Qgdp(:,4); B.gdp4_p95 = Qgdp(:,5);
B.lur_p05 = Qlur(:,1); B.lur_p15 = Qlur(:,2); B.lur_p50 = Qlur(:,3); B.lur_p85 = Qlur(:,4); B.lur_p95 = Qlur(:,5);
B.pcxfe4_p05 = Qpc(:,1); B.pcxfe4_p15 = Qpc(:,2); B.pcxfe4_p50 = Qpc(:,3); B.pcxfe4_p85 = Qpc(:,4); B.pcxfe4_p95 = Qpc(:,5);
B.rff_p05 = Qrff(:,1); B.rff_p15 = Qrff(:,2); B.rff_p50 = Qrff(:,3); B.rff_p85 = Qrff(:,4); B.rff_p95 = Qrff(:,5);

out_csv = fullfile(bundle_root, 'docs', 'stochastic_simulation_backward_bands.csv');
out_png = fullfile(bundle_root, 'docs', 'stochastic_simulation_backward_pyfrbus_style.png');
out_mat = fullfile(bundle_root, 'docs', 'stochastic_simulation_backward_replications.mat');
writetable(B, out_csv);
frbus_plot_stochsim_pyfrbus_style(B, out_png, sprintf('Backward-looking FRB/US: stochastic simulation (%d replications)', success));
save(out_mat, 'B', 'gdp4_sims', 'lur_sims', 'pcxfe4_sims', 'rff_sims', ...
    'stochastic_vars', 'success', 'attempt', 'failed_messages', '-v7.3');

fprintf('Saved stochastic bands to %s\n', out_csv);
fprintf('Saved stochastic fan chart to %s\n', out_png);
fprintf('Saved replication matrix to %s\n', out_mat);
