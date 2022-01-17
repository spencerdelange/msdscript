//
// Created by Spencer DeLange on 1/16/22.
//

#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "test.h"
#include "expr.h"
#include "Var.h"
#include "Add.h"
#include "Mult.h"
#include "Num.h"

// displays the results of the test
int test(char* argv[]){
    return Catch::Session().run(1, argv);
}

TEST_CASE( "exprtests" ) {
    Num* n1 = new Num(1);
    Num* n2 = new Num(2);
    Num* n3 = new Num(3);
    Num* n4 = new Num(4);
    Add* a1 = new Add(n1, n2);
    Add* a2 = new Add(n3, n4);
    Mult* m = new Mult(n1, n2);
    CHECK(n1->equals(n2) == false);
    CHECK(a1->equals(a2) == false);
    CHECK(a1->equals(a1) == true);
    CHECK(a1->equals(m) == false);
    CHECK(n1->equals(a1) == false);
}