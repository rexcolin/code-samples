#include "step.h"
#include "env.h"
#include "cont.h"
#include "expr.h"
#include "catch.h"
#include "boolVal.h"
#include "numVal.h"
#include "callExpr.h"
#include "funExpr.h"
#include "varExpr.h"
#include "numExpr.h"
#include "addExpr.h"
#include "multExpr.h"
#include "parse.h"

Step::mode_t Step::mode;
PTR(Expr) Step::expr;
PTR(Env) Step::env;
PTR(Val) Step::val;
PTR(Cont) Step::cont;


/** NOTES on Step::interp_by_steps()
 *  This method sets the end of the continuation loop, with 
 *  an EmptyEnv in Step::env, a nullptr in Step::val, and a 
 *  DoneCont in Step::cont, and then calls step_interp() on 
 *  Step::expr or step_continue on Step::cont.
 * 
 *  Arg:    Expr pointer
 *  Return: Val pointer
 * */
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



/****************************************
 *          TESTS
 * **************************************/

TEST_CASE("step_interp() with AddExpr") {

    CHECK( Step::interp_by_steps(parse_string("2 + 2"))->equals(NEW(NumVal)(4)) );
}

TEST_CASE("step_interp() with BoolExpr") {

    CHECK( Step::interp_by_steps( parse_string("_true") )->equals( NEW(BoolVal)(true) ) );
    CHECK( Step::interp_by_steps( parse_string("(1 == 2)") )->equals( NEW(BoolVal)(false) ) );
}

TEST_CASE("step_interp with CallExpr") {

    PTR(NumExpr) four = NEW(NumExpr)(4);
    PTR(FunExpr) funExprTest = NEW(FunExpr)( "x", NEW(MultExpr)( NEW(VarExpr)("x"), four ) );
    PTR(CallExpr) callExprTest = NEW(CallExpr)( funExprTest, four );

    CHECK( Step::interp_by_steps(callExprTest)->equals( NEW(NumVal)(16) ) == true);
}

TEST_CASE("step_interp() with DoneCont") {
    CHECK_THROWS_WITH( Cont::done->step_continue(), "can't continue done" );
}

TEST_CASE("step_interp() with IfExpr") {

    CHECK( Step::interp_by_steps( parse_string("_if _true _then 1 _else 2") )->equals( NEW(NumVal)(1) ) );
    CHECK( Step::interp_by_steps( parse_string("_if _false _then 1 _else 2") )->equals( NEW(NumVal)(2) ) );
    CHECK( Step::interp_by_steps( parse_string("_if (1 == 1) _then 1 _else 2") )->equals( NEW(NumVal)(1) ) );
}

TEST_CASE("step_interp() with LetExpr") { 

    CHECK( Step::interp_by_steps( parse_string("_let x = 1 _in x + 2") )->equals( NEW(NumVal)(3) ) );
    
}

TEST_CASE("step_interp() with MultExpr") {

    CHECK( Step::interp_by_steps( parse_string("2 * 5") )->equals( NEW(NumVal)(10) ) );
}

TEST_CASE("step_interp() with NumExpr") {
    
    CHECK( Step::interp_by_steps( parse_string("1") )->equals( NEW(NumVal)(1) ) );
}

TEST_CASE("step_interp with countdown") {

    std::string countdown = "_let countdown = _fun(countdown) _fun(n) _if n == 0 _then 0 _else countdown(countdown)(n + -1) _in countdown(countdown)(1000000)";

    CHECK( Step::interp_by_steps( parse_string(countdown) )->equals( NEW(NumVal)(0) ) );
}

TEST_CASE("step_interp with factoral") {

    std::string fac = "_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1) _in factrl(factrl)(10)";

    CHECK( Step::interp_by_steps( parse_string(fac) )->equals( NEW(NumVal)(3628800) ) );
}
