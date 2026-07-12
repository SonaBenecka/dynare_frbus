function labels = frbus_qid_to_label(qid)
%FRBUS_QID_TO_LABEL Convert FRB/US numeric quarter IDs to labels such as 2040Q1.
qid = qid(:);
year = floor((qid - 1) / 4);
quarter = mod(qid - 1, 4) + 1;
labels = arrayfun(@(y,q) sprintf('%04dQ%d', y, q), year, quarter, 'UniformOutput', false);
end
