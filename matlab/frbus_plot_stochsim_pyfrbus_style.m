function frbus_plot_stochsim_pyfrbus_style(B, out_file, fig_title)
%FRBUS_PLOT_STOCHSIM_PYFRBUS_STYLE Four-panel stochastic-simulation fan chart.
%
% B is a table with qlabel plus baseline and central 70/90 percent intervals for
% gdp4, lur, pcxfe4 and rff. The visual convention follows the Fed/pyfrbus demo:
% baseline line with a wider 90 percent interval and a narrower 70 percent band.
if nargin < 3
    fig_title = '';
end
x = 1:height(B);
nticks = min(5, height(B));
tick_idx = unique(round(linspace(1, height(B), nticks)));

fig = figure('Visible', 'off');
tiledlayout(2, 2, 'TileSpacing', 'compact', 'Padding', 'compact');
plot_one(1, x, B.base_gdp4, B.gdp4_p05, B.gdp4_p15, B.gdp4_p85, B.gdp4_p95, ...
    'Real GDP Growth, 4-Quarter Percent Change');
plot_one(2, x, B.base_lur, B.lur_p05, B.lur_p15, B.lur_p85, B.lur_p95, ...
    'Unemployment Rate');
plot_one(3, x, B.base_pcxfe4, B.pcxfe4_p05, B.pcxfe4_p15, B.pcxfe4_p85, B.pcxfe4_p95, ...
    'Core PCE Inflation, 4-Quarter Percent Change');
plot_one(4, x, B.base_rff, B.rff_p05, B.rff_p15, B.rff_p85, B.rff_p95, ...
    'Federal Funds Rate');

if ~isempty(fig_title)
    sgtitle(fig_title);
end
exportgraphics(fig, out_file);
close(fig);

    function plot_one(tile_id, xx, base, p05, p15, p85, p95, ttl)
        nexttile(tile_id);
        hold on;
        fill([xx, fliplr(xx)], [p05(:).', fliplr(p95(:).')], [0.82 0.82 0.82], ...
            'LineStyle', 'none', 'DisplayName', '90% CI');
        fill([xx, fliplr(xx)], [p15(:).', fliplr(p85(:).')], [0.65 0.65 0.65], ...
            'LineStyle', 'none', 'DisplayName', '70% CI');
        plot(xx, base, 'LineWidth', 1.2, 'DisplayName', 'Baseline');
        hold off;
        title(ttl);
        xlabel('Date');
        ylabel('Percent');
        grid on;
        xticks(tick_idx);
        xticklabels(B.qlabel(tick_idx));
        xtickangle(0);
        if tile_id == 4
            legend('Location', 'best');
        end
    end
end
