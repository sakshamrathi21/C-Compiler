# include "common-headers.hh"

map<TAC_Opd*, reg> TAC_to_reg_map;
map<string, TAC_Opd*> return_label_opd;

using namespace std;

void free_register(reg reg_num)
{
    whether_register_available[reg_num] = true;
    vector<TAC_Opd*> to_be_removed;
    for (auto p : TAC_to_reg_map)
    {
        if (p.second == reg_num)
        {
            to_be_removed.push_back(p.first);
        }
    }
    for (auto p : to_be_removed)
    {
        TAC_to_reg_map.erase(p);
    }
}

string convert_reltype_to_string1(rel_op_type op) {
    if (op == 0) return "!=";
    if (op == 1) return "==";
    if (op == 2) return ">";
    if (op == 3) return ">=";
    if (op == 4) return "<";
    if (op == 5) return "<=";
    return "";
}

string convert_optype_to_string1(op_type op) 
{
    if (op == 0) return "+";
    if (op == 1 or op == 4) return "-";
    if (op == 2) return "*";
    if (op == 3) return "/";
    return "";
}

string convert_logtype_to_string1(log_op_type op) {
    if (op == 0) return "&&";
    if (op == 1) return "||";
    if (op == 2) return "!";
    return "";
}

void Goto_TAC_Statement::print_stmt()
{
    cout << "\t" << "goto" << label->get_operand_string() << "\n";
}

void Label_TAC_Statement::print_stmt()
{
    cout << label->get_operand_string() << ":\n";
}

void If_Goto_TAC_Statement::print_stmt()
{
    cout << "\t" << "if(" << temp->get_operand_string() << ") goto " << label->get_operand_string() << "\n";
}

void Print_TAC_Statement::print_stmt()
{
    cout << "\t" << "write " << var->get_operand_string() << "\n";
}

void Read_TAC_Statement::print_stmt()
{
    cout << "\t" << "read " << var->get_operand_string() << "\n";
}


void TAC_Statement::print_stmt()
{
    
}


void Assgn_TAC_Statement::print_stmt()
{
    if (operand_type == 1 && op == COPY__h)
    {
        if (lhs != NULL) cout << "\t" << lhs->get_operand_string() << " = " << opd1->get_operand_string() << "\n";
        else cout << "\t" << opd1->get_operand_string() << "\n";
    }
    else
    {
        if (operand_type == 1) {
            if (opd2 != NULL) cout << "\t" << lhs->get_operand_string() << " = " << opd1->get_operand_string() << " " << convert_optype_to_string1(op) << " " << opd2->get_operand_string() << "\n";
            else {
                assert(op == 4);
                cout << "\t" << lhs->get_operand_string() << " = " << convert_optype_to_string1(op) << opd1->get_operand_string() << "\n";
            }
        }
        else if (operand_type == 2) {
            if (opd2 != NULL) cout << "\t" << lhs->get_operand_string() << " = " << opd1->get_operand_string() << " " << convert_reltype_to_string1(relop) << " " << opd2->get_operand_string() << "\n";
        }
        else if (operand_type == 3) {
            if (opd2 != NULL) cout << "\t" << lhs->get_operand_string() << " = " << opd1->get_operand_string() << " " << convert_logtype_to_string1(logop) << " " << opd2->get_operand_string() << "\n";
            else {
                assert(lopop == 2);
                cout << "\t" << lhs->get_operand_string() << " = " << convert_logtype_to_string1(logop) << opd1->get_operand_string() << "\n";
            }
        }

    }
}

void Call_TAC_Statement::print_stmt()
{

    cout << "\t";
    if (res != NULL)
    {
        cout << res->get_operand_string() << " = ";
    }

    cout << func_name;
    if (func_name != "main") cout << "_";
    cout << "(";

    for (int i = 0 ; i < actual_args.size() ; i++)
    {

        cout << actual_args[i]->get_operand_string();
        if (i != actual_args.size() - 1) cout << ", ";
    }

    cout << ")\n";

}

void Return_TAC_Statement::print_stmt()
{
    cout << "\treturn " << return_temp->get_operand_string() << "\n";
}

