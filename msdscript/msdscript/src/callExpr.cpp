#include "callExpr.h"
#include "expr.h"
#include "val.h"
#include "numExpr.h"
#include "varExpr.h"
#include "addExpr.h"
#include "multExpr.h"
#include "funExpr.h"
#include "numVal.h"
#include "env.h"
#include "step.h"
#include "cont.h"

#include "catch.h"
#include <iostream>

/*
 * **************************************
 * Methods for the CallExpr class
 * **************************************
 * */

/* constructor */
CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}


/**	NOTES on CallExpr::equals()
 * 	Uses std::dynamic_cast (in the CAST() macro, see pointer.h) to cast the 
 * 	argument to a CallExpr, see if it was the same class, then compare each 
 * 	member variable. This does not interpret the expression.
 * 
 *  Arg:    Expr pointer
 *  Return: bool
 * */
bool CallExpr::equals(PTR(Expr) e) {
    PTR(CallExpr) c = CAST(CallExpr)(e);
    if (c == NULL)
        return false;
    else
        return (this->to_be_called->equals(c->to_be_called) && this->actual_arg->equals(c->actual_arg));
}


/** NOTES on CallExpr::interp()
 *  Uses the Val class's call() method to call the function stored in 
 *  to_be_called() using the actual_arg member variable.
 * 
 *  Arg:    Env pointer
 *  Return: Val pointer
 * */
PTR(Val) CallExpr::interp(PTR(Env) env) {
    return this->to_be_called->interp(env)->call(this->actual_arg->interp(env));
}


/** NOTES on CallExpr::step_interp()
 *  Same idea as interp() above, but it uses the continuation format 
 * 	so we can perform the operation without using traditional recursion. 
 * 	Does not directly use the Val class's call() method.
 * 
 * Arg:     none
 * Return:  none
 * */
void CallExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = this->to_be_called;
    Step::cont = NEW(ArgThenCallCont)(actual_arg, Step::env, Step::cont);
}


/** NOTES on CallExpr::print()
 *  Uses a std::ostream erference to print the actual_arg member variable 
 *  surrounded by parentheses, with not spaces in between.
 * 
 * Arg:     std::osream reference
 * Return:  none
 * */
void CallExpr::print(std::ostream& stream) {
    this->to_be_called->print(stream);
    stream << "(";
    this->actual_arg->print(stream);
    stream << ")";
}


/** NOTES on CallExpr::pretty_print_at()
 *  Currently just calls print(). See expr.h for more description.
 * 
 *  Args:	print_mode_t	(see expr.h)
 *			std::ostream reference
 *	Return:	none
 * */
void CallExpr::pretty_print_at(print_mode_t mode, std::ostream& stream) {
    this->print(stream);
}



/**
 * ********************************************
 * TESTS FOR CallExpr
 * ********************************************
 * */

TEST_CASE("CallExpr equals()") {

    PTR(CallExpr) callExpr1 = NEW(CallExpr)( NEW(NumExpr)(1), NEW(NumExpr)(1) );
    PTR(CallExpr) callExpr2 = NEW(CallExpr)( NEW(NumExpr)(2), NEW(NumExpr)(2) );

    CHECK( callExpr1->equals( NEW(CallExpr)( NEW(NumExpr)(1), NEW(NumExpr)(1) ) ) );
    CHECK( callExpr1->equals(callExpr2) == false );

}


TEST_CASE("CallExpr interp()") {

    PTR(NumExpr) numExpr1 = NEW(NumExpr)(1);
    PTR(NumExpr) numExpr2 = NEW(NumExpr)(2);
    PTR(NumVal) numVal1 = NEW(NumVal)(1);
    PTR(NumVal) numVal2 = NEW(NumVal)(2);
    PTR(NumVal) numVal3 = NEW(NumVal)(3);

    PTR(CallExpr) callExpr1 = NEW(CallExpr)( NEW(FunExpr)( "x", numExpr1 ), numExpr1 );
    CHECK( ( callExpr1->interp(Env::empty) )->equals( numVal1 ) == true );
    CHECK( ( callExpr1->interp(Env::empty) )->equals( numVal2 ) == false );

    PTR(CallExpr) callExpr3 = NEW(CallExpr)( NEW(FunExpr)( "x", NEW(MultExpr)( numExpr1, numExpr2 ) ), numExpr1 );
    CHECK( callExpr3->interp(Env::empty)->equals( numVal2 ) == true );
    CHECK( callExpr3->interp(Env::empty)->equals( numVal3 ) == false );
    
}


TEST_CASE("CallExpr print()") {

    PTR(NumExpr) numExpr1 = NEW(NumExpr)(1);
    PTR(NumExpr) numExpr2 = NEW(NumExpr)(2);
    PTR(VarExpr) varExprX = NEW(VarExpr)("x");

    PTR(CallExpr) callExpr1 = NEW(CallExpr)( varExprX, numExpr1 );
    PTR(CallExpr) callExpr2 = NEW(CallExpr)( numExpr2, numExpr1 );

    CHECK( callExpr1->to_string() == "x(1)");
    CHECK( callExpr2->to_string() == "2(1)");

}


