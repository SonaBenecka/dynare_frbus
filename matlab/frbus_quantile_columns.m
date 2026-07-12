function q = frbus_quantile_columns(X, probs)
%FRBUS_QUANTILE_COLUMNS Quantiles by row without requiring Statistics Toolbox.
%
% X is T-by-N. probs are percentages in [0,100]. q is T-by-numel(probs).
if isempty(X)
    q = nan(size(X, 1), numel(probs));
    return
end
if any(probs < 0 | probs > 100)
    error('Quantile probabilities must be in [0,100].');
end
[T, N] = size(X);
q = nan(T, numel(probs));
for t = 1:T
    row = sort(X(t, isfinite(X(t, :))));
    n = numel(row);
    if n == 0
        continue
    end
    for p = 1:numel(probs)
        h = 1 + (n - 1) * probs(p) / 100;
        lo = floor(h);
        hi = ceil(h);
        if lo == hi
            q(t, p) = row(lo);
        else
            q(t, p) = row(lo) + (h - lo) * (row(hi) - row(lo));
        end
    end
end
end