void Call_TAC_Statement::make_rtl()
{
    RTL_Statement* s1, *s2;

    for (int i = actual_args.size() - 1 ; i >= 0 ; i --)
    {
        TAC_Opd* exp = actual_args[i];
        if (exp->tac_opd_type == temporary_tac_opd)
        {
            s1 = new RTL_Push_Statememt(new RTL_Register_Const_Opd(TAC_to_reg_map[exp]), exp->dt);
            free_register(TAC_to_reg_map[exp]);
            all_rtl_statements.push_back(s1);
        }
        else if (exp->tac_opd_type == variable_tac_opd)
        {
            reg reg_arg = get_register(exp->dt == FLOAT__h);
            RTL_Register_Const_Opd* r = new RTL_Register_Const_Opd(reg_arg);
            s2 = new RTL_Move_Statement(r, new RTL_Var_Opd(static_case<Variable_TAC_Opd*>(exp)->name, exp->dt));
            free_register(reg_arg);
            s1 = new RTL_Push_Statememt(r, exp->dt);
            all_rtl_statements.push_back(s2);
            all_rtl_statements.push_back(s1);
        }
        else if (exp->tac_opd_type == int_const_tac_opd)
        {
            reg reg_arg = get_register();
            RTL_Register_Const_Opd* r = new RTL_Register_Const_Opd(reg_arg);
            s2 = new RTL_Move_Statement(r, new RTL_Int_Const_Opd(static_case<Int_Const_TAC_Opd*>(exp)->num));
            free_register(reg_arg);
            s1 = new RTL_Push_Statememt(r, exp->dt);
            all_rtl_statements.push_back(s2);
            all_rtl_statements.push_back(s1);
        }
        else if (exp->tac_opd_type == double_const_tac_opd)
        {
            reg reg_arg = get_register(true);
            RTL_Register_Const_Opd* r = new RTL_Register_Const_Opd(reg_arg);
            s2 = new RTL_Move_Statement(r, new RTL_Double_Const_Opd(static_case<Double_Const_TAC_Opd*>(exp)->num));
            free_register(reg_arg);
            s1 = new RTL_Push_Statememt(r, exp->dt);
            all_rtl_statements.push_back(s2);
            all_rtl_statements.push_back(s1);
        }
        else if (exp->tac_opd_type == saved_temporary_tac_opd)
        {
            reg reg_arg = get_register(exp->dt == FLOAT__h);
            RTL_Register_Const_Opd* r = new RTL_Register_Const_Opd(reg_arg);
            string* nm = new string(static_case<Saved_Temporary_TAC_Opd*>(exp)->get_operand_string());
            s2 = new RTL_Move_Statement(r, new RTL_Var_Opd(nm, exp->dt, true));
            free_register(reg_arg);
            s1 = new RTL_Push_Statememt(r, exp->dt);
            all_rtl_statements.push_back(s2);
            all_rtl_statements.push_back(s1);
        }
        else if (exp->tac_opd_type == string_const_tac_opd)
        {
            reg reg_arg = get_register();
            RTL_Register_Const_Opd* r = new RTL_Register_Const_Opd(reg_arg);
            s2 = new RTL_Move_Statement(r, new RTL_String_Const_Opd(static_case<String_Const_TAC_Opd*>(exp)->s));
            free_register(reg_arg);
            s1 = new RTL_Push_Statememt(r, exp->dt);
            all_rtl_statements.push_back(s2);
            all_rtl_statements.push_back(s1);
        }
    }
    RTL_Register_Const_Opd* r1 = NULL;
    reg reg_arg;
    if (res != NULL) {
        if (res->dt == FLOAT__h) reg_arg = f0;
        else reg_arg = v1;
        r1 = new RTL_Register_Const_Opd(reg_arg);
    }
    all_rtl_statements.push_back(new RTL_Call_Statement(func_name, r1));
    for (int i = 0 ; i < actual_args.size() ; i++) all_rtl_statements.push_back(new RTL_Pop_Statement(actual_args[i]->dt));
    if (res != NULL) {
        reg reg_res = get_register(res->dt == FLOAT__h);
        all_rtl_statements.push_back(new RTL_Move_Statement(new RTL_Register_Const_Opd(reg_res), r1, res->dt == FLOAT__h));
        TAC_to_reg_map[res] = reg_res;
    }
}

