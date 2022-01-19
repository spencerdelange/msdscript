//
// Created by Spencer DeLange on 1/16/22.
//

#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "test.h"
#include "Expr.h"

int test(char* argv[]){
    return Catch::Session().run(1, argv);
}
TEST_CASE("equalstests") {
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
TEST_CASE("interp tests") {
    Num* n2 = new Num(2);
    Num* n3 = new Num(3);
    Add* a1 = new Add(n2, n3);
    Add* a2 = new Add(n2, n3);
    Mult* m1 = new Mult(n2, n3);
    Mult* m2 = new Mult(a2, m1);
    CHECK(n2->interp() == 2);
    CHECK(a1->interp() == 5);
    CHECK(m1->interp() == 6);
    CHECK(m2->interp() == 30);
    Var* v1 = new Var("a");
    CHECK_THROWS_WITH(v1->interp(), "Error: a variable has no value.");
}
TEST_CASE("has_variable tests"){
    Add* a1 = new Add(new Num(3), new Var("a"));
    Add* a2 = new Add(new Num(3), new Num(2));
    CHECK(a1->has_variable() == true);
    CHECK(a2->has_variable() == false);
    Mult* m1 = new Mult(new Num(3), new Var("a"));
    CHECK(m1->has_variable() == true);
    Num* n1 = new Num(3);
    CHECK(n1->has_variable() == false);
    Var* v1 = new Var("b");
    CHECK(v1->has_variable() == true);
}
TEST_CASE("subst tests"){
    Var* v1 = new Var("a");
    Var* v2 = new Var("a");
    Var* v3 = new Var("b");
    CHECK(v1->equals(v2));
    v2 = (Var*)(v2->subst("a", v3));
    CHECK(v2->equals(v3));

    Num* n1 = new Num(3);
    Var* v4 = new Var("z");
    Add* a1 = new Add(v4, n1);
    CHECK(a1->equals(new Add(new Var("z"), new Num(3))));


    Var* v5 = new Var("x");
    Num* n7 = new Num(7);
    Add* a2 = new Add(v5, n7);
    Var* v6 = new Var("y");
    Add* a3 = new Add(v6, n7);
    CHECK(a2->equals(new Add(v5, n7)));
    CHECK(a3->equals(new Add(v6, n7)));

    a2 = (Add*)a2->subst("x", new Var("y"));
    CHECK(a2->equals(a3));


    CHECK( (new Add(new Var("x"), new Num(7)))
                   ->subst("x", new Var("y"))
                   ->equals(new Add(new Var("y"), new Num(7))) );
    CHECK( (new Mult(new Var("x"), new Num(7)))
                   ->subst("x", new Var("y"))
                   ->equals(new Mult(new Var("y"), new Num(7))) );

}