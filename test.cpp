//
// Created by Spencer DeLange on 1/16/22.
//

#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "test.h"
#include "Expr.h"
#include "msdscript.h"
#include "Val.h"

// Static test assistance method returning a string representing the value of a given string expression
static std::string run(const std::string& s){
    return parse_str(s)->interp()->to_string();
}

int test(char* argv[]){
    return Catch::Session().run(1, argv);
}
TEST_CASE("equalstests") {
    NumExpr* n1 = new NumExpr(1);
    NumExpr* n2 = new NumExpr(2);
    NumExpr* n3 = new NumExpr(3);
    NumExpr* n4 = new NumExpr(4);
    AddExpr* a1 = new AddExpr(n1, n2);
    AddExpr* a2 = new AddExpr(n3, n4);
    MultExpr* m = new MultExpr(n1, n2);
    CHECK(n1->equals(n2) == false);
    CHECK(a1->equals(a2) == false);
    CHECK(a1->equals(a1) == true);
    CHECK(a1->equals(m) == false);
    CHECK(n1->equals(a1) == false);
    MultExpr* m1 = nullptr;
    CHECK(m->equals(m1) == false);
    VarExpr* v1 = nullptr;
    VarExpr* v2 = new VarExpr("x");
    CHECK(v2->equals(v1) == false);
    LetExpr* l1 = new LetExpr(new VarExpr("x"), new NumExpr(1), new NumExpr(2));
    LetExpr* l2 = new LetExpr(new VarExpr("x"), new NumExpr(1), new NumExpr(2));
    CHECK(v2->equals(l1) == false);
    CHECK(l1->equals(l2) == true);
    LetExpr* l3 = new LetExpr(new VarExpr("y"), new NumExpr(1), new NumExpr(2));
    CHECK(l1->equals(l3) == false);
    LetExpr* l4 = nullptr;
    CHECK(l3->equals(l4) == false);
}
TEST_CASE("interp tests") {
    NumExpr* n2 = new NumExpr(2);
    NumExpr* n3 = new NumExpr(3);
    AddExpr* a1 = new AddExpr(n2, n3);
    AddExpr* a2 = new AddExpr(n2, n3);
    MultExpr* m1 = new MultExpr(n2, n3);
    MultExpr* m2 = new MultExpr(a2, m1);
    CHECK(n2->interp()->equals(new NumVal(2)));
    CHECK(a1->interp()->equals(new NumVal(5)));
    CHECK(m1->interp()->equals(new NumVal(6)));
    CHECK(m2->interp()->equals(new NumVal(30)));
    VarExpr* v1 = new VarExpr("a");
    CHECK_THROWS_WITH(v1->interp(), "Error: a variable has no value.");
    LetExpr* l1 = new LetExpr(new VarExpr("x"), new NumExpr(1), new NumExpr(2));
    CHECK(l1->interp()->equals(new NumVal(2)));
    LetExpr* l2 = new LetExpr(new VarExpr("x"), new NumExpr(2), new AddExpr(new VarExpr("x"), new NumExpr(5)));
    CHECK(l2->interp()->equals(new NumVal(7)));

    LetExpr* l3 = new LetExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(2)), new AddExpr(new VarExpr("x"), new NumExpr(1)));
    LetExpr* l4 = new LetExpr(new VarExpr("x"), new NumExpr(5), l3);
    CHECK(l4->interp()->equals(new NumVal(8)));

    LetExpr* l5 = new LetExpr(new VarExpr("x"), new NumExpr(6), new AddExpr(new VarExpr("x"), new NumExpr(1)));
    LetExpr* l6 = new LetExpr(new VarExpr("x"), new NumExpr(5), l5);
    CHECK(l6->interp()->equals(new NumVal(7)));

    LetExpr* l7 = new LetExpr(new VarExpr("y"), new NumExpr(6), new AddExpr(new VarExpr("x"), new NumExpr(1)));
    LetExpr* l8 = new LetExpr(new VarExpr("x"), new NumExpr(5), l7);
    CHECK(l8->interp()->equals(new NumVal(6)));
}
TEST_CASE("has_variable tests"){
    AddExpr* a1 = new AddExpr(new NumExpr(3), new VarExpr("a"));
    AddExpr* a2 = new AddExpr(new NumExpr(3), new NumExpr(2));
    CHECK(a1->has_variable() == true);
    CHECK(a2->has_variable() == false);
    MultExpr* m1 = new MultExpr(new NumExpr(3), new VarExpr("a"));
    CHECK(m1->has_variable() == true);
    NumExpr* n1 = new NumExpr(3);
    CHECK(n1->has_variable() == false);
    VarExpr* v1 = new VarExpr("b");
    CHECK(v1->has_variable() == true);
    LetExpr* l3 = new LetExpr(new VarExpr("x"), new AddExpr(new VarExpr("x"), new NumExpr(2)), new NumExpr(3));
    CHECK(l3->has_variable() == true);
    LetExpr* l1 = new LetExpr(new VarExpr("x"), new NumExpr(1), new NumExpr(2));
    CHECK(l1->has_variable() == false);
    LetExpr* l4 = new LetExpr(new VarExpr("x"), new NumExpr(3), new AddExpr(new VarExpr("x"), new NumExpr(2)));
    CHECK(l4->has_variable() == true);
}
TEST_CASE("subst tests"){
    VarExpr* v1 = new VarExpr("a");
    VarExpr* v2 = new VarExpr("a");
    VarExpr* v3 = new VarExpr("b");
    CHECK(v1->equals(v2));
    v2 = (VarExpr*)(v2->subst("a", v3));
    CHECK(v2->equals(v3));

    NumExpr* n1 = new NumExpr(3);
    VarExpr* v4 = new VarExpr("z");
    AddExpr* a1 = new AddExpr(v4, n1);
    CHECK(a1->equals(new AddExpr(new VarExpr("z"), new NumExpr(3))));

    VarExpr* v5 = new VarExpr("x");
    NumExpr* n7 = new NumExpr(7);
    AddExpr* a2 = new AddExpr(v5, n7);
    VarExpr* v6 = new VarExpr("y");
    AddExpr* a3 = new AddExpr(v6, n7);
    CHECK(a2->equals(new AddExpr(v5, n7)));
    CHECK(a3->equals(new AddExpr(v6, n7)));

    a2 = (AddExpr*)a2->subst("x", new VarExpr("y"));
    CHECK(a2->equals(a3));

    VarExpr* v7 = new VarExpr("x");
    v6 = (VarExpr*)v7->subst("j", new VarExpr("j"));
    CHECK(v7->pretty_to_string() == "x");

    CHECK( (new AddExpr(new VarExpr("x"), new NumExpr(7)))
                   ->subst("x", new VarExpr("y"))
                   ->equals(new AddExpr(new VarExpr("y"), new NumExpr(7))) );
    CHECK( (new MultExpr(new VarExpr("x"), new NumExpr(7)))
                   ->subst("x", new VarExpr("y"))
                   ->equals(new MultExpr(new VarExpr("y"), new NumExpr(7))) );
}
TEST_CASE("print"){
    NumExpr* n1 = new NumExpr(3);
    std::stringstream out1("");
    n1->print(out1);
    CHECK(out1.str() == "3");

    VarExpr* v1 = new VarExpr("var1");
    std::stringstream out2("");
    v1->print(out2);
    CHECK(out2.str() == "var1");

    AddExpr* a1 = new AddExpr(n1, v1);
    std::stringstream out3("");
    a1->print(out3);
    CHECK(out3.str() == "(3+var1)");

    MultExpr* m1 = new MultExpr(n1, v1);
    std::stringstream out4("");
    m1->print(out4);
    CHECK(out4.str() == "(3*var1)");

    MultExpr* m2 = new MultExpr(m1, n1);
    std::stringstream out5("");
    m2->print(out5);
    CHECK(out5.str() == "((3*var1)*3)");

    AddExpr* a2 = new AddExpr(n1, a1);
    std::stringstream out6("");
    a2->print(out6);
    CHECK(out6.str() == "(3+(3+var1))");

    CHECK(a2->to_string() == "(3+(3+var1))");

    LetExpr* l1 = new LetExpr(new VarExpr("x"), new NumExpr(1), new NumExpr(2));
    CHECK(l1->to_string() == "(_let x=1 _in 2)");

    LetExpr* l2 = new LetExpr(new VarExpr("y"), new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2)));
    CHECK(l2->to_string() == "(_let y=3 _in (y+2))");

    LetExpr* l3 = new LetExpr(new VarExpr("x"), new NumExpr(5), new AddExpr(l2, new VarExpr("x")));
    CHECK(l3->to_string() == "(_let x=5 _in ((_let y=3 _in (y+2))+x))");

}
TEST_CASE("pretty_print"){
    NumExpr* n3 = new NumExpr(3);
    CHECK(n3->pretty_to_string() == "3");

    VarExpr* v1 = new VarExpr("v1");
    CHECK(v1->pretty_to_string() == "v1");

    AddExpr* a1 = new AddExpr(n3, v1);
    CHECK(a1->pretty_to_string() == "3 + v1");

    AddExpr* a2 = new AddExpr(a1, n3);
    CHECK(a2->pretty_to_string() == "(3 + v1) + 3");

    MultExpr* m1 = new MultExpr(n3, v1);
    CHECK(m1->pretty_to_string() == "3 * v1");

    NumExpr* n1 = new NumExpr(1);
    NumExpr* n2 = new NumExpr(2);
    MultExpr* m2 = new MultExpr(n2, n3);
    AddExpr* a4 = new AddExpr(n1, m2);
    CHECK(a4->pretty_to_string() == "1 + 2 * 3");

    MultExpr* m3 = new MultExpr(n1, new AddExpr(n2, n3));
    CHECK(m3->pretty_to_string() == "1 * (2 + 3)");

    MultExpr* m4 = new MultExpr(new MultExpr(new NumExpr(2), new NumExpr(3)), new NumExpr(4));
    CHECK(m4->pretty_to_string() == "(2 * 3) * 4");

    MultExpr* m5 = new MultExpr(new NumExpr(2), new MultExpr(new NumExpr(3), new NumExpr(4)));
    CHECK(m5->pretty_to_string() == "2 * 3 * 4");

    MultExpr* m6 = new MultExpr(new MultExpr(new NumExpr(3), new NumExpr(2)), new AddExpr(new NumExpr(2), new NumExpr(1)));
    CHECK(m6->pretty_to_string() == "(3 * 2) * (2 + 1)");

    AddExpr* a5 = new AddExpr(new MultExpr(new NumExpr(1), new NumExpr(2)),
                              new AddExpr(new NumExpr(4), new NumExpr(7)));
    CHECK(a5->pretty_to_string() == "1 * 2 + 4 + 7");

    AddExpr* a7 = new AddExpr(new AddExpr(n1, n2), new NumExpr(3));
    CHECK(a7->pretty_to_string() == "(1 + 2) + 3");

    n3 = new NumExpr(3);
    AddExpr* a8 = new AddExpr(n1, new AddExpr(n2, n3));
    CHECK(a8->pretty_to_string() == "1 + 2 + 3");

    AddExpr* a9 = new AddExpr(a7, new NumExpr(4));
    CHECK(a9->pretty_to_string() == "((1 + 2) + 3) + 4");

    LetExpr* l1 = new LetExpr(new VarExpr("y"), new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2)));
    CHECK(l1->pretty_to_string() == "_let y = 3\n"
                                    "_in  y + 2");

    AddExpr* a6 = new AddExpr(new NumExpr(7), l1);
    CHECK(a6->pretty_to_string() == "7 + _let y = 3\n"
                                    "    _in  y + 2");

    LetExpr* l2 = new LetExpr(new VarExpr("x"), new NumExpr(5), new AddExpr(new LetExpr(new VarExpr("y"), new NumExpr(3), new AddExpr(new VarExpr("y"), new NumExpr(2))), new VarExpr("x")));
    CHECK(l2->pretty_to_string() == "_let x = 5\n"
                                    "_in  (_let y = 3\n"
                                    "      _in  y + 2) + x");

    MultExpr* m7 = new MultExpr(new NumExpr(5), new LetExpr(new VarExpr("x"), new NumExpr(5), new VarExpr("x")));
    AddExpr* a10 = new AddExpr(m7, new NumExpr(1));
    CHECK(a10->pretty_to_string() == "5 * (_let x = 5\n"
                                     "     _in  x) + 1");
    CHECK(a10->interp()->equals(new NumVal(26)));

    AddExpr* a11 = new AddExpr(new LetExpr(new VarExpr("x"), new NumExpr(5), new VarExpr("x")), new NumExpr(1));
    CHECK(a11->pretty_to_string() == "(_let x = 5\n"
                                     " _in  x) + 1");

    MultExpr* m8 = new MultExpr(new NumExpr(5), new LetExpr(new VarExpr("x"), new NumExpr(5), new AddExpr(new VarExpr("x"), new NumExpr(1))));
    CHECK(m8->pretty_to_string() == "5 * _let x = 5\n"
                                    "    _in  x + 1");
    CHECK(m8->interp()->equals(new NumVal(30)));
}

