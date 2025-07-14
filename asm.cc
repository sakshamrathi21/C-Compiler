#include "asm.hh"

string reg_to_string(reg reg_num)
{
    switch (reg_num) {
        case v0: return "$v0";
        case t0: return "$t0";
        case t1: return "$t1";
        case t2: return "$t2";
        case t3: return "$t3";
        case t4: return "$t4";
        case t5: return "$t5";
        case t6: return "$t6";
        case t7: return "$t7";
        case t8: return "$t8";
        case t9: return "$t9";
        case s0: return "$s0";
        case s1: return "$s1";
        case s2: return "$s2";
        case s3: return "$s3";
        case s4: return "$s4";
        case s5: return "$s5";
        case s6: return "$s6";
        case s7: return "$s7";
        case a0: return "$a0";
        case zero: return "$zero";
        case f2: return "$f2";
        case f4: return "$f4";
        case f6: return "$f6";
        case f8: return "$f8";
        case f10: return "$f10";
        case f12: return "$f12";
        case f14: return "$f14";
        case f16: return "$f16";
        case f18: return "$f18";
        case f20: return "$f20";
        case f22: return "$f22";
        case f24: return "$f24";
        case f26: return "$f26";
        case f28: return "$f28";
        case f30: return "$f30";
        case f0: return "$f0";
        case v1: return "$v1";
        case gp: return "$gp";
        case sp: return "$sp";
        case fp: return "$fp";
        case ra: return "$ra";
    }
    return "";
}

string ins_to_string(asm_instruction ins)
{
    string to_be_returned = "";
    switch (ins) {
        case _push: to_be_returned = "push"; break;
        case _pop: to_be_returned = "pop"; break;
        case _iLoad: to_be_returned = "iLoad"; break;
        case iLoad_d: to_be_returned = "iLoad.d"; break;
        case _load: to_be_returned = "load"; break;
        case _load_d: to_be_returned = "load.d"; break;
        case _load_addr: to_be_returned = "load_addr"; break;
        case _store: to_be_returned = "store"; break;
        case _store_d: to_be_returned = "store.d"; break;
        case _and: to_be_returned = "and"; break;
        case _or: to_be_returned = "or"; break;
        case _not: to_be_returned = "not"; break;
        case _move: to_be_returned = "move"; break;
        case _move_d: to_be_returned = "mov.d"; break;
        case _movf: to_be_returned = "movf"; break;
        case _movt: to_be_returned = "movt"; break;
        case _beq: to_be_returned = "beq"; break;
        case _bne: to_be_returned = "bne"; break;
        case _bgtz: to_be_returned = "bgtz"; break;
        case _bgez: to_be_returned = "bgez"; break;
        case _bltz: to_be_returned = "bltz"; break;
        case _blez: to_be_returned = "blez"; break;
        case _bclt: to_be_returned = "bclt"; break;
        case _bclf: to_be_returned = "bclf"; break;
        case _goto: to_be_returned = "goto"; break;
        case _call: to_be_returned = "call"; break;
        case _return: to_be_returned = "return"; break;
        case _label: to_be_returned = "label"; break;
        case _add: to_be_returned = "add"; break;
        case _sub: to_be_returned = "sub"; break;
        case _mul: to_be_returned = "mul"; break;
        case _div: to_be_returned = "div"; break;
        case _add_i: to_be_returned = "addi"; break;
        case _uminus:  to_be_returned = "neg"; break;
        case _add_d: to_be_returned = "add.d"; break;
        case _sub_d: to_be_returned = "sub.d"; break;
        case _mul_d: to_be_returned = "mul.d"; break;
        case _div_d: to_be_returned = "div.d"; break;
        case _uminus_d: to_be_returned = "neg.d"; break;
        case _slt: to_be_returned = "slt"; break;
        case _sle: to_be_returned = "sle"; break;
        case _sgt: to_be_returned = "sgt"; break;
        case _sge: to_be_returned = "sge"; break;
        case _seq: to_be_returned = "seq"; break;
        case _sne: to_be_returned = "sne"; break;
        case _seq_d: to_be_returned = "seq.d"; break;
        case _slt_d: to_be_returned = "slt.d"; break;
        case _sle_d: to_be_returned = "sle.d"; break;
        case _sgt_d: to_be_returned = "sgt.d"; break;
        case _sge_d: to_be_returned = "sge.d"; break;
        case _sne_d: to_be_returned = "sne.d"; break;
        case _write: to_be_returned = "write"; break;
        case _read: to_be_returned = "read"; break;
        case _nop: to_be_returned = "nop"; break;
        case _li: to_be_returned = "li"; break;
        case _li_d: to_be_returned = "li.d"; break;
        case _lw: to_be_returned = "lw"; break;
        case _l_d: to_be_returned = "l.d"; break;
        case _la: to_be_returned = "la"; break;
        case _lw_w: to_be_returned = "lw.w"; break;
        case _sw: to_be_returned = "sw"; break;
        case s_d: to_be_returned = "s.d"; break;
        case _xori: to_be_returned = "xori"; break;
        case _mov_d: to_be_returned = "mov.d"; break;
        case _j: to_be_returned = "j"; break;
        case _jal: to_be_returned = "jal"; break;
        case _neg: to_be_returned = "neg"; break;
        case _neg_d: to_be_returned = "neg.d"; break;
        case _c_eq_d: to_be_returned = "c.eq.d"; break;
        case _c_lt_d: to_be_returned = "c.lt.d"; break;
        case _c_le_d: to_be_returned = "c.le.d"; break;
        case _syscall: to_be_returned = "syscall"; break;
        case _jr: to_be_returned = "jr"; break;
    }
    return to_be_returned + " ";
}


