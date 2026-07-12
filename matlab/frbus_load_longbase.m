function [D, qid, qlabel, T] = frbus_load_longbase(file, first_yq)
%FRBUS_LOAD_LONGBASE Load FRB/US LONGBASE data into a lower-case struct.
%
% The Fed/R/Python data files can differ slightly in separators and date column
% names. This function accepts CSV-like files with a quarterly date column named
% date, period, time, or an unnamed first column containing a sequential row
% index. The public LONGBASE sample used by the BIMETS paper has 848 quarterly
% rows from 1962Q1 to 2173Q4. Therefore a row-index-only file is interpreted
% as starting in 1962Q1 by default. Pass [year quarter] as the second argument
% when a row-index-only file uses a different start period. All variable names are
% converted to lower case.
if nargin < 1 || isempty(file)
    file = fullfile('data','LONGBASE.TXT');
end
if nargin < 2 || isempty(first_yq)
    first_yq = [1962 1];
end
if numel(first_yq) ~= 2 || first_yq(2) < 1 || first_yq(2) > 4
    error('first_yq must be a two-element [year quarter] vector.');
end
if ~isfile(file)
    error('Data file not found: %s', file);
end
opts = detectImportOptions(file, 'FileType', 'text');
opts.VariableNamingRule = 'preserve';
T = readtable(file, opts);
raw_names = T.Properties.VariableNames;
names = lower(regexprep(raw_names, '[^A-Za-z0-9_]', ''));
T.Properties.VariableNames = names;

preferred = {'date','period','time','quarter','obs'};
date_idx = [];
for k = 1:numel(preferred)
    date_idx = find(strcmp(names, preferred{k}), 1);
    if ~isempty(date_idx), break; end
end
skip_idx = date_idx;
if isempty(date_idx)
    first = T{:,1};
    if ~isnumeric(first)
        date_idx = 1;
        skip_idx = date_idx;
    elseif is_row_index(first)
        % Some exports (including the bundled frbus_data.csv) drop the
        % quarterly labels but retain a 1..N row index. Reconstruct the
        % quarterly IDs deterministically from the documented start period.
        n = height(T);
        q0 = frbus_quarter_id(first_yq(1), first_yq(2));
        qid = q0 + (0:n-1)';
        qlabel = arrayfun(@(q) sprintf('%04dQ%d', floor((q-1)/4), mod(q-1,4)+1), ...
            qid, 'UniformOutput', false);
        skip_idx = 1;
    else
        error(['Could not identify a date column. Rename it to date/period, ' ...
            'or supply a sequential row-index column.']);
    end
end
if ~exist('qid', 'var')
    [qid, qlabel] = frbus_parse_periods(T{:,date_idx});
end
D = struct();
for j = 1:width(T)
    if j == skip_idx, continue; end
    nm = names{j};
    if isempty(nm)
        error('Data column %d has no usable variable name.', j);
    end
    col = T{:,j};
    if iscell(col) || isstring(col)
        col = str2double(string(col));
    end
    D.(nm) = double(col(:));
end
D.qid = qid;
D.qlabel = qlabel;
end

function tf = is_row_index(x)
%IS_ROW_INDEX True for a numeric 1..N row-index column.
x = double(x(:));
tf = ~isempty(x) && all(isfinite(x)) && isequal(x, (1:numel(x))');
end
