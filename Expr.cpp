//
// Created by Spencer DeLange on 1/16/22.
//

#include <sstream>
#include "Expr.h"
#include "Val.h"

// Expr implementations
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

// NumExpr implementations
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
bool NumExpr::has_variable() {
    return false;
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

// VarExpr implementations
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
    throw std::runtime_error("Error: a variable has no value.");
}
bool VarExpr::has_variable() {
    return true;
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

// AddExpr implementations
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
bool AddExpr::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
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

// MultExpr implementations
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
bool MultExpr::has_variable() {
    return lhs->has_variable() || rhs->has_variable();
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

// LetExpr implementations
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
    return body->subst(lhs->name, rhs_val->to_expr())->interp();
}
bool LetExpr::has_variable() {
    return rhs->has_variable() || body->has_variable();
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
// Helper functions
void printExpr(Expr* e){
    std::cout << e->to_string() << std::endl;
}
void printPrettyExpr(Expr* e){
    std::cout << e->pretty_to_string() << std::endl;
}
