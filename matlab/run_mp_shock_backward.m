%RUN_MP_SHOCK_BACKWARD FRB/US backward-looking monetary-policy shock in Dynare.
%
% This script follows the BIMETS/pyfrbus demo: it computes tracking residuals on
% the LONGBASE baseline, adds a 100bp shock to rffintay in 2040Q1, solves the
% deterministic path, and plots the result using the pyfrbus plotting convention.
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
end_yq   = [2045 4];
start_idx = frbus_find_period(qid, start_yq(1), start_yq(2));
end_idx   = frbus_find_period(qid, end_yq(1), end_yq(2));

% Standard fiscal configuration used by the Fed/BIMETS demo.
D.dfpdbt(start_idx:end_idx) = 0;
D.dfpsrp(start_idx:end_idx) = 1;

addf = frbus_compute_addfactors_backward(D, start_idx:end_idx);
addf.rffintay(start_idx) = addf.rffintay(start_idx) + 1;

[oo_, options_, window] = frbus_prepare_perfect_foresight_arrays( ...
    D, addf, qid, start_yq, end_yq, M_, oo_, options_);
options_.simul.maxit = 500;
options_.dynatol.f = 1e-10;
options_.dynatol.x = 1e-10;
[oo_, ~] = perfect_foresight_solver(M_, options_, oo_);

S = frbus_extract_simul_table(oo_, M_, window, {'hggdp','lur','picxfe','rff','xgdp','pcxfe'});
disp(S(1:min(height(S), 8), :));

% Match pyfrbus sim_plot padding: 25 percent of horizon, capped at 6 quarters.
back_pad = max(min(round((end_idx - start_idx) / 4), 6), 2);
plot_start_yq = frbus_shift_yq(start_yq, -back_pad);
P = frbus_build_pyfrbus_plot_table(D, qid, oo_, M_, window, plot_start_yq, end_yq);

out_csv = fullfile(bundle_root, 'docs', 'mp_shock_backward_paths.csv');
out_png = fullfile(bundle_root, 'docs', 'mp_shock_backward_pyfrbus_style.png');
writetable(P, out_csv);
frbus_plot_pyfrbus_style(P, out_png, 'Backward-looking FRB/US: 100bp rffintay shock');

fprintf('Saved pyfrbus-style plot to %s\n', out_png);
fprintf('Saved plotted paths and diagnostics to %s\n', out_csv);
