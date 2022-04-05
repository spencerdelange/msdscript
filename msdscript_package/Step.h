//
// Created by Spencer DeLange on 3/30/22.
//

#ifndef MSDSCRIPT_STEP_H
#define MSDSCRIPT_STEP_H

// Class representing one step in interpreting

#include <string>
#include "pointer.h"

// Declared to be able to write definitions
class Val;
class Env;
class Expr;
class Cont;

CLASS(Step) {
public:
    // Enum whether step should interpret or continue
    typedef enum {
        interp_mode,
        continue_mode
    } mode_t;
    static PTR(Expr) expr;
    static PTR(Env) env;
    static PTR(Val) val;
    static PTR(Cont) cont;
    static mode_t mode;
    static PTR(Val) interp_by_steps(PTR(Expr) e);

};

#endif //MSDSCRIPT_STEP_H
