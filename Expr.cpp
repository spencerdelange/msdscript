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

/////////////////////////////////
//// NumExpr implementations ////
/////////////////////////////////
NumExpr::NumExpr(int rep) {
    this->rep = rep;
}
bool NumExpr::equals(Expr *e) {
    NumExpr *n = dynamic_cast<NumExpr*>(e);
    if (n == nullptr)
        return false;
    else
        return (this->rep == n->rep);
}
Val * NumExpr::interp() {
    return new NumVal(rep);
}

Expr *NumExpr::subst(std::string to_replace, Expr *substitute_expr) {
    return this;
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
bool BoolExpr::equals(Expr *e){
    BoolExpr *v = dynamic_cast<BoolExpr*>(e);
    if (v == nullptr)
        return false;
    else
        return (this->rep == v->rep);
}
Val * BoolExpr::interp() {
    return new BoolVal(rep);
}

Expr *BoolExpr::subst(std::string to_replace, Expr* substitute_expr) {
    return this;
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
bool VarExpr::equals(Expr *e){
    VarExpr *v = dynamic_cast<VarExpr*>(e);
    if (v == nullptr)
        return false;
    else
        return (this->name == v->name);
}
Val * VarExpr::interp() {
    throw std::runtime_error("Error: a variable has no value: " + this->name);
}

Expr *VarExpr::subst(std::string to_replace, Expr* substitute_expr) {
    if(this->name == to_replace){
        return substitute_expr;
    } else{
        return this;
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
EqExpr::EqExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool EqExpr::equals(Expr *e) {
    EqExpr*a = dynamic_cast<EqExpr*>(e);
    if (a == nullptr)
        return false;
    else
        return (this->lhs->equals(a->lhs)
                && this->rhs->equals(a->rhs));
}
Val * EqExpr::interp() {
    return new BoolVal(lhs->equals(rhs));
}

Expr *EqExpr::subst(std::string to_replace, Expr *substitute_expr) {
    EqExpr* temp = new EqExpr(lhs->subst(to_replace, substitute_expr), rhs->subst(to_replace, substitute_expr));
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
AddExpr::AddExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool AddExpr::equals(Expr *e) {
    AddExpr*a = dynamic_cast<AddExpr*>(e);
    if (a == nullptr)
        return false;
    else
        return (this->lhs->equals(a->lhs)
                && this->rhs->equals(a->rhs));
}
Val * AddExpr::interp() {
    return lhs->interp()->add_to(rhs->interp());
}

Expr *AddExpr::subst(std::string to_replace, Expr *substitute_expr) {
    AddExpr* temp = new AddExpr(lhs->subst(to_replace, substitute_expr), rhs->subst(to_replace, substitute_expr));
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
MultExpr::MultExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool MultExpr::equals(Expr *e) {
    MultExpr* m = dynamic_cast<MultExpr*>(e);
    if (m == nullptr)
        return false;
    else
        return (this->lhs->equals(m->lhs)
                && this->rhs->equals(m->rhs));
}
Val * MultExpr::interp() {
    return lhs->interp()->mult_to(rhs->interp());
}

Expr *MultExpr::subst(std::string to_replace, Expr* substitute_expr) {
    return new MultExpr(lhs->subst(to_replace, substitute_expr), rhs->subst(to_replace, substitute_expr));
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
IfExpr::IfExpr(Expr* _if, Expr* _then, Expr* _else) {
    this->_if = _if;
    this->_then = _then;
    this->_else = _else;
}
bool IfExpr::equals(Expr *e) {
    IfExpr* l = dynamic_cast<IfExpr*>(e);
    if (l == nullptr)
        return false;
    else
        return (this->_if->equals(l->_if)
                && this->_then->equals(l->_then) && this->_else->equals(l->_else));
}
Val * IfExpr::interp() {
    if(_if->interp()->is_true())
        return _then->interp();
    else
        return _else->interp();
}

Expr *IfExpr::subst(std::string to_replace, Expr *substitute_expr) {
    IfExpr* temp = new IfExpr(_if->subst(to_replace, substitute_expr), _then->subst(to_replace, substitute_expr), _else->subst(to_replace, substitute_expr));
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
LetExpr::LetExpr(VarExpr *lhs, Expr *rhs, Expr *body) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}
bool LetExpr::equals(Expr *e) {
    LetExpr* l = dynamic_cast<LetExpr*>(e);
    if (l == nullptr)
        return false;
    else
        return (this->lhs->equals(l->lhs)
                && this->rhs->equals(l->rhs) && this->body->equals(l->body));
}
Val * LetExpr::interp() {
    Val *rhs_val = rhs->interp();
    Expr* e = body->subst(lhs->name, rhs_val->to_expr());
    return e->interp();
}

Expr *LetExpr::subst(std::string to_replace, Expr *substitute_expr) {
    // if the name of the var to replace is the same as the current lhs of this LetExpr, only subst on the rhs, as the body var is bound. Else, subst on the rhs and the body.
    if(this->lhs->name == to_replace){
        return new LetExpr(this->lhs, rhs->subst(to_replace, substitute_expr), this->body);
    } else {
        return new LetExpr(this->lhs, this->rhs->subst(to_replace, substitute_expr), this->body->subst(to_replace, substitute_expr));
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
FunExpr::FunExpr(VarExpr* formal_arg, Expr* body) {
    this->formal_arg = formal_arg;
    this->body = body;
}
bool FunExpr::equals(Expr *e) {
    FunExpr* l = dynamic_cast<FunExpr*>(e);
    if (l == nullptr)
        return false;
    else
        return this->formal_arg == l->formal_arg && this->body->equals(l->body);
}
Val * FunExpr::interp() {
    return new FunVal(formal_arg->name, body);
}
Expr *FunExpr::subst(std::string to_replace, Expr *substitute_expr) {
    // if the name of the var to replace is the same as the current formal_arg of this FunExpr, don't perform subst. Else, return FunExpr after body->subst
    if(this->formal_arg->name == to_replace)
        return this;
    return new FunExpr(formal_arg, body->subst(to_replace, substitute_expr));
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
CallExpr::CallExpr(Expr* to_be_called, Expr* actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}
bool CallExpr::equals(Expr *e) {
    CallExpr* l = dynamic_cast<CallExpr*>(e);
    if (l == nullptr)
        return false;
    else
        return this->to_be_called->equals(l->to_be_called) && this->actual_arg->equals(l->actual_arg);
}
Val * CallExpr::interp() {
    Val* v1 = to_be_called->interp();
    Val* v2 = actual_arg->interp();
    Val* v3 = v1->call(v2);
    return v3;
}
Expr *CallExpr::subst(std::string to_replace, Expr *substitute_expr) {
    // if the name of the var to replace is the same as the current lhs of this CallExpr, only subst on the rhs, as the body var is bound. Else, subst on the rhs and the body.

    Expr* e = actual_arg->subst(to_replace, substitute_expr);

    return new CallExpr(to_be_called->subst(to_replace, substitute_expr), e);
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
void printExpr(Expr* e){
    std::cout << e->to_string() << std::endl;
}
void printPrettyExpr(Expr* e){
    std::cout << e->pretty_to_string() << std::endl;
}
