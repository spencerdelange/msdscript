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
    // Returns a string representing this value
    std::string to_string();
};

// Value subclass representing a number in an expression
class NumVal : public Val {
public:
    int rep;
    // Constructs a NumVal object with a value of given rep
    NumVal(int rep);
    // Returns an Expr representing this valuew
    Expr *to_expr() override;
    // Returns whether this is equal to given Val v
    bool equals(Val *v) override;
    // Returns a Val representing this val plus another val, v
    Val *add_to(Val *other_val) override;
    // Returns a Val representing this val times another val, v
    Val *mult_to(Val *other_val) override;
};

#endif //MSDSCRIPT_VAL_H
