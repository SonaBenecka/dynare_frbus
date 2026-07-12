%RUN_ERROR_PROPAGATION_BACKWARD FRB/US exercise 3: residual propagation.
%
% Replicates the BIMETS/pyfrbus backward-looking exercise where selected
% historical shocks are imposed and then rolled off with AR(1) persistence 0.5.
% The monetary-policy threshold is enabled; the unemployment threshold binds and
% pushes the federal funds rate to the lower bound in the reference run.
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

start_yq = [2040 1];
end_yq   = [2046 1];  % pyfrbus: end = start + 24
start_idx = frbus_find_period(qid, start_yq(1), start_yq(2));
end_idx   = frbus_find_period(qid, end_yq(1), end_yq(2));
idx_range = start_idx:end_idx;

% Standard fiscal configuration and exercise-specific policy switches.
D.dfpdbt(idx_range) = 0;
D.dfpsrp(idx_range) = 1;
D.dmptay(idx_range) = 1;      % non-inertial Taylor rule
D.dmpintay(idx_range) = 0;
D.dmptrsh(idx_range) = 1;     % enable threshold/lower-bound logic
D.lurtrsh(idx_range) = 6.0;
D.pitrsh(idx_range) = 3.0;

% Compute tracking residuals for the policy configuration above. Keep a copy
% of the exact baseline path: the final exercise shock is introduced by
% continuation from this solvable point, which is materially more robust at
% the max() threshold kinks than asking Dynare to jump directly to the full
% propagated shock.
addf_base = frbus_compute_addfactors_backward(D, idx_range);
addf = addf_base;

% Zero tracking residuals for funds-rate and threshold variables so the rule and
% threshold mechanics are active, matching the pyfrbus/BIMETS exercise.
zero_vars = {'rfftay','rffrule','rff','dmptpi','dmptlur','dmptmax','dmptr'};
for k = 1:numel(zero_vars)
    addf.(zero_vars{k})(idx_range) = 0;
end

% Build shock-only components. These are then added to the baseline tracking
% residuals, exactly like the BIMETS aerr list added to ConstantAdjustment.
rho = 0.5;
shock = struct();
shock.eco   = zeros(numel(D.xgdp), 1);
shock.ecd   = zeros(numel(D.xgdp), 1);
shock.eh    = zeros(numel(D.xgdp), 1);
shock.rbbbp = zeros(numel(D.xgdp), 1);
shock.lhp   = zeros(numel(D.xgdp), 1);
shock.dmptr = zeros(numel(D.xgdp), 1);
shock.dmptlur = zeros(numel(D.xgdp), 1);

shock.eco(start_idx:start_idx+3) = [-0.002; -0.0016; -0.0070; -0.0045];
shock.ecd(start_idx:start_idx+3) = [-0.0319; -0.0154; -0.0412; -0.0838];
shock.eh(start_idx:start_idx+3) = [-0.0512; -0.0501; -0.0124; -0.0723];
shock.rbbbp(start_idx:start_idx+3) = [0.3999; 2.7032; 0.3391; -0.7759];
shock.lhp(start_idx:start_idx+8) = [-0.0029; -0.0048; -0.0119; -0.0085; ...
    -0.0074; -0.0061; -0.0077; -0.0033; -0.0042];

for t = (start_idx+4):end_idx
    shock.eco(t) = rho * shock.eco(t-1);
    shock.ecd(t) = rho * shock.ecd(t-1);
    shock.eh(t) = rho * shock.eh(t-1);
    shock.rbbbp(t) = rho * shock.rbbbp(t-1);
end
for t = (start_idx+9):end_idx
    shock.lhp(t) = rho * shock.lhp(t-1);
end

% Adds so thresholds do not trigger before shocks are felt.
shock.dmptr(start_idx) = -1;
shock.dmptlur(start_idx:start_idx+2) = -1;

shock_vars = fieldnames(shock);
for k = 1:numel(shock_vars)
    v = shock_vars{k};
    addf.(v)(idx_range) = addf.(v)(idx_range) + shock.(v)(idx_range);
end

% Solve the baseline first, then ramp the complete residual change in twelve
% deterministic continuation steps. Each step uses the previous solution as
% its initial guess, while the exact max() equations remain unchanged.
[oo_, options_, window, info] = frbus_solve_perfect_foresight( ...
    D, addf_base, qid, start_yq, end_yq, M_, oo_, options_, ...
    'MaxIter', 700, 'TolF', 1e-9, 'TolX', 1e-9);
if ~info.status
    error('Baseline solve before propagated shocks failed: %s', info.message);
end
addf_fields = fieldnames(addf_base);
ncontinuation = 12;
for c = 1:ncontinuation
    lambda = c / ncontinuation;
    addf_step = addf_base;
    for k = 1:numel(addf_fields)
        v = addf_fields{k};
        addf_step.(v)(idx_range) = addf_base.(v)(idx_range) + ...
            lambda * (addf.(v)(idx_range) - addf_base.(v)(idx_range));
    end
    [oo_step, options_step, window_step, info_step] = frbus_solve_perfect_foresight( ...
        D, addf_step, qid, start_yq, end_yq, M_, oo_, options_, ...
        'MaxIter', 700, 'TolF', 1e-9, 'TolX', 1e-9, ...
        'SolveAlgo', 9, 'InitialGuess', oo_);
    if ~info_step.status
        error('Propagated-shock continuation failed at lambda=%.6g: %s', ...
            lambda, info_step.message);
    end
    oo_ = oo_step;
    options_ = options_step;
    window = window_step;
    fprintf('Propagated-shock continuation %d/%d completed (lambda=%.6g).\n', ...
        c, ncontinuation, lambda);
end

S = frbus_extract_simul_table(oo_, M_, window, {'hggdp','lur','picxfe','rff','xgdp','pcxfe'});
disp(S(1:min(height(S), 8), :));

back_pad = max(min(round((end_idx - start_idx) / 4), 6), 2);
plot_start_yq = frbus_shift_yq(start_yq, -back_pad);
P = frbus_build_pyfrbus_plot_table(D, qid, oo_, M_, window, plot_start_yq, end_yq);

out_csv = fullfile(bundle_root, 'docs', 'error_propagation_backward_paths.csv');
out_png = fullfile(bundle_root, 'docs', 'error_propagation_backward_pyfrbus_style.png');
writetable(P, out_csv);
frbus_plot_pyfrbus_style(P, out_png, 'Backward-looking FRB/US: propagated residual shocks');

fprintf('Saved pyfrbus-style plot to %s\n', out_png);
fprintf('Saved plotted paths and diagnostics to %s\n', out_csv);
