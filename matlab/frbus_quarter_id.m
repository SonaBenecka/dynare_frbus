function q = frbus_quarter_id(year, quarter)
%FRBUS_QUARTER_ID Numeric quarterly index used internally: year*4 + quarter.
q = year .* 4 + quarter;
end
