function [qid, qlabel] = frbus_parse_periods(raw)
%FRBUS_PARSE_PERIODS Parse quarterly labels such as 2040Q1, 2040 Q1, 2040:1.
if isnumeric(raw)
    % Accept YYYYQ numeric coding if present, otherwise assume row index is unusable.
    vals = raw(:);
    if all(vals > 10000)
        yr = floor(vals/10);
        qr = vals - 10*yr;
        qid = frbus_quarter_id(yr, qr);
        qlabel = arrayfun(@(y,q) sprintf('%04dQ%d', y, q), yr, qr, 'UniformOutput', false);
        return
    end
    error('Numeric period column is not in YYYYQ form. Provide a date column like 2040Q1.');
end
if iscell(raw)
    s = string(raw(:));
else
    s = string(raw(:));
end
s = strtrim(s);
yr = nan(numel(s),1);
qr = nan(numel(s),1);
for i = 1:numel(s)
    si = upper(char(s(i)));
    tok = regexp(si, '(\d{4})\s*Q\s*([1-4])', 'tokens', 'once');
    if isempty(tok)
        tok = regexp(si, '(\d{4})\s*[:\-]\s*([1-4])', 'tokens', 'once');
    end
    if isempty(tok)
        % Try MATLAB date string and infer quarter.
        try
            d = datetime(si, 'InputFormat', 'yyyy-MM-dd');
        catch
            try
                d = datetime(si);
            catch
                error('Could not parse quarterly date label: %s', si);
            end
        end
        yr(i) = year(d);
        qr(i) = quarter(d);
    else
        yr(i) = str2double(tok{1});
        qr(i) = str2double(tok{2});
    end
end
qid = frbus_quarter_id(yr, qr);
qlabel = arrayfun(@(y,q) sprintf('%04dQ%d', y, q), yr, qr, 'UniformOutput', false);
end
