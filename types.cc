#include "types.hh"

string convert_reltype_to_string(rel_op_type op) {
    if (op == 0) return "NE";
    if (op == 1) return "EQ";
    if (op == 2) return "GT";
    if (op == 3) return "GE";
    if (op == 4) return "LT";
    if (op == 5) return "LE";
    return "";
}

string convert_datatype_to_string(data_type x) 
{
    if (x == 0) return "int";
    if (x == 1) return "float";
    if (x == 2) return "bool";
    if (x == 3) return "string";
    if (x == 4) return "void";
    return "";
}

string convert_optype_to_string(op_type op) 
{
    if (op == 0) return "Plus";
    if (op == 1) return "Minus";
    if (op == 2) return "Mult";
    if (op == 3) return "Div";
    return "";
}

string convert_logtype_to_string(log_op_type op) {
    if (op == 0) return "AND";
    if (op == 1) return "OR";
    return "";
}