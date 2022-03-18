//
// Created by Spencer DeLange on 1/16/22.
//

#include <sstream>
#include "Expr.h"
#include "Val.h"
#include "Env.h"

//////////////////////////////
//// Expr implementations ////
//////////////////////////////
std::string Expr::to_string() {
    std::stringstream out("");
    this->print(out);
    return out.str();
}
std::string Expr::pretty_to_string() {
    std::stringstream out("");
    this->pretty_print(out);
    return out.str();
}

Expr::~Expr() {}

/////////////////////////////////
//// NumExpr implementations ////
/////////////////////////////////
NumExpr::NumExpr(int rep) {
    this->rep = rep;
}
bool NumExpr::equals(PTR(Expr) e) {
    PTR(NumExpr) n = CAST(NumExpr)(e);
    if (n == nullptr)
        return false;
    else
        return (this->rep == n->rep);
}
PTR(Val) NumExpr::interp(PTR(Env) env) {
    return NEW(NumVal)(rep);
}
void NumExpr::print(std::ostream& output) {
    output << this->rep;
}
void NumExpr::pretty_print(std::ostream &output) {
    pretty_print_at(output, prec_none, 0, false);
}
void NumExpr::pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) {
    output << this->rep;
}

//////////////////////////////////
//// BoolExpr implementations ////
//////////////////////////////////
BoolExpr::BoolExpr(bool rep){
    this->rep = rep;
}
bool BoolExpr::equals(PTR(Expr) e){
    PTR(BoolExpr) v = CAST(BoolExpr)(e);
    if (v == nullptr)
        return false;
    else
        return (this->rep == v->rep);
}
PTR(Val) BoolExpr::interp(PTR(Env) env) {
    return NEW(BoolVal)(rep);
}
void BoolExpr::print(std::ostream &output) {
    if(this->rep)
        output << "_true";
    else
        output << "_false";
}
void BoolExpr::pretty_print(std::ostream &output) {
    pretty_print_at(output, prec_none, 0, false);
}
void BoolExpr::pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) {
    if(this->rep)
        output << "_true";
    else
        output << "_false";
}

/////////////////////////////////
//// VarExpr implementations ////
/////////////////////////////////
VarExpr::VarExpr(std::string name){
    this->name = name;
}
bool VarExpr::equals(PTR(Expr) e){
    PTR(VarExpr) v = CAST(VarExpr)(e);
    if (v == nullptr)
        return false;
    else
        return (this->name == v->name);
}
PTR(Val) VarExpr::interp(PTR(Env) env) {
    return env->lookup(name);
}
void VarExpr::print(std::ostream &output) {
    output << this->name;
}
void VarExpr::pretty_print(std::ostream &output) {
    pretty_print_at(output, prec_none, 0, false);
}
void VarExpr::pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) {
    output << this->name;
}

/////////////////////////////////
//// EqExpr implementations /////
/////////////////////////////////
EqExpr::EqExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool EqExpr::equals(PTR(Expr) e) {
    PTR(EqExpr) a = CAST(EqExpr)(e);
    if (a == nullptr)
        return false;
    else
        return (this->lhs->equals(a->lhs)
                && this->rhs->equals(a->rhs));
}
PTR(Val) EqExpr::interp(PTR(Env) env) {
    return NEW(BoolVal)(lhs->interp(env)->equals(rhs->interp(env)));
}
void EqExpr::print(std::ostream &output) {
    output << "(";
    this->lhs->print(output);
    output << "==";
    this->rhs->print(output);
    output << ")";
}
void EqExpr::pretty_print(std::ostream &output) {
    lhs->pretty_print_at(output, prec_none, 0, false);
    output << " == ";
    rhs->pretty_print_at(output, prec_none, 0, false);
}
void EqExpr::pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) {
    if(prec > prec_none){
        output << "(";
        lhs->pretty_print_at(output, prec_none, lastNewLine, false);
        output << " == ";
        rhs->pretty_print_at(output, prec_none, lastNewLine, false);
        output << ")";
    } else {
        lhs->pretty_print_at(output, prec_none, lastNewLine, false);
        output << " == ";
        rhs->pretty_print_at(output, prec_none, lastNewLine, false);
    }
}

