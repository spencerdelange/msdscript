//
// Created by Spencer DeLange on 2/13/22.
// Represents a value. Includes boolean values and number values
//

#ifndef MSDSCRIPT_VAL_H
#define MSDSCRIPT_VAL_H

#include <string>
#include "pointer.h"

// included so Expr and Env can be in the method definitions
class Expr;
class Env;
class Cont;

// Class representing a value
CLASS(Val) {
public:
    // Returns whether this Val is equal to another Val, v
    virtual bool equals(PTR(Val) v) = 0;
    // Returns a Val representing this val plus another val
    virtual PTR(Val) add_to(PTR(Val) other_val) = 0;
    // Returns a Val representing this val multiplied with another val
    virtual PTR(Val) mult_to(PTR(Val) other_val) = 0;
    // Returns whether this is true
    virtual bool is_true() = 0;
    // Returns the value of a function being called by the given argument value
    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;
    virtual void call_step(PTR(Val) actual_arg, PTR(Cont) rest) = 0;
    // Returns a string representing this value
    virtual std::string to_string() = 0;
    // Satisfies 'virtual destructors' warning
    virtual ~Val();
};

// Value subclass representing a number
class NumVal : public Val {
public:
    int rep;
    // Constructs a NumVal object with a value of given rep
    NumVal(int rep);
    // Returns an Expr representing this value
    bool equals(PTR(Val) v) override;
    // Returns a Val representing this val plus another val, v
    PTR(Val) add_to(PTR(Val) other_val) override;
    // Returns a Val representing this val times another val, v
    PTR(Val) mult_to(PTR(Val) other_val) override;
    // Returns false, a number is never true
    bool is_true() override;
    // Returns the value of a function being called by the given argument value
    PTR(Val) call(PTR(Val) actual_arg) override;
    void call_step(PTR(Val) actual_arg, PTR(Cont) rest) override;
    // Returns a string representing this value
    std::string to_string() override;
};

// Value subclass representing a boolean
class BoolVal : public Val {
public:
    bool rep;
    // Constructs a BoolVal object with a value of given rep
    explicit BoolVal(bool rep);
    // Returns an Expr representing this value
    bool equals(PTR(Val) v) override;
    // Returns a Val representing this val plus another val, v
    PTR(Val) add_to(PTR(Val) other_val) override;
    // Returns a Val representing this val times another val, v
    PTR(Val) mult_to(PTR(Val) other_val) override;
    // Returns whether ths is true or false
    bool is_true() override;
    // Returns the value of a function being called by the given argument value
    PTR(Val) call(PTR(Val) actual_arg) override;
    void call_step(PTR(Val) actual_arg, PTR(Cont) rest) override;
    // Returns a string representing this value
    std::string to_string() override;
};

// Value subclass representing a function's value
class FunVal : public Val {
public:
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;
    // Constructs a FunVal object with a value of given rep
    explicit FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env);
    // Returns an Expr representing this value
    bool equals(PTR(Val) v) override;
    // Returns a Val representing this val plus another val, v
    PTR(Val) add_to(PTR(Val) other_val) override;
    // Returns a Val representing this val times another val, v
    PTR(Val) mult_to(PTR(Val) other_val) override;
    // Returns whether ths is true or false
    bool is_true() override;
    // Returns the value of a function being called by the given argument value
    PTR(Val) call(PTR(Val) actual_arg) override;
    // Does one call_step
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) override;
    // Returns a string representing this value
    std::string to_string() override;
};

#endif //MSDSCRIPT_VAL_H
