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
    output << this->val;
}
precedence_t Num::pretty_print_at() {
    return prec_none;
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
    output << this->name;
}
precedence_t Var::pretty_print_at() {
    return prec_none;
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
    if(lhs->pretty_print_at() == prec_add){
        output << "(";
        this->lhs->pretty_print(output);
        output << ")";
    } else {
        this->lhs->pretty_print(output);
    }
    output << " + ";
    if(lhs->pretty_print_at() == prec_mult && rhs->pretty_print_at() == prec_add){
        output << "(";
        this->rhs->pretty_print(output);
        output << ")";
    } else {
        this->rhs->pretty_print(output);
    }
}
// if pretty_print_at returns
precedence_t Add::pretty_print_at() {
    return prec_add;
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
    if(lhs->pretty_print_at() >= prec_add){
        output << "(";
        this->lhs->pretty_print(output);
        output << ")";
    } else {
        this->lhs->pretty_print(output);
    }
    output << " * ";
    if(this->rhs->pretty_print_at() == prec_add){
        output << "(";
        this->rhs->pretty_print(output);
        output << ")";
    } else{
        this->rhs->pretty_print(output);
    }
}
precedence_t Mult::pretty_print_at() {
    return prec_mult;
}