void Return_TAC_Statement::make_rtl()
{
    RTL_Register_Const_Opd* r;
    RTL_Statement* s1;
    if (return_temp->dt == FLOAT__h)
    {
        r = new RTL_Register_Const_Opd(f0);
    }
    else 
    {
        r = new RTL_Register_Const_Opd(v1);
    }
    string* nm = new string(static_case<Saved_Temporary_TAC_Opd*>(return_temp)->get_operand_string());
    s1 = new RTL_Move_Statement(r, new RTL_Var_Opd(nm, return_temp->dt, true));
    all_rtl_statements.push_back(s1);
    RTL_Statement* s2 = new RTL_Return_Statement(r);
    all_rtl_statements.push_back(s2);
}

void Print_TAC_Statement::make_rtl()
{
    int type_to_pass = 1;
    if (var_dt == FLOAT__h) type_to_pass = 3;
    else if (var_dt == STRING__h) type_to_pass = 4;
    reg reg_arg = get_register();
    if (reg_arg != v0)
    {
        RTL_Statement* sf = new RTL_Move_Statement(new RTL_Register_Const_Opd(reg_arg), new RTL_Register_Const_Opd(v0), var_dt == FLOAT__h);
        all_rtl_statements.push_back(sf);
        for (auto p : TAC_to_reg_map)
        {
            if (p.second == v0)
            {
                TAC_to_reg_map[p.first] = reg_arg;
                break;
            }
        }
        reg_arg = v0;
    }
    RTL_Statement* s1 = new RTL_Move_Statement(new RTL_Register_Const_Opd(reg_arg), new RTL_Int_Const_Opd(type_to_pass));
    all_rtl_statements.push_back(s1);
    free_register(reg_arg);
    reg_arg = var_dt == FLOAT__h ? f12 : a0;
    RTL_Register_Const_Opd* r = new RTL_Register_Const_Opd(reg_arg);
    RTL_Statement* s2 = NULL;
    if (var->tac_opd_type == string_const_tac_opd)
    {
        s2 = new RTL_Move_Statement(r, new RTL_String_Const_Opd(static_cast<String_Const_TAC_Opd*>(var)->s));
    }
    else if (var->tac_opd_type == variable_tac_opd)
    {
        s2 = new RTL_Move_Statement(r, new RTL_Var_Opd(static_cast<Variable_TAC_Opd*>(var)->name, var->dt));
    }
    else if (var->tac_opd_type == int_const_tac_opd)
    {
        s2 = new RTL_Move_Statement(r, new RTL_Int_Const_Opd(static_cast<Int_Const_TAC_Opd*>(var)->num));
    }
    else if (var->tac_opd_type == double_const_tac_opd)
    {
        s2 = new RTL_Move_Statement(r, new RTL_Double_Const_Opd(static_cast<Double_Const_TAC_Opd*>(var)->num));
    }
    else if (var->tac_opd_type == saved_temporary_tac_opd)
    {
        string* nm = new string(static_cast<Saved_Temporary_TAC_Opd*>(var)->get_operand_string());
        s2 = new RTL_Move_Statement(r, new RTL_Var_Opd(nm, var->dt, true));
    }
    ekse if (var->tac_opd_type == temporary_tac_opd)
    {
        s2 = new RTL_Move_Statement(r, new RTL_Register_Const_Opd(TAC_to_reg_map[var]), var->dt == FLOAT__h);
        free_register(TAC_to_reg_map[var]);
    }
    all_rtl_statements.push_back(s2);
    RTL_Statement* s3 = new RTL_Write_Statement();
    all_rtl_statements.push_back(s3);
}

