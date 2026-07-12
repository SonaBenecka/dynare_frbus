%RUN_MP_SHOCK_MCE FRB/US MCE monetary-policy shock in Dynare.
%
% Same structure as run_mp_shock_backward.m, but with model-consistent
% expectations equations and the short two-year example used in the BIMETS paper.
% For long-horizon MCE work, extend the terminal horizon substantially and
% report only the early quarters of interest.
clearvars;
this_file = mfilename('fullpath');
bundle_root = fileparts(fileparts(this_file));
addpath(fullfile(bundle_root, 'matlab'));
frbus_setup_dynare();
clear M_ oo_ options_ estim_params_ bayestopt_

old_dir = pwd;
cd(fullfile(bundle_root, 'dynare'));
dynare frbus_mce.mod noclearall nolog nostrict
cd(old_dir);
addpath(fullfile(bundle_root, 'dynare'));
M_ = frbus_sanitize_dynare_model(M_);

data_file = fullfile(bundle_root, 'data', 'LONGBASE.TXT');
if ~isfile(data_file)
    data_file = fullfile(bundle_root, 'data', 'frbus_data.csv');
end
[D, qid] = frbus_load_longbase(data_file, [1962 1]);

start_yq = [2040 1];
end_yq   = [2041 4];
start_idx = frbus_find_period(qid, start_yq(1), start_yq(2));
end_idx   = frbus_find_period(qid, end_yq(1), end_yq(2));
D.dfpdbt(start_idx:end_idx) = 0;
D.dfpsrp(start_idx:end_idx) = 1;
D.drstar(start_idx:end_idx) = 0;
D.drstar((start_idx+4):end_idx) = 1;

addf = frbus_compute_addfactors_mce(D, start_idx:end_idx);
addf.rffintay(start_idx) = addf.rffintay(start_idx) + 1;

[oo_, options_, window] = frbus_prepare_perfect_foresight_arrays( ...
    D, addf, qid, start_yq, end_yq, M_, oo_, options_);
options_.simul.maxit = 500;
options_.dynatol.f = 1e-6;
options_.dynatol.x = 1e-6;
[oo_, ~] = perfect_foresight_solver(M_, options_, oo_);

S = frbus_extract_simul_table(oo_, M_, window, {'hggdp','lur','picxfe','rff','xgdp','pcxfe'});
disp(S);

back_pad = max(min(round((end_idx - start_idx) / 4), 6), 2);
plot_start_yq = frbus_shift_yq(start_yq, -back_pad);
P = frbus_build_pyfrbus_plot_table(D, qid, oo_, M_, window, plot_start_yq, end_yq);

out_csv = fullfile(bundle_root, 'docs', 'mp_shock_mce_paths.csv');
out_png = fullfile(bundle_root, 'docs', 'mp_shock_mce_pyfrbus_style.png');
writetable(P, out_csv);
frbus_plot_pyfrbus_style(P, out_png, 'MCE FRB/US: 100bp rffintay shock');

fprintf('Saved pyfrbus-style plot to %s\n', out_png);
fprintf('Saved plotted paths and diagnostics to %s\n', out_csv);
