//
// Created by Spencer DeLange on 1/14/22.
//

#ifndef MSDSCRIPT_EXPR_H
#define MSDSCRIPT_EXPR_H

#include <string>
#include <iostream>
#include "pointer.h"

// Enum that represents precedence of a given Expr
typedef enum precedence_t {
    prec_none,      // = 0
    prec_add,       // = 1
    prec_mult       // = 2
} precedence_t;

// Included so Val and Env can be in the method definitions
class Val;
class Env;

// Class representing an expression
CLASS(Expr) {
public:
    // Returns boolean whether this is equal to a given Expr object.
    virtual bool equals(PTR(Expr) e) = 0;
    // Returns an int for the value of an expression
    virtual PTR(Val) interp(PTR(Env) env) = 0;
    // substitutes a variable in an expression with a given variable
    virtual void print(std::ostream& output) = 0;
    // Prints this Expr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    virtual void pretty_print(std::ostream& output) = 0;
    // Prints this Expr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    virtual void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) = 0;
    // Returns a string containing the results of print
    std::string to_string();
    // Returns a string containing the results of print
    std::string pretty_to_string();
    // Satisfies 'virtual destructors' warning
    virtual ~Expr();
};

// Expr subclass representing a number in an expression
class NumExpr : public Expr {
public:
    int rep;
    // Constructs a NumExpr object with a value of given rep
    explicit NumExpr(int rep);
    // Returns whether this is equal to a given Expr object
    bool equals(PTR(Expr) e) override;
    // Returns the value of this number
    PTR(Val) interp(PTR(Env) env) override;
    // substitutes a variable in an expression with a given variable
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
    bool equals(PTR(Expr) e) override;
    // Returns the value of this number
    PTR(Val) interp(PTR(Env) env) override;
    // substitutes a variable in an expression with a given variable
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
    bool equals(PTR(Expr) e) override;
    // A variable has no value, so throws a runtime_error
    PTR(Val) interp(PTR(Env) env) override;
    // substitutes a variable in an expression with a given variable
    void print(std::ostream& output) override;
    // Prints this VarExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this VarExpr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};

// Expr subclass representing an equals '==' expression
class EqExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    // Constructor
    EqExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    // Returns whether this is equal to a given Expr
    bool equals(PTR(Expr) e) override;
    // Returns the value of this addition Expr
    PTR(Val) interp(PTR(Env) env) override;
    // substitutes a variable in an expression with a given variable
    void print(std::ostream& output) override;
    // Prints this EqExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this EqExpr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};

// Expr subclass representing the addition of two expressions
class AddExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    // Constructor
    AddExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    // Returns whether this is equal to a given Expr
    bool equals(PTR(Expr) e) override;
    // Returns the value of this addition Expr
    PTR(Val) interp(PTR(Env) env) override;
    // substitutes a variable in an expression with a given variable
    void print(std::ostream& output) override;
    // Prints this AddExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this AddExpr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};

// Expr subclass representing the multiplication of two expressions
class MultExpr : public Expr {
public:
    PTR(Expr) lhs;
    PTR(Expr) rhs;
    // Constructor
    MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);
    // Returns whether this is equal to a given Expr object
    bool equals(PTR(Expr) e) override;
    // Returns the value of this expression
    PTR(Val) interp(PTR(Env) env) override;
    // substitutes a variable in an expression with a given variable
    void print(std::ostream& output) override;
    // Prints this MultExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this MultExpr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};

// Expr subclass representing a variable assigned a value in a given expression
class LetExpr : public Expr {
public:
    PTR(VarExpr) lhs;
    PTR(Expr) rhs;
    PTR(Expr) body;
    // Constructor
    LetExpr(PTR(VarExpr) lhs, PTR(Expr) rhs, PTR(Expr) body);
    // Returns whether this is equal to a given Expr
    bool equals(PTR(Expr) e) override;
    // Returns the value of this let Expr
    PTR(Val) interp(PTR(Env) env) override;
    // substitutes a variable in an expression with a given variable
    void print(std::ostream& output) override;
    // Prints this LetExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this LetExpr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _let needs parentheses is given.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};

// Expr subclass representing an 'if, then, else' expression
class IfExpr : public Expr {
public:
    PTR(Expr) _if;
    PTR(Expr) _then;
    PTR(Expr) _else;
    // Constructor
    IfExpr(PTR(Expr) _if, PTR(Expr) _then, PTR(Expr) _else);
    // Returns whether this is equal to a given Expr
    bool equals(PTR(Expr) e) override;
    // Returns the value of this let Expr
    PTR(Val) interp(PTR(Env) env) override;
    // Print's this IfExpr's string representation to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this IfExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this IfExpr to output.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};
// Expr subclass representing a function as an expression
class FunExpr : public Expr {
public:
    PTR(VarExpr) formal_arg;
    PTR(Expr) body;
    // Constructor
    FunExpr(PTR(VarExpr) formal_arg, PTR(Expr) body);
    // Returns whether this is equal to a given Expr
    bool equals(PTR(Expr) e) override;
    // Returns the value of this let Expr
    PTR(Val) interp(PTR(Env) env) override;
    // Print's this FunExpr's string representation to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this FunExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this FunExpr to output.
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};
// Expr subclass representing a function being called as an expression
class CallExpr : public Expr {
public:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;
    // Constructor
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);
    // Returns whether this is equal to a given Expr
    bool equals(PTR(Expr) e) override;
    // Returns the value of this let Expr
    PTR(Val) interp(PTR(Env) env) override;
    // Print's this CallExpr's string representation to a given std::ostream&
    void print(std::ostream& output) override;
    // Prints this CallExpr's string representation to a given std::ostream& without unnecessary parentheses. Signs are associated to the right.
    void pretty_print(std::ostream& output) override;
    // Prints this CallExpr to output. Preceding expression, the integer place of the last newline is given and a boolean of whether _fun needs parentheses is given. True means _fun doesn't need parentheses
    void pretty_print_at(std::ostream &output, precedence_t prec, int lastNewLine, bool letParens) override;
};

// Helper function that prints the given expression's to_string to std::cout
void printExpr(PTR(Expr) e);
// Helper function that prints the given expression's pretty_to_string to std::cout
void printPrettyExpr(PTR(Expr) e);

#endif //MSDSCRIPT_EXPR_H