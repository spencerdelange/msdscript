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
TEST_CASE("print"){
    Num* n1 = new Num(3);
    std::stringstream out1("");
    n1->print(out1);
    CHECK(out1.str() == "3");

    Var* v1 = new Var("var1");
    std::stringstream out2("");
    v1->print(out2);
    CHECK(out2.str() == "var1");

    Add* a1 = new Add(n1, v1);
    std::stringstream out3("");
    a1->print(out3);
    CHECK(out3.str() == "(3+var1)");

    Mult* m1 = new Mult(n1, v1);
    std::stringstream out4("");
    m1->print(out4);
    CHECK(out4.str() == "(3*var1)");

    Mult* m2 = new Mult(m1, n1);
    std::stringstream out5("");
    m2->print(out5);
    CHECK(out5.str() == "((3*var1)*3)");

    Add* a2 = new Add(n1, a1);
    std::stringstream out6("");
    a2->print(out6);
    CHECK(out6.str() == "(3+(3+var1))");

    CHECK(a2->to_string() == "(3+(3+var1))");
}
TEST_CASE("pretty_print"){
    Num* n3 = new Num(3);
    CHECK(n3->pretty_to_string() == "3");

    Var* v1 = new Var("v1");
    CHECK(v1->pretty_to_string() == "v1");

    Add* a1 = new Add(n3, v1);
    CHECK(a1->pretty_to_string() == "3 + v1");

    Add* a2 = new Add(a1, n3);
    CHECK(a2->pretty_to_string() == "(3 + v1) + 3");

    Mult* m1 = new Mult(n3, v1);
    CHECK(m1->pretty_to_string() == "3 * v1");

    Num* n1 = new Num(1);
    Num* n2 = new Num(2);
    Mult* m2 = new Mult(n2, n3);
    Add* a4 = new Add(n1, m2);
    CHECK(a4->pretty_to_string() == "1 + 2 * 3");

    Mult* m3 = new Mult(n1, new Add(n2, n3));
    CHECK(m3->pretty_to_string() == "1 * (2 + 3)");

    Mult* m4 = new Mult(new Mult(new Num(2), new Num(3)), new Num(4));
    CHECK(m4->pretty_to_string() == "(2 * 3) * 4");

    Mult* m5 = new Mult(new Num(2), new Mult(new Num(3), new Num(4)));
    CHECK(m5->pretty_to_string() == "2 * 3 * 4");
}