void Read_TAC_Statement::make_rtl()
{
    int type_to_pass = 5;
    if (var_dt == FLOAT__h) type_to_pass = 7;
    reg reg_arg = get_register();
    RTL_Register_Const_Opd* r = new RTL_Register_Const_Opd(reg_arg);
    RTL_Statement* s1 = new RTL_Move_Statement(r, new RTL_Int_Const_Opd(type_to_pass));
    free_register(reg_arg);
    all_rtl_statements.push_back(s1);
    RTL_Statement* s2 = new RTL_Read_Statement();
    all_rtl_statements.push_back(s2);
    reg_arg = var_dt == FLOAT__h ? f0 : v0;
    r = new RTL_Register_Const_Opd(reg_arg);
    RTL_Statement* s3 = new RTL_Move_Statement(new RTL_Var_Opd(static_case<Variable_TAC_Opd*>(var)->name, var_dt), r);
    all_rtl_statements.push_back(s3);
}

void Assgn_TAC_Statement::make_rtl() {
    reg reg_arg1, reg_arg2, reg_arg3, reg_arg4;
    if (opd1->tac_opd_type != temporary_tac_opd) {
        if (opd1->dt ==FLOAT__h)
        {
            reg_arg1 = get_register(true);
        }
        else {
            reg_arg1 = get_register();
        }
    } else {
        reg_arg1 = TAC_to_reg_map[opd1];
    }
    if (opd1->dt == FLOAT__h && operand_type == 2) {
        reg_arg4 = get_register();
    }
    if (lhs != NULL) {
        if (lhs->tac_opd_type == temporary_tac_opd) { 
            if (lhs->dt == FLOAT__h) 
            {
                reg_arg2 = get_register(true);
            } 
            else {
                reg_arg2 = get_register();
            }
            TAC_to_reg_map[lhs] = reg_arg2;
        }
    }
    if (opd2 != NULL) {
        if (opd2->tac_opd_type != temporary_tac_opd) {
            if (opd2->dt == FLOAT__h) 
            {
                reg_arg3 = get_register(true);
            } 
            else 
            {
                reg_arg3 = get_register();
            }
        } else {
            reg_arg3 = TAC_to_reg_map[opd2];
        }
    }
    RTL_Register_Const_Opd* first_term = new RTL_Register_Const_Opd(reg_arg1);
    RTL_Register_Const_Opd* lhs_result = new RTL_Register_Const_Opd(reg_arg2);
    RTL_Register_Const_Opd* second_term = NULL;
    if (opd2 != NULL) second_term = new RTL_Register_Const_Opd(reg_arg3);
    RTL_Statement* s1 = NULL, *s2 = NULL, *s3 = NULL;
    if (opd1->tac_opd_type == variable_tac_opd)
    {
        s1 = new RTL_Move_Statement(first_term, new RTL_Var_Opd(static_cast<Variable_TAC_Opd*>(opd1)->name, opd1->dt));
    }
    else if (opd1->tac_opd_type == saved_temporary_tac_opd)
    {
        string* nm = new string(static_case<Saved_Temporary_TAC_Opd*>(opd1)->get_operand_string());
        s1 = new RTL_Move_Statement(first_term, new RTL_Var_Opd(nm, opd1->dt, true));
    }
    else if (opd1->tac_opd_type == int_const_tac_opd)
    {
        s1 = new RTL_Move_Statement(first_term, new RTL_Int_Const_Opd(static_case<Int_Const_TAC_Opd*>(opd1)->num));
    }
    else if (opd1->tac_opd_type == double_const_tac_opd)
    {
        s1 = new RTL_Move_Statement(first_term, new RTL_Double_Const_Opd(static_case<Double_Const_TAC_Opd*>(opd1)->num));
    }
    else if (opd1->tac_opd_type == string_const_tac_opd)
    {
        s1 = new RTL_Move_Statement(first_term, new RTL_String_Const_Opd(static_case<String_Const_TAC_Opd*>(opd1)->s));
    }
    if (opd2 != NULL)
    {
        if (opd2->tac_opd_type == variable_tac_opd)
        {
            s2 = new RTL_Move_Statement(second_term, new RTL_Var_Opd(static_cast<Variable_TAC_Opd*>(opd2)->name, opd2->dt));
        }
        else if (opd2->tac_opd_type == saved_temporary_tac_opd)
        {
            string* nm = new string(static_case<Saved_Temporary_TAC_Opd*>(opd2)->get_operand_string());
            s2 = new RTL_Move_Statement(second_term, new RTL_Var_Opd(nm, opd2->dt, true));
        }
        else if (opd2->tac_opd_type == int_const_tac_opd)
        {
            s2 = new RTL_Move_Statement(second_term, new RTL_Int_Const_Opd(static_case<Int_Const_TAC_Opd*>(opd2)->num));
        }
        else if (opd2->tac_opd_type == double_const_tac_opd)
        {
            s2 = new RTL_Move_Statement(second_term, new RTL_Double_Const_Opd(static_case<Double_Const_TAC_Opd*>(opd2)->num));
        }
        else if (opd2->tac_opd_type == string_const_tac_opd)
        {
            s2 = new RTL_Move_Statement(second_term, new RTL_String_Const_Opd(static_case<String_Const_TAC_Opd*>(opd2)->s));
        }
    }
    if (s1 != NULL) all_rtl_statements.push_back(s1);
    if (s2 != NULL) all_rtl_statements.push_back(s2);
    if (operand_type == 1) 
    {
        if (op == PLUS__h)
        {
            if (opd1->dt == FLOAT__h) s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, add_d_);
            else s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, add_);
            free_register(reg_arg1);
            free_register(reg_arg3);
        }
        else if (op == MINUS__h)
        {
            if (opd1->dt == FLOAT__h) s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, sub_d_);
            else s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, sub_);
            free_register(reg_arg1);
            free_register(reg_arg3);
        }
        else if (op == MULT__h)
        {
            if (opd1->dt == FLOAT__h) s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, mul_d_);
            else s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, mul_);
            free_register(reg_arg1);
            free_register(reg_arg3);
        }
        else if (op == DIV__h)
        {
            if (opd1->dt == FLOAT__h) s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, div_d_);
            else s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, div_);
            free_register(reg_arg1);
            free_register(reg_arg3);
        }
        else if (op == UMINUS__h)
        {
            if (opd1->dt == FLOAT__h) s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, uminus_d_);
            else s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, uminus_);
            free_register(reg_arg1);
        }
        else if (op == COPY__h) {
            string * name;
            bool isSavedTemp = false;
            if (lhs->tac_opd_type == variable_tac_opd) {
                name = static_cast<Variable_TAC_Opd*>(lhs)->name;
            }
            else {
                name = new string(static_case<Saved_Temporary_TAC_Opd*>(lhs)->get_operand_string());
                isSavedTemp = true;
            }
            RTL_Var_Opd* var_copy = new RTL_Var_Opd(name, lhs->dt, isSavedTemp);
            s3 = new RTL_Move_Statement(var_copy, first_term);
            free_register(reg_arg1);
        }
        if (s3 != NULL) all_rtl_statements.push_back(s3);
    }
    else if (operand_type == 2)
    {
        if (relop == NOT_EQUAL__h)
        {
            if (opd1->dt != FLOAT__h)
            {
                s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, sne_);
                all_rtl_statements.push_back(s3);
                free_register(reg_arg1);
                free_register(reg_arg3);
            }
            else
            {
                s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, seq_d_, 1);
                RTL_Register_Const_Opd* r = new RTL_Register_Const_Opd(reg_arg4);
                RTLStatement* s4 = new RTL_Move_Statement(r, new RTL_Int_Const_Opd(1));
                RTL_Statement* s5 = new RTL_Compute_Statement(new RTL_Register_Const_Opd(zero), lhs_result, move_, 0);
                RTL_Statement* s6 = new RTL_Compute_Statement(r, new RTL_Int_Const_Opd(0), lhs_result, movf_);
                all_rtl_statements.push_back(s3);
                all_rtl_statements.push_back(s4);
                all_rtl_statements.push_back(s5);
                all_rtl_statements.push_back(s6);
                free_register(reg_arg1);
                free_register(reg_arg3);
                free_register(reg_arg4);
            }
        }
        else if (relop == EQUAL__h)
        {
            if (opd1->dt != FLOAT__h)
            {
                s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, seq_);
                all_rtl_statements.push_back(s3);
                free_register(reg_arg1);
                free_register(reg_arg3);
            }
            else
            {
                s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, seq_d_, 1);
                RTL_Register_Const_Opd* r = new RTL_Register_Const_Opd(reg_arg4);
                RTLStatement* s4 = new RTL_Move_Statement(r, new RTL_Int_Const_Opd(1));
                RTL_Statement* s5 = new RTL_Compute_Statement(new RTL_Register_Const_Opd(zero), lhs_result, move_, 0);
                RTL_Statement* s6 = new RTL_Compute_Statement(r, new RTL_Int_Const_Opd(0), lhs_result, movt_);
                all_rtl_statements.push_back(s3);
                all_rtl_statements.push_back(s4);
                all_rtl_statements.push_back(s5);
                all_rtl_statements.push_back(s6);
                free_register(reg_arg1);
                free_register(reg_arg3);
                free_register(reg_arg4);
            }
        }
        else if (relop == GREATER_THAN__h)
        {
            if (opd1->dt != FLOAT__h)
            {
                s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, sgt_);
                all_rtl_statements.push_back(s3);
                free_register(reg_arg1);
                free_register(reg_arg3);
            }
            else
            {
                s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, sle_d_, 1);
                RTL_Register_Const_Opd* r = new RTL_Register_Const_Opd(reg_arg4);
                RTLStatement* s4 = new RTL_Move_Statement(r, new RTL_Int_Const_Opd(1));
                RTL_Statement* s5 = new RTL_Compute_Statement(new RTL_Register_Const_Opd(zero), lhs_result, move_, 0);
                RTL_Statement* s6 = new RTL_Compute_Statement(r, new RTL_Int_Const_Opd(0), lhs_result, movf_);
                all_rtl_statements.push_back(s3);
                all_rtl_statements.push_back(s4);
                all_rtl_statements.push_back(s5);
                all_rtl_statements.push_back(s6);
                free_register(reg_arg1);
                free_register(reg_arg3);
                free_register(reg_arg4);
            }
        }
        else if (relop == GREATER_THAN_EQUAL__h)
        {
            if (opd1->dt != FLOAT__h)
            {
                s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, sge_);
                all_rtl_statements.push_back(s3);
                free_register(reg_arg1);
                free_register(reg_arg3);
            }
            else
            {
                s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, slt_d_, 1);
                RTL_Register_Const_Opd* r = new RTL_Register_Const_Opd(reg_arg4);
                RTLStatement* s4 = new RTL_Move_Statement(r, new RTL_Int_Const_Opd(1));
                RTL_Statement* s5 = new RTL_Compute_Statement(new RTL_Register_Const_Opd(zero), lhs_result, move_, 0);
                RTL_Statement* s6 = new RTL_Compute_Statement(r, new RTL_Int_Const_Opd(0), lhs_result, movf_);
                all_rtl_statements.push_back(s3);
                all_rtl_statements.push_back(s4);
                all_rtl_statements.push_back(s5);
                all_rtl_statements.push_back(s6);
                free_register(reg_arg1);
                free_register(reg_arg3);
                free_register(reg_arg4);
            }
        }
        else if (relop == LESS_THAN__h)
        {
            if (opd1->dt != FLOAT__h)
            {
                s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, slt_);
                all_rtl_statements.push_back(s3);
                free_register(reg_arg1);
                free_register(reg_arg3);
            }
            else
            {
                s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, slt_d_, 1);
                RTL_Register_Const_Opd* r = new RTL_Register_Const_Opd(reg_arg4);
                RTLStatement* s4 = new RTL_Move_Statement(r, new RTL_Int_Const_Opd(1));
                RTL_Statement* s5 = new RTL_Compute_Statement(new RTL_Register_Const_Opd(zero), lhs_result, move_, 0);
                RTL_Statement* s6 = new RTL_Compute_Statement(r, new RTL_Int_Const_Opd(0), lhs_result, movt_);
                all_rtl_statements.push_back(s3);
                all_rtl_statements.push_back(s4);
                all_rtl_statements.push_back(s5);
                all_rtl_statements.push_back(s6);
                free_register(reg_arg1);
                free_register(reg_arg3);
                free_register(reg_arg4);
            }
        }
        else if (relop == LESS_THAN_EQUAL__h)
        {
            if (opd1->dt != FLOAT__h)
            {
                s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, sle_);
                all_rtl_statements.push_back(s3);
                free_register(reg_arg1);
                free_register(reg_arg3);
            }
            else
            {
                s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, sle_d_, 1);
                RTL_Register_Const_Opd* r = new RTL_Register_Const_Opd(reg_arg4);
                RTLStatement* s4 = new RTL_Move_Statement(r, new RTL_Int_Const_Opd(1));
                RTL_Statement* s5 = new RTL_Compute_Statement(new RTL_Register_Const_Opd(zero), lhs_result, move_, 0);
                RTL_Statement* s6 = new RTL_Compute_Statement(r, new RTL_Int_Const_Opd(0), lhs_result, movt_);
                all_rtl_statements.push_back(s3);
                all_rtl_statements.push_back(s4);
                all_rtl_statements.push_back(s5);
                all_rtl_statements.push_back(s6);
                free_register(reg_arg1);
                free_register(reg_arg3);
                free_register(reg_arg4);
            }
        }
    }
    else if (operand_type == 3)
    {
        if (logop == AND__h)
        {
            s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, and_);
            free_register(reg_arg1);
            free_register(reg_arg3);
        }
        else if (logop == OR__h)
        {
            s3 = new RTL_Compute_Statement(first_term, second_term, lhs_result, or_);
            free_register(reg_arg1);
            free_register(reg_arg3);
        }
        else if (logop ==NOT__h)
        {
            s3 = new RTL_Compute_Statement(first_term, lhs_result, not_, 0);
            free_register(reg_arg1);
        }
        if (s3 != NULL) all_rtl_statements.push_back(s3);
    }

}

