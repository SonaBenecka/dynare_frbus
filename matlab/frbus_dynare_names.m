function out = frbus_dynare_names(x)
%FRBUS_DYNARE_NAMES Return Dynare name arrays as a cellstr, robust to versions.
if iscell(x)
    out = x(:);
elseif isstring(x)
    out = cellstr(x(:));
else
    out = cellstr(x);
end
out = strtrim(out(:));
end
