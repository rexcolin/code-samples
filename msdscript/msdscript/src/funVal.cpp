#include "funVal.h"
#include "numVal.h"
#include "funExpr.h"
#include "catch.h"
#include "varExpr.h"
#include "addExpr.h"
#include "numExpr.h"
#include "env.h"

#include <string>


/*
 * **************************************
 * Methods for the FunVal class
 * **************************************
 * */


/* constructor */
FunVal::FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env) {
    
    this->formal_arg = formal_arg;
    this->body = body;
    this->env = env;
}


/**	NOTES on FunVal::equals()
 * 	Uses std::dynamic_cast (in the CAST() macro, see pointer.h) to cast the 
 * 	argument to an FunVal, see if it was the same class, then compare each 
 * 	member variable. Makes use of the Expr class's equals() method to compare 
 *  the FunExpr::body member variables.
 * 
 * 	Arg:	Val pointer
 * 	Return:	bool
 * */
bool FunVal::equals(PTR(Val) val) {
    PTR(FunVal) c = CAST(FunVal)(val);
    if (c == NULL)
        return false;
    else
        return (this->body->equals(c->body)); //can't use the == because it's an Expr, not an int rep
}


/** NOTES on FunVal::add_to()
 *  This method is intended to use with NumVals only, so here it throws 
 *  a std::runtime error.
 * */
PTR(Val) FunVal::add_to(PTR(Val) val) {
    throw std::runtime_error("You can't add functions");
}


/** NOTES on FunVal::mult_to()
 *  This method is intended to use with NumVals only, so here it throws 
 *  a std::runtime error.
 * */
PTR(Val) FunVal::mult_to(PTR(Val) val) {
    throw std::runtime_error("You can't multiply functions");
}


/** NOTES on FunVal::to_string()
 *  Rather than converting the various member variables to a string, 
 *  this just returns a std::string that says function in square brackets.
 * 
 *  Arg:     none
 *  Return:  std::string 
 * */
std::string FunVal::to_string() {
    return "[function]";
}


/** NOTES on FunVal::call()
 *  This is how we interpret functions. It is basically a wrapper to call 
 *  interp() on the FunVal::body member variable, by creating a new ExtendedEnv 
 *  using the Val provided as an argument and FunExpr::formal_arg.
 * 
 *  Arg:    Val pointer
 *  Return: Val pointer
 * */
PTR(Val) FunVal::call(PTR(Val) val) {
    return this->body->interp(NEW(ExtendedEnv)(formal_arg, val, env));
}


/** NOTES on FunVal::call_step()
 *  Basically does the same thing as call() above, but using the continuation 
 *  loop. Instead of calling interp() on the body, it stores the FunVal::body 
 *  and ExtendedEnv in the appropriate Step fields.
 * */
void FunVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) {
    Step::mode = Step::interp_mode;
    Step::expr = body;
    Step::env = NEW(ExtendedEnv)(formal_arg, actual_arg_val, env);
    Step::cont = rest;
}




/**
 * ***************************************************
 * Tests for FunVal
 * */

TEST_CASE("FunVal equals()") {

    PTR(FunVal) fun1 = NEW(FunVal)( "x",
                            NEW(AddExpr)( NEW(VarExpr)("x"), 
                                        NEW(NumExpr)(1) ),
                            Env::empty );
    PTR(FunVal) fun2 = NEW(FunVal)( "x",
                            NEW(AddExpr)( NEW(VarExpr)("x"),
                                        NEW(NumExpr)(1) ),
                            Env::empty );
    PTR(FunVal) fun3 = NEW(FunVal)( "x",
                            NEW(AddExpr)( NEW(VarExpr)("x"),
                                        NEW(NumExpr)(2) ),
                            Env::empty );
    PTR(FunVal) fun4 = NEW(FunVal)( "y",
                            NEW(AddExpr)( NEW(VarExpr)("y"),
                                        NEW(NumExpr)(1) ),
                            Env::empty );
    
    CHECK( fun1->equals(fun2) );
    CHECK( fun1->equals(fun3) == false );
    CHECK( fun1->equals(fun4) == false );
    
}


TEST_CASE("FunVal add_to()") {

    PTR(FunVal) funVal1 = NEW(FunVal)( "x",
                            NEW(AddExpr)( NEW(VarExpr)("x"),
                                        NEW(NumExpr)(1) ),
                            Env::empty );
    PTR(FunVal) funVal2 = NEW(FunVal)( "x",
                            NEW(AddExpr)( NEW(VarExpr)("x"),
                                        NEW(NumExpr)(2) ),
                            Env::empty );
    
    CHECK_THROWS_WITH( funVal1->add_to(funVal2), "You can't add functions" );
}


TEST_CASE("FunVal mult_to()") {

    PTR(FunVal) funVal1 = NEW(FunVal)( "x",
                            NEW(AddExpr)( NEW(VarExpr)("x"),
                                        NEW(NumExpr)(1) ),
                            Env::empty );
    PTR(FunVal) funVal2 = NEW(FunVal)( "x",
                            NEW(AddExpr)( NEW(VarExpr)("x"),
                                        NEW(NumExpr)(2) ),
                            Env::empty );
    
    CHECK_THROWS_WITH( funVal1->mult_to(funVal2), "You can't multiply functions" );
}


TEST_CASE("FunVal toString()") {
   
   PTR(FunVal) function = NEW(FunVal)( "x", NEW(NumExpr)(4), Env::empty );
   CHECK( function->to_string() == "[function]" );
}


TEST_CASE("FunVal call()") {

PTR(FunVal) function = NEW(FunVal)( "x", NEW(NumExpr)(4), Env::empty );

CHECK( (function->call( NEW(NumVal)(1) ))->equals(NEW(NumVal)(4)) );
CHECK( (function->call( NEW(NumVal)(2) ))->equals(NEW(NumVal)(6)) == false );
}
