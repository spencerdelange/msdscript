//
// Created by Spencer DeLange on 2/13/22.
// Represents a value. Includes boolean values and number values
//

#ifndef MSDSCRIPT_VAL_H
#define MSDSCRIPT_VAL_H

#include <string>

// included so Expr* can be in the method definitions
class Expr;

// Class representing a value
class Val {
public:
    // Returns the Expr representation of a value
    virtual Expr* to_expr() = 0;
    // Returns whether this Val is equal to another Val, v
    virtual bool equals(Val* v) = 0;
    // Returns a Val representing this val plus another val
    virtual Val* add_to(Val* other_val) = 0;
    // Returns a Val representing this val multiplied with another val
    virtual Val* mult_to(Val* other_val) = 0;
    // Returns whether this is true
    virtual bool is_true() = 0;
    // Returns the value of a function being called by the given argument value
    virtual Val *call(Val *actual_arg) = 0;
    // Returns a string representing this value
    std::string to_string();
};

// Value subclass representing a number
class NumVal : public Val {
public:
    int rep;
    // Constructs a NumVal object with a value of given rep
    explicit NumVal(int rep);
    // Returns an Expr representing this value
    Expr *to_expr() override;
    // Returns whether this is equal to given Val v
    bool equals(Val *v) override;
    // Returns a Val representing this val plus another val, v
    Val *add_to(Val *other_val) override;
    // Returns a Val representing this val times another val, v
    Val *mult_to(Val *other_val) override;
    // Returns false, a number is never true
    bool is_true() override;
    // Returns the value of a function being called by the given argument value
    Val *call(Val *actual_arg) override;
};

// Value subclass representing a boolean
class BoolVal : public Val {
public:
    bool rep;
    // Constructs a BoolVal object with a value of given rep
    explicit BoolVal(bool rep);
    // Returns an Expr representing this value
    Expr *to_expr() override;
    // Returns whether this is equal to given Val v
    bool equals(Val *v) override;
    // Returns a Val representing this val plus another val, v
    Val *add_to(Val *other_val) override;
    // Returns a Val representing this val times another val, v
    Val *mult_to(Val *other_val) override;
    // Returns whether ths is true or false
    bool is_true() override;
    // Returns the value of a function being called by the given argument value
    Val *call(Val *actual_arg) override;
};

// Value subclass representing a function's value
class FunVal : public Val {
public:
    std::string formal_arg;
    Expr *body;
    // Constructs a FunVal object with a value of given rep
    explicit FunVal(std::string formal_arg, Expr* body);
    // Returns an Expr representing this value
    Expr *to_expr() override;
    // Returns whether this is equal to given Val v
    bool equals(Val *v) override;
    // Returns a Val representing this val plus another val, v
    Val *add_to(Val *other_val) override;
    // Returns a Val representing this val times another val, v
    Val *mult_to(Val *other_val) override;
    // Returns whether ths is true or false
    bool is_true() override;
    // Returns the value of a function being called by the given argument value
    Val *call(Val *actual_arg) override;
};

#endif //MSDSCRIPT_VAL_H