TEST_CASE("parse"){
    CHECK(parse_str("5")->interp()->equals(new NumVal(5)));
    CHECK(parse_str("-5")->interp()->equals(new NumVal(-5)));
    CHECK(parse_str("5 + 2")->interp()->equals(new NumVal(7)));
    CHECK(parse_str("5 + -10")->interp()->equals(new NumVal((-5))));
    CHECK(parse_str("    5 + 2")->interp()->equals(new NumVal(7)));
    CHECK(parse_str("4 * 2")->interp()->equals(new NumVal(8)));
    CHECK(parse_str("(4 * 2)")->interp()->equals(new NumVal(8)));
    CHECK(parse_str("2 * 2 + 3")->interp()->equals(new NumVal(7)));
    CHECK(parse_str("2 * (2 + 3)")->interp()->equals(new NumVal(10)));
    CHECK(parse_str("(4)")->interp()->equals(new NumVal(4)));

    CHECK(parse_str("this")->pretty_to_string() == "this");
    CHECK(parse_str("this    ")->pretty_to_string() == "this");
    CHECK(parse_str("     this")->pretty_to_string() == "this");

    CHECK(parse_str("this + that")->pretty_to_string() == "this + that");

    //
    CHECK(parse_str("_let x = 4 _in x + -2")->interp()->equals(new NumVal(2)));
    CHECK(parse_str("_let x = 4 _in x + -2")->pretty_to_string() ==
    "_let x = 4\n_in  x + -2");
    CHECK(parse_str("_let x = 4 _in x + -2")->to_string() == "(_let x=4 _in (x+-2))");
    CHECK_THROWS_WITH(parse_str("_lt x = 4 _in x + -2")->interp(), "invalid input");
}

