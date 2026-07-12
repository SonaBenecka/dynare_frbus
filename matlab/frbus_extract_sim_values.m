function Y = frbus_extract_sim_values(oo_, M_, window, wanted)
%FRBUS_EXTRACT_SIM_VALUES Return T-by-K matrix of simulated endogenous values.
endo_names = lower(frbus_dynare_names(M_.endo_names));
Y = nan(numel(window.sim_cols), numel(wanted));
for k = 1:numel(wanted)
    nm = lower(wanted{k});
    j = find(strcmp(endo_names, nm), 1);
    if isempty(j)
        error('Endogenous variable %s not found in Dynare model.', nm);
    end
    Y(:, k) = oo_.endo_simul(j, window.sim_cols).';
end
end
