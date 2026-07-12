function idx = frbus_find_period(qid, year, quarter)
%FRBUS_FIND_PERIOD Locate a year/quarter in qid.
qid0 = frbus_quarter_id(year, quarter);
idx = find(qid == qid0, 1);
if isempty(idx)
    error('Period %04dQ%d not found in data.', year, quarter);
end
end
