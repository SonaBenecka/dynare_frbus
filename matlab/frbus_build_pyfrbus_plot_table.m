function P = frbus_build_pyfrbus_plot_table(D, qid, oo_, M_, window, plot_start_yq, plot_end_yq)
%FRBUS_BUILD_PYFRBUS_PLOT_TABLE Build baseline/simulation paths for FRB/US demo plots.
%
% The Fed pyfrbus sim_plot convention plots GDP and core PCE inflation as
% four-quarter percent changes computed from the levels xgdp and pcxfe:
%   100 * (level(t) / level(t-4) - 1)
% It does not plot the endogenous hggdp or picxfe series directly. This matters
% because hggdp/picxfe carry their own tracking residuals in the translated
% system. Using the level-based convention makes the Dynare plots comparable
% with pyfrbus/BIMETS figures.
plot_start_idx = frbus_find_period(qid, plot_start_yq(1), plot_start_yq(2));
plot_end_idx   = frbus_find_period(qid, plot_end_yq(1), plot_end_yq(2));
if plot_start_idx - 4 < 1
    error('Need at least four quarters before plot_start_yq to compute 4-quarter growth.');
end
plot_idx = (plot_start_idx:plot_end_idx)';

endo_names = lower(frbus_dynare_names(M_.endo_names));
needed = {'xgdp','pcxfe','lur','rff','hggdp','picxfe'};
sim_full = struct();
for k = 1:numel(needed)
    nm = needed{k};
    if ~isfield(D, nm)
        error('Data are missing required variable %s.', nm);
    end
    sim_full.(nm) = D.(nm)(:);
    j = find(strcmp(endo_names, nm), 1);
    if isempty(j)
        error('Endogenous variable %s not found in Dynare model.', nm);
    end
    sim_full.(nm)(window.start_idx:window.end_idx) = oo_.endo_simul(j, window.sim_cols).';
end

P = table();
P.qid = qid(plot_idx);
P.qlabel = frbus_qid_to_label(P.qid);
P.base_gdp4 = 100 * (D.xgdp(plot_idx) ./ D.xgdp(plot_idx - 4) - 1);
P.sim_gdp4  = 100 * (sim_full.xgdp(plot_idx) ./ sim_full.xgdp(plot_idx - 4) - 1);
P.base_pcxfe4 = 100 * (D.pcxfe(plot_idx) ./ D.pcxfe(plot_idx - 4) - 1);
P.sim_pcxfe4  = 100 * (sim_full.pcxfe(plot_idx) ./ sim_full.pcxfe(plot_idx - 4) - 1);
P.base_lur = D.lur(plot_idx);
P.sim_lur  = sim_full.lur(plot_idx);
P.base_rff = D.rff(plot_idx);
P.sim_rff  = sim_full.rff(plot_idx);

% Diagnostics: the translated endogenous annualized/instantaneous rates are
% retained for comparison with source-model paths and plotting conventions.
P.base_hggdp = D.hggdp(plot_idx);
P.sim_hggdp  = sim_full.hggdp(plot_idx);
P.base_picxfe = D.picxfe(plot_idx);
P.sim_picxfe  = sim_full.picxfe(plot_idx);
P.base_xgdp = D.xgdp(plot_idx);
P.sim_xgdp  = sim_full.xgdp(plot_idx);
P.base_pcxfe = D.pcxfe(plot_idx);
P.sim_pcxfe  = sim_full.pcxfe(plot_idx);
end
