% MAIN Run the complete FRB/US Dynare workflow.
%
% This script finds its own location, makes the repository root MATLAB's
% working directory, and adds the MATLAB helper folder to the path. You can
% therefore start it from the Command Window, the Editor, or another folder.
this_file = mfilename('fullpath');
if isempty(this_file)
    this_file = which('main');
end
if isempty(this_file)
    error('Could not locate main.m. Open the repository folder or add it to the MATLAB path.');
end

repo_root = fileparts(this_file);
matlab_dir = fullfile(repo_root, 'matlab');
if ~isfolder(matlab_dir)
    error('The MATLAB folder was not found below: %s', repo_root);
end

cd(repo_root)
addpath(matlab_dir)
fprintf('FRB/US Dynare project: %s\n', repo_root)
fprintf('MATLAB working directory: %s\n', pwd)

run_baseline_check_backward
run_mp_shock_backward
run_mp_shock_mce
run_error_propagation_backward
run_endogenous_targeting_backward

if isempty(getenv('FRBUS_NREPL'))
    setenv('FRBUS_NREPL', '1000')
end
run_stochastic_simulation_backward