/////////////////////////////////
//// AddExpr implementations ////
/////////////////////////////////
AddExpr::AddExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool AddExpr::equals(PTR(Expr) e) {
    PTR(AddExpr) a = CAST(AddExpr)(e);
    if (a == nullptr)
        return false;
    else
        return (this->lhs->equals(a->lhs)
                && this->rhs->equals(a->rhs));
}
PTR(Val) AddExpr::interp(PTR(Env) env) {
    return lhs->interp(env)->add_to(rhs->interp(env));
}
void AddExpr::print(std::ostream &output) {
    output << "(";
    this->lhs->print(output);
    output << "+";
    this->rhs->print(output);
    output << ")";
}
void AddExpr::pretty_print(std::ostream &output) {
    lhs->pretty_print_at(output, prec_add, 0, false);
    output << " + ";
    rhs->pretty_print_at(output, prec_none, 0, true);
}
void AddExpr::pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) {
    if(prec > prec_none){
        output << "(";
        lhs->pretty_print_at(output, prec_add, lastNewLine, false);
        output << " + ";
        rhs->pretty_print_at(output, prec_add, lastNewLine, false);
        output << ")";
    } else {
        lhs->pretty_print_at(output, prec_add, lastNewLine, false);
        output << " + ";
        rhs->pretty_print_at(output, prec_add, lastNewLine, false);
    }
}

//////////////////////////////////
//// MultExpr implementations ////
//////////////////////////////////
MultExpr::MultExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool MultExpr::equals(PTR(Expr) e) {
    PTR(MultExpr) m = CAST(MultExpr)(e);
    if (m == nullptr)
        return false;
    else
        return (this->lhs->equals(m->lhs)
                && this->rhs->equals(m->rhs));
}
PTR(Val) MultExpr::interp(PTR(Env) env) {
    return lhs->interp(env)->mult_to(rhs->interp(env));
}
void MultExpr::print(std::ostream &output) {
    output << "(";
    this->lhs->print(output);
    output << "*";
    this->rhs->print(output);
    output << ")";
}
void MultExpr::pretty_print(std::ostream &output) {
    lhs->pretty_print_at(output, prec_mult, 0, false);
    output << " * ";
    rhs->pretty_print_at(output, prec_add, 0, true);
}
void MultExpr::pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) {
    if(prec == prec_mult){
        output << "(";
        lhs->pretty_print_at(output, prec_mult, lastNewLine, false);
        output << " * ";
        rhs->pretty_print_at(output, prec_mult, lastNewLine, false);
        output << ")";
    } else {
        lhs->pretty_print_at(output, prec_mult, lastNewLine, false);
        output << " * ";
        rhs->pretty_print_at(output, prec_mult, lastNewLine, false);
    }
}

/////////////////////////////////
//// IfExpr implementations ////
/////////////////////////////////
IfExpr::IfExpr(PTR(Expr) _if, PTR(Expr) _then, PTR(Expr) _else) {
    this->_if = _if;
    this->_then = _then;
    this->_else = _else;
}
bool IfExpr::equals(PTR(Expr) e) {
    PTR(IfExpr) l = CAST(IfExpr)(e);
    if (l == nullptr)
        return false;
    else
        return (this->_if->equals(l->_if)
                && this->_then->equals(l->_then) && this->_else->equals(l->_else));
}
PTR(Val) IfExpr::interp(PTR(Env) env) {
    if(_if->interp(env)->is_true())
        return _then->interp(env);
    else
        return _else->interp(env);
}
void IfExpr::print(std::ostream &output) {
    output << "(_if " << this->_if->to_string() << " _then " << this->_then->to_string() << " _else " << this->_else->to_string() << ")";
}
void IfExpr::pretty_print(std::ostream &output) {
    pretty_print_at(output, prec_none, 0, true);
}
void IfExpr::pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) {
    int numSpaces = (int)output.tellp() - lastNewLine;
    if(prec > prec_none && !letParens){
        std::string spaces(numSpaces+1, ' ');
        output << "(_if ";
        _if->pretty_print_at(output, prec_none, lastNewLine, false);
        output << "\n" << spaces << "_then ";
        int tempLastNewLine = (int)output.tellp();
        _then->pretty_print_at(output, prec_none, tempLastNewLine, false);
        output << "\n" << spaces << "_else ";
        tempLastNewLine = (int)output.tellp();
        _else->pretty_print_at(output, prec_none, tempLastNewLine, false);
        output << ")";
    } else {
        std::string spaces(numSpaces, ' ');
        output << "_if ";
        _if->pretty_print_at(output, prec_none, lastNewLine, false);
        output << "\n" << spaces << "_then ";
        int tempLastNewLine = (int)output.tellp();
        _then->pretty_print_at(output, prec_none, tempLastNewLine, false);
        output << "\n" << spaces << "_else ";
        tempLastNewLine = (int)output.tellp();
        _else->pretty_print_at(output, prec_none, tempLastNewLine, false);
    }
}

