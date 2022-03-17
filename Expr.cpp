//
// Created by Spencer DeLange on 1/16/22.
//

#include <sstream>
#include "Expr.h"
#include "Val.h"

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
PTR(Val) NumExpr::interp() {
    return NEW(NumVal)(rep);
}

PTR(Expr) NumExpr::subst(std::string to_replace, PTR(Expr) substitute_expr) {
    return THIS;
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
PTR(Val) BoolExpr::interp() {
    return NEW(BoolVal)(rep);
}

PTR(Expr) BoolExpr::subst(std::string to_replace, PTR(Expr) substitute_expr) {
    return THIS;
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
PTR(Val) VarExpr::interp() {
    throw std::runtime_error("Error: a variable has no value: " + this->name);
}

PTR(Expr) VarExpr::subst(std::string to_replace, PTR(Expr) substitute_expr) {
    if(this->name == to_replace){
        return substitute_expr;
    } else{
        return THIS;
    }
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
PTR(Val) EqExpr::interp() {
    return NEW(BoolVal)(lhs->interp()->equals(rhs->interp()));
}

PTR(Expr) EqExpr::subst(std::string to_replace, PTR(Expr) substitute_expr) {
    PTR(EqExpr) temp = NEW(EqExpr)(lhs->subst(to_replace, substitute_expr), rhs->subst(to_replace, substitute_expr));
    return temp;
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
PTR(Val) AddExpr::interp() {
    return lhs->interp()->add_to(rhs->interp());
}

PTR(Expr) AddExpr::subst(std::string to_replace, PTR(Expr) substitute_expr) {
    PTR(AddExpr) temp = NEW(AddExpr)(lhs->subst(to_replace, substitute_expr), rhs->subst(to_replace, substitute_expr));
    return temp;
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
PTR(Val) MultExpr::interp() {
    return lhs->interp()->mult_to(rhs->interp());
}

PTR(Expr) MultExpr::subst(std::string to_replace, PTR(Expr) substitute_expr) {
    return NEW(MultExpr)(lhs->subst(to_replace, substitute_expr), rhs->subst(to_replace, substitute_expr));
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
PTR(Val) IfExpr::interp() {
    if(_if->interp()->is_true())
        return _then->interp();
    else
        return _else->interp();
}

PTR(Expr) IfExpr::subst(std::string to_replace, PTR(Expr) substitute_expr) {
    PTR(Expr) i = _if->subst(to_replace, substitute_expr);
    PTR(Expr) t = _then->subst(to_replace, substitute_expr);
    PTR(Expr) e = _else->subst(to_replace, substitute_expr);
    PTR(IfExpr) temp = NEW(IfExpr)(i, t, e);
    return temp;
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
PTR(Val) LetExpr::interp() {
    PTR(Val) rhs_val = rhs->interp();
    PTR(Expr) rhs_expr = rhs_val->to_expr();
    PTR(Expr) e = body->subst(lhs->name, rhs_expr);
    return e->interp();
}

PTR(Expr) LetExpr::subst(std::string to_replace, PTR(Expr) substitute_expr) {
    // if the name of the var to replace is the same as the current lhs of this LetExpr, only subst on the rhs, as the body var is bound. Else, subst on the rhs and the body.
    if(this->lhs->name == to_replace){
        return NEW(LetExpr)(this->lhs, rhs->subst(to_replace, substitute_expr), this->body);
    } else {
        return NEW(LetExpr)(this->lhs, this->rhs->subst(to_replace, substitute_expr), this->body->subst(to_replace, substitute_expr));
    }
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
PTR(Val) FunExpr::interp() {
    return NEW(FunVal)(formal_arg->name, body);
}
PTR(Expr) FunExpr::subst(std::string to_replace, PTR(Expr) substitute_expr) {
    // if the name of the var to replace is the same as the current formal_arg of this FunExpr, don't perform subst. Else, return FunExpr after body->subst
    if(this->formal_arg->name == to_replace)
        return THIS;
    return NEW(FunExpr)(formal_arg, body->subst(to_replace, substitute_expr));
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
PTR(Val) CallExpr::interp() {
    PTR(Val) v1 = to_be_called->interp();
    PTR(Val) v2 = actual_arg->interp();
    PTR(Val) v3 = v1->call(v2);
    return v3;
}
PTR(Expr) CallExpr::subst(std::string to_replace, PTR(Expr) substitute_expr) {
    // if the name of the var to replace is the same as the current lhs of this CallExpr, only subst on the rhs, as the body var is bound. Else, subst on the rhs and the body.

    PTR(Expr) e = actual_arg->subst(to_replace, substitute_expr);

    return NEW(CallExpr)(to_be_called->subst(to_replace, substitute_expr), e);
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
