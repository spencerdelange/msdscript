//
// Created by Spencer DeLange on 1/16/22.
//

#include <sstream>
#include "Expr.h"

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

// Num implementations
Num::Num(int val) {
    this->val = val;
}
bool Num::equals(Expr *e) {
    Num *n = dynamic_cast<Num*>(e);
    if (n == nullptr)
        return false;
    else
        return (this->val == n->val);
}
int Num::interp() {
    return val;
}
bool Num::has_variable() {
    return false;
}
Expr *Num::subst(std::string to_replace, Expr *substitute_expr) {
    return this;
}
void Num::print(std::ostream& output) {
    output << this->val;
}
void Num::pretty_print(std::ostream &output) {
    pretty_print_at(output, prec_none, 0, false);
}
void Num::pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) {
    output << this->val;
}

// Var implementations
Var::Var(std::string name){
    this->name = name;
}
bool Var::equals(Expr *e){
    Var *v = dynamic_cast<Var*>(e);
    if (v == nullptr)
        return false;
    else
        return (this->name == v->name);
}
int Var::interp() {
    throw std::runtime_error("Error: a variable has no value.");
}
bool Var::has_variable() {
    return true;
}
Expr *Var::subst(std::string to_replace, Expr* substitute_expr) {
    if(this->name == to_replace){
        return substitute_expr;
    } else{
        return this;
    }
}
void Var::print(std::ostream &output) {
    output << this->name;
}
void Var::pretty_print(std::ostream &output) {
    pretty_print_at(output, prec_none, 0, false);
}
void Var::pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) {
    output << this->name;
}

// Add implementations
Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool Add::equals(Expr *e) {
    Add*a = dynamic_cast<Add*>(e);
    if (a == nullptr)
        return false;
    else
        return (this->lhs->equals(a->lhs)
                && this->rhs->equals(a->rhs));
}
int Add::interp() {
    return this->lhs->interp() + this->rhs->interp();
}
bool Add::has_variable() {
    return this->lhs->has_variable() || this->rhs->has_variable();
}
Expr *Add::subst(std::string to_replace, Expr *substitute_expr) {
    Add* temp = new Add(lhs->subst(to_replace, substitute_expr), rhs->subst(to_replace, substitute_expr));
    return temp;
}
void Add::print(std::ostream &output) {
    output << "(";
    this->lhs->print(output);
    output << "+";
    this->rhs->print(output);
    output << ")";
}
void Add::pretty_print(std::ostream &output) {
    lhs->pretty_print_at(output, prec_add, 0, false);
    output << " + ";
    rhs->pretty_print_at(output, prec_none, 0, true);
}
void Add::pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) {
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

// Mult implementations
Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}
bool Mult::equals(Expr *e) {
    Mult* m = dynamic_cast<Mult*>(e);
    if (m == nullptr)
        return false;
    else
        return (this->lhs->equals(m->lhs)
                && this->rhs->equals(m->rhs));
}
int Mult::interp() {
    return lhs->interp() * rhs->interp();
}
bool Mult::has_variable() {
    return lhs->has_variable() || rhs->has_variable();
}
Expr *Mult::subst(std::string to_replace, Expr* substitute_expr) {
    return new Mult(lhs->subst(to_replace, substitute_expr), rhs->subst(to_replace, substitute_expr));
}
void Mult::print(std::ostream &output) {
    output << "(";
    this->lhs->print(output);
    output << "*";
    this->rhs->print(output);
    output << ")";
}
void Mult::pretty_print(std::ostream &output) {
    lhs->pretty_print_at(output, prec_mult, 0, false);
    output << " * ";
    rhs->pretty_print_at(output, prec_add, 0, true);
}
void Mult::pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) {
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

// Let implementations
Let::Let(Var *lhs, Expr *rhs, Expr *body) {
    this->lhs = lhs;
    this->rhs = rhs;
    this->body = body;
}
bool Let::equals(Expr *e) {
    Let* l = dynamic_cast<Let*>(e);
    if (l == nullptr)
        return false;
    else
        return (this->lhs->equals(l->lhs)
                && this->rhs->equals(l->rhs) && this->body->equals(l->body));
}
int Let::interp() {
    Expr* temp = body->subst(lhs->name, rhs);
    return temp->interp();
}
bool Let::has_variable() {
    return rhs->has_variable() || body->has_variable();
}
Expr *Let::subst(std::string to_replace, Expr *substitute_expr) {
    // if the name of the var to replace is the same as the current lhs of this Let, only subst on the rhs, as the body var is bound. Else, subst on the rhs and the body.
    if(this->lhs->name == to_replace){
        return new Let(this->lhs, rhs->subst(to_replace, substitute_expr), this->body);
    } else {
        return new Let(this->lhs, this->rhs->subst(to_replace, substitute_expr), this->body->subst(to_replace, substitute_expr));
    }
}
void Let::print(std::ostream &output) {
    output << "(_let " << this->lhs->name << "=" << this->rhs->to_string() << " _in " << this->body->to_string() << ")";
}
void Let::pretty_print(std::ostream &output) {
    pretty_print_at(output, prec_none, 0, true);
}
void Let::pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) {
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
