#include "funExpr.h"
#include "expr.h"
#include "funVal.h"
#include "val.h"

#include "addExpr.h"
#include "numExpr.h"
#include "varExpr.h"
#include "catch.h"
#include "env.h"
#include "step.h"
#include <string>
#include <iostream>


/*
 * **************************************
 * Methods for the FunExpr class
 * **************************************
 * */


/* constructor */
FunExpr::FunExpr(std::string formal_arg, PTR(Expr) body) {
    this->formal_arg = formal_arg;
    this->body = body;
}


/** NOTES on FunExpr::equals()
 *  Uses std::dynamic_cast (in the CAST() macro, see pointer.h) to cast the 
 * 	argument to an FunExpr, see if it was the same class, then compare each 
 * 	member variable. This does not interpret the expression.
 * 
 * 	Arg:	Expr pointer
 * 	Return:	bool
 * */
bool FunExpr::equals(PTR(Expr) e) {
    PTR(FunExpr) temp = CAST(FunExpr)(e);
    if (temp == NULL)
        return false;
    else
        return ( this->formal_arg == temp->formal_arg &&
        this->body->equals(temp->body) );
}


/** NOTES on FunExpr::interp()
 *  Returns a new FunVal object using the FunExpr::formal_arg and FunExpr::body 
 *  member variables from the object that is calling the function, and adds the 
 *  Env object taken as a parameter, which will be used to call the function. 
 *  Only FunVal objects can be called using the Val::call() method.
 * 
 *  Arg:    Env pointer
 *  Return: FunVal
 * */
PTR(Val) FunExpr::interp(PTR(Env) env) {
    return NEW(FunVal)(formal_arg, body, env);
}


/** NOTES on FunExpr::step_interp()
 *  As this is one of the 'end points' of step interpretation, it saves a 
 * 	new FunVal in the Step::val field, not unlike how FunExpr::interp() 
 * 	returns a new FunVal.
 * 
 * 	Arg:	none		(values are stored in the global variables 
 * 						in the Step class)
 * 	Return:	none
 * */
void FunExpr::step_interp() {
    Step::mode = Step::continue_mode;
    Step::val = NEW(FunVal)(formal_arg, body, Step::env);
    Step::cont = Step::cont;        /* no-op */
}


/** NOTES on FunExpr::print()
 *  Instead of actually printing the member variables, this prints just 
 *  the word "function" inside square brackets, as follows:
 * 
 *      [function]
 * 
 *  Arg:    std::ostream reference
 *  Return: none
 * */
void FunExpr::print(std::ostream& stream) {
    stream << "[function]";
}


/** NOTES on FunExpr::pretty_print_at()
 *  Not implemented, currently just calls print().
 * 
 *  Arg:    std::ostream reference
 *  Return: none
 * */
void FunExpr::pretty_print_at(print_mode_t mode, std::ostream& stream) {
    this->print(stream);
}



/**
 * ********************************************
 * TESTS for FunExpr class
 * ********************************************
 * */

TEST_CASE("FunExpr equals()") {

    PTR(FunExpr) funExpr1 = NEW(FunExpr)( "x",
                        NEW(AddExpr)( NEW(VarExpr)("x"),
                                        NEW(NumExpr)(1) ) );
    PTR(FunExpr) funExpr2 = NEW(FunExpr)( "x",
                        NEW(AddExpr)( NEW(VarExpr)("x"),
                                        NEW(NumExpr)(2) ) );
    
    CHECK( funExpr1->equals(NEW(FunExpr)( "x",
                        NEW(AddExpr)( NEW(VarExpr)("x"),
                                        NEW(NumExpr)(1) ) )) );
    CHECK( funExpr1->equals(funExpr2) == false );

}


TEST_CASE("FunExpr interp()") {

    PTR(FunVal) funVal1 = NEW(FunVal)( "x",
                        NEW(AddExpr)( NEW(VarExpr)("x"),
                                        NEW(NumExpr)(1) ), Env::empty );
    PTR(FunExpr) funExpr1 = NEW(FunExpr)( "x",
                        NEW(AddExpr)( NEW(VarExpr)("x"),
                                        NEW(NumExpr)(1) ) );

    CHECK( funExpr1->interp(Env::empty)->equals(funVal1) );

}


TEST_CASE("FunExpr print()") {

    std::string expected1 = "[function]";
    PTR(FunExpr) funExpr1 = NEW(FunExpr)( "x", NEW(AddExpr)( NEW(VarExpr)("x"), NEW(NumExpr)(1) ) );

    CHECK( funExpr1->to_string() == expected1 );

}