void If_Goto_TAC_Statement::make_rtl()
{
    if (temp->tac_opd_type == variable_tac_opd)
    {
        reg reg_arg = get_register();
        RTL_Register_Const_Opd* r = new RTL_Register_Const_pd(reg_arg);
        RTL_Var_Opd* v1 = new RTL_Var_Opd(static_cast<Variable_TAC_Opd*>(temp)->name, temp->dt);
        RTL_Statement* s1 = new RTL_Move_Statement(r, v1);
        free_register(reg_arg);
        RTL_Label_Const_Opd* opd1 = new RTL_Label_Const_Opd(static_Cast<Label_TAC_Opd*>(label)->label_number);
        RTL_Statement* s2 = new RTL_If_Goto_Statement(r, opd1);
        all_rtl_statements.push_back(s1);
        all_rtl_statements.push_back(s2);
    }
    else if (temp->tac_opd_type == saved_temporary_tac_opd)
    {
        reg reg_arg = get_register();
        RTL_Register_Const_Opd* r = new RTL_Register_Const_pd(reg_arg);
        string *name = new string(static_cast<Saved_Temporary_TAC_Opd*>(temp)->get_operand_string());
        RTL_Var_Opd* v1 = new RTL_Var_Opd(name, temp->dt, true);
        RTL_Statement* s1 = new RTL_Move_Statement(r, v1);
        free_register(reg_arg);
        RTL_Label_Const_Opd* opd1 = new RTL_Label_Const_Opd(static_Cast<Label_TAC_Opd*>(label)->label_number);
        RTL_Statement* s2 = new RTL_If_Goto_Statement(r, opd1);
        all_rtl_statements.push_back(s1);
        all_rtl_statements.push_back(s2);
    }
    else if (temp->tac_opd_type == temporary_tac_opd)
    {
        RTL_Register_Const_Opd* r = new RTL_Register_Const_pd(TAC_to_reg_map[temp]);
        RTL_Label_Const_Opd* opd1 = new RTL_Label_Const_Opd(static_Cast<Label_TAC_Opd*>(label)->label_number);
        RTL_Statement* s2 = new RTL_If_Goto_Statement(r, opd1);
        all_rtl_statements.push_back(s2);
        free_register(TAC_to_reg_map[temp]);
    }
}

