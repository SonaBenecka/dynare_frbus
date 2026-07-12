function labels = frbus_quarter_labels_between(start_yq, end_yq)
%FRBUS_QUARTER_LABELS_BETWEEN Build YYYYQq labels for a closed period range.
q0 = frbus_quarter_id(start_yq(1), start_yq(2));
q1 = frbus_quarter_id(end_yq(1), end_yq(2));
labels = frbus_qid_to_label((q0:q1)');
end
