#include "expr.h"
#include "numExpr.h"
#include "val.h"
#include "numVal.h"
#include "catch.h"
#include "env.h"
#include "step.h"
#include <iostream>

/*
 * **************************************
 * Methods for the NumExpr class
 * **************************************
 * */


/* constructor */
NumExpr::NumExpr(int rep) {
	this->rep = rep;
}


/** NOTES on NumExpr::equals()
 * 	Uses std::dynamic_cast (in the CAST() macro, see pointer.h) to cast the 
 * 	argument to a NumExpr, see if it was the same class, then compare each 
 * 	member variable, which is just an int. This does not interpret the expression.
 * 
 * 	Arg:	Expr pointer
 * 	Return:	bool
 * */
bool NumExpr::equals(PTR(Expr) e) {
	PTR(NumExpr) c = CAST(NumExpr)(e);
	if (c == NULL)
		return false;
	else
		return (this->rep == c->rep);
}


/** NOTES on NumExpr::interp()
 * 	As this is one of the 'end points' interpretation, it just returns a NumVal 
 * 	with the same int as a number variable.
 * 
 *  Arg:    Env pointer
 *  Return: Val pointer
 * */
PTR(Val) NumExpr::interp(PTR(Env) env) {
	return NEW(NumVal)(this->rep);
}


/** NOTES on NumExpr::step_interp()
 * 	As this is one of the 'end points' of step interpretation, it saves a 
 * 	new NumVal in the Step::val field, not unlike how NumExpr::interp() 
 * 	returns a new NumVal.
 * 
 * 	Arg:	none		(values are stored in the global variables 
 * 						in the Step class)
 * 	Return:	none
 * */
void NumExpr::step_interp() {
	Step::mode = Step::continue_mode;
	Step::val = NEW(NumVal)(rep);
	Step::cont = Step::cont;		/* no-op */
}


/**	NOTES on NumExpr::print()
 * 	Prints the int value stored in NumExpr::rep, without parentheses or spaces.
 * 
 *  Arg:     std::osream reference
 * 	Return:  none
 * */
void NumExpr::print(std::ostream& stream) {
	stream << this->rep;
}


/** NOTES on NumExpr::pretty_print_at()
 * 	Not fully implemented. Currently does the exact same thing as NumExpr::print() 
 * 	above.
 * 
 *	Args:	print_mode_t	(see expr.h)
 *			std::ostream reference
 *	Return:	none
 * */
void NumExpr::pretty_print_at(print_mode_t mode, std::ostream& stream) {
	stream << this->rep;
}




/*
 * *************************************************************************
 * TESTS for NumExpr
 * *************************************************************************
 * */

TEST_CASE("NumExpr equals"){
	CHECK( NEW(NumExpr)(1)->equals( NEW(NumExpr)(1) ) == true );
	CHECK( NEW(NumExpr)(1)->equals( NEW(NumExpr)(2) ) == false );
}


TEST_CASE("NumExpr interp") {
	CHECK( NEW(NumExpr)(1)->interp(Env::empty)->equals( NEW(NumVal)(1) ) );
}


TEST_CASE("NumExpr print") {
	PTR(NumExpr) one = NEW(NumExpr)(1);
	CHECK(one->to_string() == "1");
}