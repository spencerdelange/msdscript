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

// Included so Val* can be in the method definitions
class Val;

// Class representing an expression
class Expr {
public:
    // Returns boolean whether this is equal to a given Expr object.
    virtual bool equals(Expr *e) = 0;
    // Returns an int for the value of an expression
    virtual Val * interp() = 0;
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
class NumExpr : public Expr {
public:
    int rep;
    // Constructs a NumExpr object with a value of given rep
    explicit NumExpr(int rep);
    // Returns whether this is equal to a given Expr object
    bool equals(Expr *e) override;
    // Returns the value of this number
    Val * interp() override;
    // Returns whether expression has a variable
    bool has_variable() override;
    // substitutes a variable in an expression with a given variable
    Expr* subst(std::string to_replace, Expr* substitute_expr) override;
    // Print's this NumExpr's value to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this NumExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this NumExpr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};

// Expr subclass representing a boolean in an expression
class BoolExpr : public Expr {
public:
    bool rep;
    // Constructs a BoolExpr object with a value of given rep
    explicit BoolExpr(bool rep);
    // Returns whether this is equal to a given Expr object
    bool equals(Expr *e) override;
    // Returns the value of this number
    Val * interp() override;
    // Returns whether expression has a variable
    bool has_variable() override;
    // substitutes a variable in an expression with a given variable
    Expr* subst(std::string to_replace, Expr* substitute_expr) override;
    // Print's this BoolExpr's value to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this BoolExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this BoolExpr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};

// Expr subclass representing a variable in an expression
class VarExpr : public Expr {
public:
    std::string name;
    // Constructor
    explicit VarExpr(std::string name);
    // Returns whether this is equals to a given expression
    bool equals(Expr *e) override;
    // A variable has no value, so throws a runtime_error
    Val * interp() override;
    // Returns whether expression has a variable
    bool has_variable() override;
    // substitutes a variable in an expression with a given variable
    Expr* subst(std::string to_replace, Expr* substitute_expr) override;
    // Print's this VarExpr's name to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this VarExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this VarExpr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};

// Expr subclass representing an equals '==' expression
class EqExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    // Constructor
    EqExpr(Expr *lhs, Expr *rhs);
    // Returns whether this is equal to a given Expr
    bool equals(Expr *e) override;
    // Returns the value of this addition Expr
    Val * interp() override;
    // Returns whether expression has a variable
    bool has_variable() override;
    // substitutes a variable in an expression with a given variable
    Expr* subst(std::string to_replace, Expr* substitute_expr) override;
    // Print's this EqExpr's string representation to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this EqExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this EqExpr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};

// Expr subclass representing the addition of two expressions
class AddExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    // Constructor
    AddExpr(Expr *lhs, Expr *rhs);
    // Returns whether this is equal to a given Expr
    bool equals(Expr *e) override;
    // Returns the value of this addition Expr
    Val * interp() override;
    // Returns whether expression has a variable
    bool has_variable() override;
    // substitutes a variable in an expression with a given variable
    Expr* subst(std::string to_replace, Expr* substitute_expr) override;
    // Print's this AddExpr's string representation to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this AddExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this AddExpr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};

// Expr subclass representing the multiplication of two expressions
class MultExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;
    // Constructor
    MultExpr(Expr *lhs, Expr *rhs);
    // Returns whether this is equal to a given Expr object
    bool equals(Expr *e) override;
    // Returns the value of this expression
    Val * interp() override;
    // Returns whether expression has a variable
    bool has_variable() override;
    // substitutes a variable in an expression with a given variable
    Expr* subst(std::string to_replace, Expr* substitute_expr) override;
    // Print's this MultExpr's string representation to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this MultExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this MultExpr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};

// Expr subclass representing a variable assigned a value in a given expression
class LetExpr : public Expr {
public:
    VarExpr *lhs;
    Expr *rhs;
    Expr *body;
    // Constructor
    LetExpr(VarExpr* lhs, Expr *rhs, Expr *body);
    // Returns whether this is equal to a given Expr
    bool equals(Expr *e) override;
    // Returns the value of this let Expr
    Val * interp() override;
    // Returns whether expression has a variable
    bool has_variable() override;
    // substitutes a variable in an expression with a given variable
    Expr* subst(std::string to_replace, Expr* substitute_expr) override;
    // Print's this LetExpr's string representation to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this LetExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this LetExpr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};

// Expr subclass representing an 'if, then, else' expression
class IfExpr : public Expr {
public:
    Expr *_if;
    Expr *_then;
    Expr *_else;
    // Constructor
    IfExpr(Expr* _if, Expr* _then, Expr* _else);
    // Returns whether this is equal to a given Expr
    bool equals(Expr *e) override;
    // Returns the value of this let Expr
    Val * interp() override;
    // Returns whether expression has a variable
    bool has_variable() override;
    // substitutes a variable in an expression with a given variable
    Expr* subst(std::string to_replace, Expr* substitute_expr) override;
    // Print's this IfExpr's string representation to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this IfExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this IfExpr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given. True means _let doesn't need parentheses
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};



// Helper function that prints the given expression's to_string to std::cout
void printExpr(Expr* e);
// Helper function that prints the given expression's pretty_to_string to std::cout
void printPrettyExpr(Expr* e);

#endif //MSDSCRIPT_EXPR_H