void Goto_TAC_Statement::make_rtl()
{
    RTL_Label_Const_Opd* opd1 = new RTL_Label_Const_Opd(static_Cast<Label_TAC_Opd*>(label)->label_number);
    RTL_Statement* s1 = new RTL_Goto_Statement(opd1);
    all_rtl_statements.push_back(s1);
}

void Label_TAC_Statement::make_rtl()
{
    RTL_Label_Const_Opd* opd1 = new RTL_Label_Const_Opd(static_Cast<Label_TAC_Opd*>(label)->label_number);
    RTL_Statement* s1 = new RTL_Label_Statement(opd1);
    all_rtl_statements.push_back(s1);
}

void Code::append_list(Code *c)
{
    if (c == NULL) return;
    for (auto it = (c->get_list())->begin() ; it != (c->get_list())->end() ; ++it)
    {
        if (*it != NULL)
            stmt_list.push_back(*it);
    }
}

void Code::print_code()
{
    for (auto it = stmt_list->begin() ; it != stmt_list->end() ; ++it)
    {
        if (*it != NULL)
            (*it)->print_stmt();
    }
}



TAC_Opd::TAC_Opd() { }

Variable_TAC_Opd::Variable_TAC_Opd(string * s, data_type dt) { name = s; this->dt = dt; tacopd_type = variable_tac_opd;}
void Variable_TAC_Opd::print_opd()      { cout << *name;}
string Variable_TAC_Opd::get_operand_string() { return *name + "_";}

