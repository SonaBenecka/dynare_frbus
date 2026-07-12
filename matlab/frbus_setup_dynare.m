function dynare_path = frbus_setup_dynare()
%FRBUS_SETUP_DYNARE Add a local Dynare installation to the MATLAB path.
%
% Set FRBUS_DYNARE_PATH to the Dynare matlab directory when Dynare is not
% installed in one of the common Windows locations. The helper returns the
% directory that supplied dynare.m and errors early with an actionable message.
if exist('dynare', 'file') == 2
    dynare_path = fileparts(which('dynare'));
    return
end

candidates = {};
env_path = getenv('FRBUS_DYNARE_PATH');
if ~isempty(env_path)
    candidates{end+1} = env_path; %#ok<AGROW>
end
candidates = [candidates, { ...
    'C:\dynare\7.1\matlab', ...
    'C:\dynare\matlab', ...
    fullfile(getenv('USERPROFILE'), 'dynare', 'matlab') ...
    }];

for k = 1:numel(candidates)
    candidate = candidates{k};
    if isfolder(candidate) && isfile(fullfile(candidate, 'dynare.m'))
        addpath(candidate);
        dynare_path = candidate;
        return
    end
end

error(['Dynare was not found on the MATLAB path. Install Dynare or set ' ...
    'the FRBUS_DYNARE_PATH environment variable to its matlab folder.']);
end