Syscall_ASM_Statement::Syscall_ASM_Statement()
{
    asm_ins = _syscall;
}

void Syscall_ASM_Statement::print_asm()
{
    cout << "\tsyscall\n";
}

Compute_ASM_Statement::Compute_ASM_Statement(ASM_Opd* first1, ASM_Opd* second1, ASM_Opd* res1, asm_instruction asm_ins1)
{
    first = first1;
    second = second1;
    res = res1;
    asm_ins = asm_ins1;
}

Compute_ASM_Statement::Compute_ASM_Statement(ASM_Opd* first1, ASM_Opd* second1, ASM_Opd* res1, instruction rtl_ins)
{
    first = first1;
    second = second1;
    res = res1;
    if (rtl_ins == add_d_) asm_ins = _add_d;
    else if (rtl_ins == add_) asm_ins = _add;
    else if (rtl_ins == sub_d_) asm_ins = _sub_d;
    else if (rtl_ins == sub_) asm_ins = _sub;
    else if (rtl_ins == mul_d_) asm_ins = _mul_d;
    else if (rtl_ins == mul_) asm_ins = _mul;
    else if (rtl_ins == div_d_) asm_ins = _div_d;
    else if (rtl_ins == div_) asm_ins = _div;
    else if (rtl_ins == uminus_d_) asm_ins = _uminus_d;
    else if (rtl_ins == uminus_) asm_ins = _uminus;
    else if (rtl_ins == sne_) asm_ins = _sne;
    else if (rtl_ins == seq_d_) asm_ins = _c_eq_d;
    else if (rtl_ins == seq_) asm_ins = _seq;
    else if (rtl_ins = move_) asm_ins = _move;
    else if (rtl_ins == movt_) asm_ins = _movt;
    else if (rtl_ins == movf_) asm_ins = _movf;
    else if (rtl_ins == sqt_) asm_ins = _sgt;
    else if (rtl_ins == sle_d_) asm_ins = _c_le_d;
    else if (rtl_ins == sge_) asm_ins = _sge;
    else if (rtl_ins == slt_d_) asm_ins = _c_lt_d;
    else if (rtl_ins == slt_) asm_ins = _slt;
    else if (rtl_ins == sle_) asm_ins = _sle;
    else if (rtl_ins == and_) asm_ins = _and;
    else if (rtl_ins == or_) asm_ins = _or;
    else if (rtl_ins == not_) {asm_ins = _xr1; second = new ASM_Int_Const_Opd(1);};
}

ASM_Mem_Opd::ASM_Mem_Opd(int offset, reg base_reg) : offset(offset), base_reg(base_reg)
{

}

ASM_Mem_Opd::ASM_Mem_Opd(string var_name) : var_name(var_name)
{

}

string ASM_Mem_Opd:;get_operand_string()
{
    if (var_name != "") return var_name + "_";
    return to_string(offset) + "(" + reg_to_string(base_reg) + ")";
}

ASM_Register_Opd::ASM_Register_Opd(reg reg_num) : reg_num(reg_num)
{

}

string ASM_Register_Opd::get_operand_string()
{
    return reg_to_string(reg_num);
}

string ASM_String_Const_Opd::get_operand_string()
{
    return "_str_" + to_string(string_number);
}