/////////////////////////////////
//// LetExpr implementations ////
/////////////////////////////////
LetExpr::LetExpr(PTR(VarExpr) lhs, PTR(Expr) rhs, PTR(Expr) body) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}
bool LetExpr::equals(PTR(Expr) e) {
    PTR(LetExpr) l = CAST(LetExpr)(e);
    if (l == nullptr)
        return false;
    else
        return (this->lhs->equals(l->lhs)
                && this->rhs->equals(l->rhs) && this->body->equals(l->body));
}
PTR(Val) LetExpr::interp(PTR(Env) env) {
    PTR(Val) rhs_val = rhs->interp(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(lhs->name, rhs_val, env);
    return body->interp(new_env);
}
void LetExpr::print(std::ostream &output) {
    output << "(_let " << this->lhs->name << "=" << this->rhs->to_string() << " _in " << this->body->to_string() << ")";
}
void LetExpr::pretty_print(std::ostream &output) {
    pretty_print_at(output, prec_none, 0, true);
}
void LetExpr::pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) {
    int numSpaces = (int)output.tellp() - lastNewLine;
    if(prec > prec_none && !letParens){
        std::string spaces(numSpaces+1, ' ');
        output << "(_let ";
        lhs->pretty_print_at(output, prec_none, lastNewLine, false);
        output << " = ";
        rhs->pretty_print_at(output, prec_none, lastNewLine, false);
        int tempLastNewLine = (int)output.tellp();
        output << "\n" << spaces << "_in  ";
        body->pretty_print_at(output, prec_none, tempLastNewLine, false);
        output << ")";
    } else {
        std::string spaces(numSpaces, ' ');
        output << "_let ";
        lhs->pretty_print_at(output, prec_none, lastNewLine, false);
        output << " = ";
        rhs->pretty_print_at(output, prec_none, lastNewLine, false);
        int tempLastNewLine = (int)output.tellp()+1;
        output << "\n" << spaces << "_in  ";
        body->pretty_print_at(output, prec_none, tempLastNewLine, false);
    }
}
/////////////////////////////////
//// FunExpr implementations ////
/////////////////////////////////
FunExpr::FunExpr(PTR(VarExpr) formal_arg, PTR(Expr) body) {
    this->formal_arg = formal_arg;
    this->body = body;
}
bool FunExpr::equals(PTR(Expr) e) {
    PTR(FunExpr) l = CAST(FunExpr)(e);
    if (l == nullptr)
        return false;
    else
        return this->formal_arg == l->formal_arg && this->body->equals(l->body);
}
PTR(Val) FunExpr::interp(PTR(Env) env) {
    return NEW(FunVal)(formal_arg->name, body, env);
}
void FunExpr::print(std::ostream &output) {
    output << "(_fun (" << this->formal_arg->name << ") " << body->to_string() << ")";
}
void FunExpr::pretty_print(std::ostream &output) {
    pretty_print_at(output, prec_none, 0, true);
}
void FunExpr::pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) {
    int numSpaces = (int)output.tellp() - lastNewLine;
    if(prec > prec_none && !letParens){
        std::string spaces(numSpaces+3, ' ');
        output << "(_fun (";
        formal_arg->pretty_print_at(output, prec_none, lastNewLine, false);
        output << ")";
        int tempLastNewLine = (int)output.tellp();
        output << "\n" << spaces;
        body->pretty_print_at(output, prec_none, tempLastNewLine, false);
        output << ")";
    } else {
        std::string spaces(numSpaces+2, ' ');
        output << "_fun (";
        formal_arg->pretty_print_at(output, prec_none, lastNewLine, false);
        output << ")";
        int tempLastNewLine = (int)output.tellp()+1;
        output << "\n" << spaces;
        body->pretty_print_at(output, prec_none, tempLastNewLine, false);
    }
}
/////////////////////////////////
//// CallExpr implementations ////
/////////////////////////////////
CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}
bool CallExpr::equals(PTR(Expr) e) {
    PTR(CallExpr) l = CAST(CallExpr)(e);
    if (l == nullptr)
        return false;
    else
        return this->to_be_called->equals(l->to_be_called) && this->actual_arg->equals(l->actual_arg);
}
PTR(Val) CallExpr::interp(PTR(Env) env) {
    PTR(Val) v1 = to_be_called->interp(env);
    PTR(Val) v2 = actual_arg->interp(env);
    PTR(Val) v3 = v1->call(v2);
    return v3;
}
void CallExpr::print(std::ostream &output) {
    output << to_be_called->to_string() << "(" << actual_arg->to_string() << ")";
}
void CallExpr::pretty_print(std::ostream &output) {
    pretty_print_at(output, prec_none, 0, true);
}
void CallExpr::pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) {
    to_be_called->pretty_print_at(output, prec, lastNewLine, letParens);
    output << "(";
    actual_arg->pretty_print_at(output , prec_none, lastNewLine, letParens);
    output << ")";
}

//////////////////////////
//// Helper Functions ////
//////////////////////////
void printExpr(PTR(Expr) e){
    std::cout << e->to_string() << std::endl;
}
void printPrettyExpr(PTR(Expr) e){
    std::cout << e->pretty_to_string() << std::endl;
}