int tac_temp_count = 0;

Temporary_TAC_Opd::Temporary_TAC_Opd(data_type dt)
{

    temp_number = tac_temp_count++;
    this->dt = dt; tac_opd_type = temporary_tac_opd;
}
void Temporary_TAC_Opd::print_opd()
{
    cout << "temp" << temp_number;
}
string Temporary_TAC_Opd::get_operand_string()
{
    return "temp"+to_string(temp_number);
}

int saved_tac_temp_count = 0;

extern map<string, map<int, data_type>> function_stemps;
extern string curr_func;

Saved_Temporary_TAC_Opd::Saved_Temporary_TAC_Opd(data_type dt, int label_number)
{
    saved_temp_number = label_number;
    this->dt = dt; tac_opd_type = saved_temporary_tac_opd;
    function_stemps[curr_func][saved_temp_number] = dt;
}

Saved_Temporary_TAC_Opd::Saved_Temporary_TAC_Opd(data_type dt)
{
    saved_temp_number = saved_tac_temp_count+=;
    this->dt = dt; tac_opd_type = saved_temporary_tac_opd;
    function_stemps[curr_func][saved_temp_number] = dt;
}

void Saved_Temporary_TAC_Opd::print_opd()
{
    cout <<"stemp" << saved_temp_number;
}

