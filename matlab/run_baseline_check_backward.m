%RUN_BASELINE_CHECK_BACKWARD Verify exact deterministic tracking of LONGBASE.
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
end_yq = [2045 4];
start_idx = frbus_find_period(qid, start_yq(1), start_yq(2));
end_idx = frbus_find_period(qid, end_yq(1), end_yq(2));

% Match the policy-switch configuration used by the scenario exercises before
% constructing add-factors. No shock is added to any residual.
D.dfpdbt(start_idx:end_idx) = 0;
D.dfpsrp(start_idx:end_idx) = 1;
addf = frbus_compute_addfactors_backward(D, start_idx:end_idx);
[oo_, options_, window] = frbus_prepare_perfect_foresight_arrays(...
    D, addf, qid, start_yq, end_yq, M_, oo_, options_);
options_.simul.maxit = 500;
options_.dynatol.f = 1e-10;
options_.dynatol.x = 1e-10;
[oo_, ~] = perfect_foresight_solver(M_, options_, oo_);
if ~isfield(oo_.deterministic_simulation, 'status') || ~oo_.deterministic_simulation.status
    error('Dynare did not report a successful baseline solution.');
end

names = frbus_names_backward();
nendo = numel(names.endo);
sim = oo_.endo_simul(1:nendo, window.sim_cols);
base = zeros(nendo, numel(window.sim_cols));
for j = 1:nendo
    nm = lower(names.endo{j});
    if ~isfield(D, nm)
        error('Baseline data are missing endogenous variable %s.', nm);
    end
    base(j,:) = D.(nm)(start_idx:end_idx).';
end
abs_err = abs(sim - base);
[max_abs_error, linear_idx] = max(abs_err(:));
[worst_var_idx, worst_period_idx] = ind2sub(size(abs_err), linear_idx);
mean_abs_error = mean(abs_err(:));
worst_var = names.endo{worst_var_idx};
worst_qid = window.qid(window.sim_cols(worst_period_idx));

fprintf('Backward baseline max_abs_error = %.16g\n', max_abs_error);
fprintf('Backward baseline mean_abs_error = %.16g\n', mean_abs_error);
fprintf('Worst variable/quarter = %s / %d\n', worst_var, worst_qid);
if max_abs_error >= 1e-7
    error('Backward baseline tolerance failed: %.16g >= 1e-7.', max_abs_error);
end

report_file = fullfile(bundle_root, 'docs', 'baseline_report_backward.txt');
fid = fopen(report_file, 'w');
if fid < 0
    error('Could not open baseline report for writing: %s', report_file);
end
fprintf(fid, 'FRB/US backward-looking baseline tracking check\n');
fprintf(fid, 'Window: 2040Q1-2045Q4; policy switches dfpdbt=0, dfpsrp=1\n');
fprintf(fid, 'max_abs_error = %.16g\n', max_abs_error);
fprintf(fid, 'mean_abs_error = %.16g\n', mean_abs_error);
fprintf(fid, 'worst_variable = %s\n', worst_var);
fprintf(fid, 'worst_qid = %d\n', worst_qid);
fclose(fid);
