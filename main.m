% Run the complete FRB/US Dynare workflow from any current folder.
this_file = mfilename('fullpath');
repo_root = fileparts(this_file);
cd(fullfile(repo_root, 'matlab'))

run_baseline_check_backward
run_mp_shock_backward
run_mp_shock_mce
run_error_propagation_backward
run_endogenous_targeting_backward

if isempty(getenv('FRBUS_NREPL'))
    setenv('FRBUS_NREPL', '1000')
end
run_stochastic_simulation_backward
