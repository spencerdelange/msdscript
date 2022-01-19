//
// Created by Spencer DeLange on 1/14/22.
//

#ifndef MSDSCRIPT_EXPR_H
#define MSDSCRIPT_EXPR_H

#include <string>

// Class representing an expression
class Expr {
public:
    // Returns boolean whether this is equal to a given Expr object.
    virtual bool equals(Expr *e) = 0;
    // Returns an int for the value of an expression
    virtual int interp() = 0;
    // Returns whether an expression has a variable or is a variable
    virtual bool has_variable() = 0;
    // substitutes a variable in an expression with a given variable
    virtual Expr* subst(std::string to_replace, Expr* substitute_expr) = 0;
};

// Expr subclass representing a number in an expression
class Num : public Expr {
public:
    int val;
    // Constructs a Num object with a value of given val
    Num(int val);
    // Returns whether this is equal to a given Expr object
    bool equals(Expr *e) override;
    // Returns the value of this number
    int interp() override;
    // Returns whether expression has a variable
    bool has_variable() override;
    // substitutes a variable in an expression with a given variable
    Expr* subst(std::string to_replace, Expr* substitute_expr) override;
};

// Expr subclass representing a variable in an expression
class Var : public Expr {
public:
    std::string name;
    // Constructor
    Var(std::string name);
    // Returns whether this is equals to a given expression
    bool equals(Expr *e) override;
    // A variable has no value, so throws a runtime_error
    int interp() override;
    // Returns whether expression has a variable
    bool has_variable() override;
    // substitutes a variable in an expression with a given variable
    Expr* subst(std::string to_replace, Expr* substitute_expr) override;
};

// Expr subclass representing the addition of two expressions
class Add : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    // Constructor
    Add(Expr *lhs, Expr *rhs);
    // Returns whether this is equal to a given Expr
    bool equals(Expr *e) override;
    // Returns the value of this addition Expr
    int interp() override;
    // Returns whether expression has a variable
    bool has_variable() override;
    // substitutes a variable in an expression with a given variable
    Expr* subst(std::string to_replace, Expr* substitute_expr) override;
};

// Expr subclass representing the multiplication of two expressions
class Mult : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    // Constructor
    Mult(Expr *lhs, Expr *rhs);
    // Returns whether this is equal to a given Expr object
    bool equals(Expr *e) override;
    // Returns the value of this expression
    int interp() override;
    // Returns whether expression has a variable
    bool has_variable() override;
    // substitutes a variable in an expression with a given variable
    Expr* subst(std::string to_replace, Expr* substitute_expr) override;
};

#endif //MSDSCRIPT_EXPR_H