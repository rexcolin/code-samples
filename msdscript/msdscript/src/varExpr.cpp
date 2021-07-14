#include "expr.h"
#include "varExpr.h"
#include "val.h"
#include "catch.h"
#include "env.h"
#include "numVal.h"
#include "step.h"
#include <string>
#include <iostream>

/*
 * **************************************
 * Methods for the VarExpr class
 * **************************************
 * */


/* constructor */
VarExpr::VarExpr(std::string rep) {
	this->rep = rep;
}


/** NOTES on VarExpr::equals()
 * 	Uses std::dynamic_cast (in the CAST() macro, see pointer.h) to cast the 
 * 	argument to a VarExpr, see if it was the same class, then compare each 
 * 	member variable, which is just a std::string. This does not interpret 
 * 	the expression.
 * 
 * 	Arg:	Expr pointer
 * 	Return:	bool
 * */
bool VarExpr::equals(PTR(Expr) e) {
	PTR(VarExpr) c = CAST(VarExpr)(e);
	if (c == NULL)
		return false;
	else
		return (this->rep == c->rep);
}


/** NOTES on VarExpr::interp()
 * 	This uses the Env class's lookup() method to look through the Env 
 * 	provided as an argument to see if the VarExpr::rep is in the 
 * 	environment. If it is, it will return the value associated with it. 
 * 	If not, then it will eventually reach an EmptyEnv and will throw an 
 * 	error.
 *  
 *  Arg:    Env pointer
 *  Return: Val pointer
 * */
PTR(Val) VarExpr::interp(PTR(Env) env) {
	return env->lookup(rep);
}


/** NOTES on VarExpr::step_interp()
 * 	Same idea as interp() above, but it uses the continuation loop 
 * 	so we can perform the operation without using tradtional recursion. 
 * 	Instead of returning a val, it stores the Val that comes from the 
 * 	Env::lookup() methos in the Step::val field.
 * 
 * 	Arg:	none		(values are stored in the global variables 
 * 						in the Step class)
 * 	Return:	none
 * */
void VarExpr::step_interp() {
	Step::mode = Step::continue_mode;
	Step::val = Step::env->lookup(rep);
	Step::cont = Step::cont;		/* no-op */
}


/**	NOTES on VarExpr::print()
 * 	Prints the std::string value stored in VarExpr::rep, without parentheses 
 * 	or spaces.
 * 
 *  Arg:     std::osream reference
 * 	Return:  none
 * */
void VarExpr::print(std::ostream& stream) {
	stream << this->rep;
}


/** NOTES on VarExpr::pretty_print_at()
 * 	Not fully implemented. Currently does the exact same thing as VarExpr::print() 
 * 	above.
 * 
 *	Args:	print_mode_t	(see expr.h)
 *			std::ostream reference
 *	Return:	none
 * */
void VarExpr::pretty_print_at(print_mode_t mode, std::ostream& stream) {
	stream << this->rep;
}




/*
 * *************************************************************************
 * TESTS for VarExpr
 * *************************************************************************
 * */

TEST_CASE("VarExpr equals") {
	CHECK( NEW(VarExpr)("a")->equals( NEW(VarExpr)("a") ) == true );
	CHECK( NEW(VarExpr)("a")->equals( NEW(VarExpr)("b") ) == false );
}


TEST_CASE("VarExpr interp") {

	PTR(VarExpr) varX = NEW(VarExpr)("x");
	CHECK_THROWS_WITH( varX->interp(Env::empty), "free variable: x" );

	PTR(ExtendedEnv) envX = NEW(ExtendedEnv)( "x", NEW(NumVal)(1), Env::empty );
	CHECK( varX->interp( envX )->equals( NEW(NumVal)(1) ) );

}


TEST_CASE("VarExpr print") {
	PTR(VarExpr) test = NEW(VarExpr)("a");
	CHECK( test->to_string() == "a" );
}
