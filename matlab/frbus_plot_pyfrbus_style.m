function frbus_plot_pyfrbus_style(P, out_file, fig_title)
%FRBUS_PLOT_PYFRBUS_STYLE Save a four-panel plot using Fed/pyfrbus conventions.
if nargin < 3
    fig_title = '';
end
x = 1:height(P);
nticks = min(5, height(P));
tick_idx = unique(round(linspace(1, height(P), nticks)));

fig = figure('Visible', 'off');
tiledlayout(2, 2, 'TileSpacing', 'compact', 'Padding', 'compact');

plot_one(1, x, P.base_gdp4, P.sim_gdp4, 'Real GDP Growth, 4-Quarter Percent Change');
plot_one(2, x, P.base_lur, P.sim_lur, 'Unemployment Rate');
plot_one(3, x, P.base_pcxfe4, P.sim_pcxfe4, 'Core PCE Inflation, 4-Quarter Percent Change');
plot_one(4, x, P.base_rff, P.sim_rff, 'Federal Funds Rate');

if ~isempty(fig_title)
    sgtitle(fig_title);
end
exportgraphics(fig, out_file);
close(fig);

    function plot_one(tile_id, xx, base, sim, ttl)
        nexttile(tile_id);
        plot(xx, base, 'LineWidth', 1.2); hold on;
        plot(xx, sim, '--', 'LineWidth', 1.2); hold off;
        title(ttl);
        xlabel('Date');
        ylabel('Percent');
        grid on;
        xticks(tick_idx);
        xticklabels(P.qlabel(tick_idx));
        xtickangle(0);
        if tile_id == 1
            legend({'Baseline','Sim'}, 'Location', 'best');
        end
    end
end
