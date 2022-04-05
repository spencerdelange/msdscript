//
// Created by Spencer DeLange on 3/30/22.
//

#include "Step.h"
#include "Env.h"
#include "Cont.h"
#include "Val.h"
#include "Expr.h"

Step::mode_t Step::mode;
PTR(Expr) Step::expr;
PTR(Env) Step::env;
PTR(Val) Step::val;
PTR(Cont) Step::cont;

PTR(Val) Step::interp_by_steps(PTR(Expr) e) {
    Step::mode = Step::interp_mode;
    Step::expr = e;
    Step::env = Env::empty;
    Step::val = nullptr;
    Step::cont = Cont::done;
    while (1) {
        if (Step::mode == Step::interp_mode)
            Step::expr->step_interp();
        else {
            if (Step::cont == Cont::done)
                return Step::val;
            else
                Step::cont->step_continue();
        }
    }
}