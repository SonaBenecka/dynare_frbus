function S = frbus_extract_simul_table(oo_, M_, window, wanted)
%FRBUS_EXTRACT_SIMUL_TABLE Extract selected simulated variables from oo_.endo_simul.
if nargin < 4 || isempty(wanted)
    wanted = {'hggdp','lur','picxfe','rff','xgdp'};
end
endo_names = lower(frbus_dynare_names(M_.endo_names));
S = table();
S.qid = window.qid(window.sim_cols(:));
for k = 1:numel(wanted)
    nm = lower(wanted{k});
    j = find(strcmp(endo_names, nm), 1);
    if isempty(j)
        warning('Variable %s not found among endogenous variables.', nm);
        continue
    end
    S.(nm) = oo_.endo_simul(j, window.sim_cols).';
end
end