ASM_String_Const_Opd::ASM_String_Const_Opd(int string_number) : string_number(string_number)
{

}

ASM_Label_Opd::ASM_Label_Opd(int label_number) : label_number(label_number)
{

}

string ASM_Label_Opd::get_operand_string()
{
    return "Label" + to_string(label_number);
}

ASM_Int_Const_Opd::ASM_Int_Const_Opd(int value) : value(value)
{

}

string ASM_Int_Const_Opd::get_operand_string()
{
    return to_string(value);
}

ASM_Double_Const_Opd::ASM_Double_Const_Opd(double value) : value(value)
{

}

string ASM_Double_Const_Opd::get_operand_string()
{
    ostringstream out;
    out << fixed << setprecision(2) << value;
    return out.str();
}

Label_ASM_Statement::Label_ASM_Statement(ASM_Opd* label)
    : label(label) {
    label_str = label->get_operand_string();
}

Goto_ASM_Statement::Goto_ASM_Statement(ASM_Opd* label)
    : label(label) {}

void Goto_ASM_Statement::print_asm()
{
    cout << "\tj " << label->get_operand_string() << "\n";
}

Call_ASM_Statement::Call_ASM_Statement(string func_name) : func_name(func_name) 
{
    asm_ins = _jal;
}

void Call_ASM_Statement::print_asm()
{
    cout << "\t" << ins_to_string(asm_ins) << " " << func_name;
    if (func_name != "main") 
    {
        cout << "_";
    }
    cout << "\n";
}

If_Goto_ASM_Statement::If_Goto_ASM_Statement(ASM_Opd* r, ASM_Opd* l) : reg(r), label(l)
{
    asm_ins = _bgtz;
}

void If_Goto_ASM_Statement::print_asm()
{
    cout << "\tbgtz" << reg->get_operand_string() << ", " << label->get_operand_string() << "\n";
}

void Label_ASM_Statement::print_asm()
{
    cout << "\n " << label_str << ":\n";
}

Jump_Reg_ASM_Statement::Jump_Reg_ASM_Statement(ASM_Opd* r) : reg(r)
{
    asm_ins = _jr;
}

void Jump_Reg_ASM_Statement::print_asm()
{
    cout << "\tjr" << reg->get_operand_string() << "\n";
}

Move_ASM_Statement::Move_ASM_Statement(ASM_Opd* first1, ASM_Opd* res1, instruction rtl_ins)
{
    first = first1;
    res = res1;
    if (rtl_ins == load_d_) asm_ins = _l_d;
    else if (rtl_ins == load_) asm_ins = _lw;
    else if (rtl_ins == store_d_) asm_ins = _s_d;
    else if (rtl_ins == store_) asm_ins = _sw;
    else if (rtl_ins == iLoad_) asm_ins = _li;
    else if (rtl_ins == iLoad_d_) asm_ins = _li_d;
    else if (rtl_ins == load_addr_) asm_ins = _la;
    else if (rtl_ins == move_d_) asm_ins = _move_d;
    else if (rtl_ins == move_) asm_ins = _move;
    if (asm_ins == _s_d || asm_ins == _sw){
        ASM_Opd* temp = first;
        first = res;
        res = temp;
    }
}

Move_ASM_Statement::Move_ASM_Statement(ASM_Opd* first1, ASM_Opd* res1, asm_instruction asm_ins1)
{
    first = first1;
    res = res1;
    asm_ins = asm_ins1;
    if (asm_ins == _s_d || asm_ins == _sw){
        ASM_Opd* temp = first;
        first = res;
        res = temp;
    }
}

Return_ASM_Statement::Return_ASM_Statement(string curr_func) 
    : curr_func(curr_func) {}

void Return_ASM_Statement::print_asm()
{
    cout << "\tj epilogue_" << curr_func << "\n";
}

void Compute_ASM_Statement::print_asm()
{
    cout << "\t" << ins_to_string(asm_ins);
    if (res != NULL) 
    {
        cout << res->get_operand_string() << ", ";
    }
    cout << first->get_operand_string();
    if (second != NULL) 
    {
        cout << ", " << second->get_operand_string();
    }
    cout << "\n";
}

void Move_ASM_Statement::print_asm()
{
    cout << "\t" << ins_to_string(asm_ins) << res->get_operand_string() << ", " << first->get_operand_string() << "\n";
}

Label_ASM_Statement::Label_ASM_Statement(string str) : label_str(str) 
{

}