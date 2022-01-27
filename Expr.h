//
// Created by Spencer DeLange on 1/14/22.
//

#ifndef MSDSCRIPT_EXPR_H
#define MSDSCRIPT_EXPR_H

#include <string>
#include <iostream>

// Enum that represents precedence of a given Expr
typedef enum precedence_t {
    prec_none,      // = 0
    prec_add,       // = 1
    prec_mult       // = 2
} precedence_t;

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
    // Prints this Expr's string representation to a given std::ostream&
    virtual void print(std::ostream& output) = 0;
    // Prints this Expr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    virtual void pretty_print(std::ostream& output) = 0;
    // Prints this Expr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    virtual void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) = 0;
    // Returns a string containing the results of print
    std::string to_string();
    // Returns a string containing the results of print
    std::string pretty_to_string();
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
    // Print's this Num's value to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this Num's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this Num to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
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
    // Print's this Var's name to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this Var's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this Var to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
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
    // Print's this Add's string representation to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this Add's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this Add to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
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
    // Print's this Mult's string representation to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this Mult's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this Mult to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};

// Expr subclass representing a variable assigned a value in a given expression
class Let : public Expr {
public:
    Var *lhs;
    Expr *rhs;
    Expr *body;
    // Constructor
    Let(Var* lhs, Expr *rhs, Expr *body);
    // Returns whether this is equal to a given Expr
    bool equals(Expr *e) override;
    // Returns the value of this let Expr
    int interp() override;
    // Returns whether expression has a variable
    bool has_variable() override;
    // substitutes a variable in an expression with a given variable
    Expr* subst(std::string to_replace, Expr* substitute_expr) override;
    // Print's this Let's string representation to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this Let's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this Let to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};
// Helper function that prints the given expression's to_string to std::cout
void printExpr(Expr* e);
// Helper function that prints the given expression's pretty_to_string to std::cout
void printPrettyExpr(Expr* e);

#endif //MSDSCRIPT_EXPR_H