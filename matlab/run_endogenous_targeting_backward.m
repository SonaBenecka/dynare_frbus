%RUN_ENDOGENOUS_TARGETING_BACKWARD FRB/US exercise 4: endogenous targeting.
%
% This is the Dynare analogue of pyfrbus mcontrol / BIMETS RENORM. A Newton
% loop adjusts five add-factor instruments so that five target variables follow
% externally supplied trajectories over 2021Q3-2022Q3.
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

start_yq = [2021 3];
end_yq   = [2022 3];
start_idx = frbus_find_period(qid, start_yq(1), start_yq(2));
end_idx   = frbus_find_period(qid, end_yq(1), end_yq(2));
idx_range = start_idx:end_idx;

% Standard fiscal configuration used by the Fed/BIMETS demo.
D.dfpdbt(idx_range) = 0;
D.dfpsrp(idx_range) = 1;

% Compute add-factors on the unmodified baseline first, as in init_trac/RESCHECK.
addf = frbus_compute_addfactors_backward(D, idx_range);

% Scenario based on 2021Q3 SPF in the public demo. Because lurnat is endogenous
% in this MDL/Dynare translation, enforce this path by adjusting its add-factor.
D.lurnat(idx_range) = 3.78;
addf.lurnat(idx_range) = 0;
addf.lurnat(start_idx) = D.lurnat(start_idx) - D.lurnat(start_idx - 1);

% Target trajectories.
target_names = {'xgdp','lur','picxfe','rff','rg10'};
gdp_growth = cumprod((([6.8; 5.2; 4.5; 3.4; 2.7] / 100) + 1) .^ 0.25);
xgdp_target = D.xgdp(start_idx - 1) * gdp_growth;
target_paths = [ ...
    xgdp_target, ...
    [5.3; 4.9; 4.6; 4.4; 4.2], ...
    [3.7; 2.2; 2.1; 2.1; 2.2], ...
    [0.1; 0.1; 0.1; 0.1; 0.1], ...
    [1.4; 1.6; 1.6; 1.7; 1.9] ...
    ];

% Instruments are add-factor fields. This matches BIMETS INSTRUMENT=c(...)
% and pyfrbus inst=[..._aerr].
instrument_names = {'eco','lhp','picxfe','rff','rg10p'};

[addf_control, oo_, target_report, info] = frbus_endogenous_targeting( ...
    D, addf, qid, start_yq, end_yq, M_, oo_, options_, ...
    target_names, target_paths, instrument_names, ...
    'MaxIter', 8, 'Tol', 2e-6, 'FiniteDiffStep', [1e-4 1e-4 1e-4 1e-4 1e-4], ...
    'MaxNewtonStep', 25, 'SolverMaxIter', 800, 'SolverTolF', 1e-9, 'SolverTolX', 1e-9);

if ~info.converged
    warning('Endogenous targeting did not fully converge: %s', info.message);
end

disp(target_report);

% Pyfrbus-style plot over a slightly padded range. Re-solve once with the final
% control add-factors to get a clean Dynare window object for reporting.
plot_start_yq = [2021 1];
[oo_plot, ~, window_plot] = frbus_solve_perfect_foresight( ...
    D, addf_control, qid, start_yq, end_yq, M_, oo_, options_, ...
    'MaxIter', 800, 'TolF', 1e-9, 'TolX', 1e-9);
P = frbus_build_pyfrbus_plot_table(D, qid, oo_plot, M_, window_plot, plot_start_yq, end_yq);

out_target_csv = fullfile(bundle_root, 'docs', 'endogenous_targeting_backward_targets.csv');
out_paths_csv  = fullfile(bundle_root, 'docs', 'endogenous_targeting_backward_paths.csv');
out_png        = fullfile(bundle_root, 'docs', 'endogenous_targeting_backward_pyfrbus_style.png');
writetable(target_report, out_target_csv);
writetable(P, out_paths_csv);
frbus_plot_pyfrbus_style(P, out_png, 'Backward-looking FRB/US: endogenous targeting');

fprintf('Saved target report to %s\n', out_target_csv);
fprintf('Saved pyfrbus-style plot to %s\n', out_png);