string Saved_Temporary_TAC_Opd::get_operand_string()
{
    return "stemp" + to_string(saved_temp_number);
}

static int tac_label_count = 0;

Label_TAC_Opd::Label_TAC_Opd() { label_number = tac_label_count++; tac_opd_type = label_tac_opd;}
void Label_TAC_Opd::print_opd()  { cout << "Label" << label_number; }
string Label_TAC_Opd::get_operand_string() { return "Label" + to_string(label_number);}

Double_Const_TAC_Opd::Double_Const_TAC_Opd(double n) { num = n; this->dt = FLOAT__h, tac_opd_type = double_const_tac_opd;}
void Double_Const_TAC_Opd::print_opd() { cout << num;}
string Double_Const_TAC_Opd::get_operand_string()
{
    ostringstream out;
    out << fixed << setprecision(2) << num;
    return out.str();
}

Int_Const_TAC_Opd::Int_Const_TAC_Opd(int n) { num = n; this->dt = INTEGER__h; tac_opd_type = int_const_tac_opd;}
void Int_Const_TAC_Opd::print_opd() { cout << num;}
string Int_Const_TAC_Opd::get_operand_string() { return to_string(num); }

String_Const_TAC_Opd::String_Const_TAC_Opd(string* str) { s = str; this->dt = STRING__h; tac_opd_type = string_const_tac_opd;}
void String_Const_TAC_Opd::print_opd() { cout <<*s;}
string String_Const_TAC_Opd::get_operand_string() { return *s; }