TEST_CASE("Val Tests"){
    CHECK(run("1 + 2") == "3");
    CHECK((new NumVal(3))->equals(new NumVal(3)));
    CHECK((new NumVal(3))->equals(new NumVal(2)) == false);
    CHECK(run("1 + 2") != "2");
    CHECK(run("1 + -2") == "-1");
    CHECK((new NumVal(2))->to_string() == "2");
    NumVal* n1 = new NumVal(2);
    CHECK(n1->to_expr()->equals(new NumExpr(2)));
    CHECK(n1->add_to(new NumVal(4))->equals(new NumVal(6)));
    CHECK(n1->mult_to(new NumVal(4))->equals(new NumVal(8)));
}

TEST_CASE("Refactor Tests"){
    CHECK(run("2 == 2") == "_true");
    CHECK(run("2 == 3") == "_false");
    CHECK((run("_let same = 1 == 2\n"
        "_in  _if 1 == 2\n"
        "     _then _false + 5\n"
        "     _else 88")) == "88");

    CHECK(parse_str("_let same = 1 == 2\n"
                    "_in  _if 1 == 2\n"
                    "     _then _false + 5\n"
                    "     _else 88")->pretty_to_string() == "_let same = 1 == 2\n"
                                                            "_in  _if 1 == 2\n"
                                                            "     _then _false + 5\n"
                                                            "     _else 88");
    CHECK_THROWS_WITH(run("_if 4 + 1\n"
              "_then 2\n"
              "_else 3"), "NumVal cannot be true or false");


}