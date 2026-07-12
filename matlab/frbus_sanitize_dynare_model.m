function M_ = frbus_sanitize_dynare_model(M_)
%FRBUS_SANITIZE_DYNARE_MODEL Remove stale auxiliary metadata after model switches.
%
% Dynare 7.1's generated driver fills M_.aux_vars by indexed assignment. When
% two models are preprocessed in one MATLAB session, a later model with fewer
% auxiliaries can retain trailing entries from the previous model. Keep only
% auxiliary definitions whose endogenous index belongs to the current model.
if isfield(M_, 'aux_vars') && ~isempty(M_.aux_vars)
    valid = false(1, numel(M_.aux_vars));
    for k = 1:numel(M_.aux_vars)
        valid(k) = isfield(M_.aux_vars(k), 'endo_index') && ...
            isfinite(M_.aux_vars(k).endo_index) && ...
            M_.aux_vars(k).endo_index >= 1 && ...
            M_.aux_vars(k).endo_index <= M_.endo_nbr;
    end
    M_.aux_vars = M_.aux_vars(valid);
end
end
