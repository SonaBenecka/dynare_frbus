function yq = frbus_shift_yq(yq0, nquarters)
%FRBUS_SHIFT_YQ Shift a [year quarter] pair by nquarters.
qid = frbus_quarter_id(yq0(1), yq0(2)) + nquarters;
year = floor((qid - 1) / 4);
quarter = mod(qid - 1, 4) + 1;
yq = [year quarter];
end
