//
// Created by Spencer DeLange on 3/30/22.
//

// Represents on step in interpretation of an expression

#ifndef MSDSCRIPT_CONT_H
#define MSDSCRIPT_CONT_H

#include <string>
#include "pointer.h"

// Included to be able to write definitions
class Val;
class Env;
class Expr;
class FunVal;

// Base class for a continuation
CLASS(Cont) {
public:
    // Continue stepping
    virtual void step_continue() = 0;
    // Done with stepping
    static PTR(Cont) done;
};

// Class representing the completion of steps
class DoneCont : public Cont {
public:
    DoneCont();
    void step_continue() override;
};

// Class representing a continuation step that must interp the right side the add
class RightThenAddCont : public Cont {
public:
    RightThenAddCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;
    void step_continue() override;
};
// Class representing a continuation step that must interp the right side the multiply
class RightThenMultCont : public Cont {
public:
    RightThenMultCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;
    void step_continue() override;
};
// Class representing a continuation step that must interp the right side the equals
class RightThenEqCont : public Cont {
public:
    RightThenEqCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;
    void step_continue() override;
};
// Add and continue
class AddCont : public Cont {
public:
    AddCont(PTR(Val) lhs_val, PTR(Cont) rest);
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    void step_continue() override;
};
// Multiply and continue
class MultCont : public Cont {
public:
    MultCont(PTR(Val) lhs_val, PTR(Cont) rest);
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    void step_continue() override;
};
// Evaluate equals and continue
class EqCont : public Cont {
public:
    EqCont(PTR(Val) lhs_val, PTR(Cont) rest);
    PTR(Val) lhs_val;
    PTR(Cont) rest;
    void step_continue() override;
};
// Evaluate if and continue
class IfBranchCont : public Cont {
public:
    IfBranchCont(PTR(Expr) then_part, PTR(Expr) else_part, PTR(Env) env, PTR(Cont) rest);
    PTR(Expr) then_part;
    PTR(Expr) else_part;
    PTR(Env) env;
    PTR(Cont) rest;
    void step_continue() override;
};
// Evaluate let body and continue
class LetBodyCont : public Cont {
public:
    LetBodyCont(std::string var, PTR(Expr) body, PTR(Env) env, PTR(Cont) rest);
    std::string var;
    PTR(Expr) body;
    PTR(Env) env;
    PTR(Cont) rest;
    void step_continue() override;
};
// Evaluate argument and continue
class ArgThenCallCont : public Cont {
public:
    ArgThenCallCont(PTR(Expr) actualArg, PTR(Env) env, PTR(Cont) rest);
    PTR(Expr) actual_arg;
    PTR(Env) env;
    PTR(Cont) rest;
    void step_continue() override;
};
// Continue after calling
class CallCont : public Cont {
public:
    CallCont(PTR(Val) toBeCalledVal, PTR(Cont) rest);
    PTR(Val) to_be_called_val;
    PTR(Cont) rest;
    void step_continue() override;
};


#endif //MSDSCRIPT_CONT_H
