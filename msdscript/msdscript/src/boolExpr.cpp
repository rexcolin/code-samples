#include "expr.h"
#include "boolExpr.h"
#include "numExpr.h"
#include "val.h"
#include "boolVal.h"
#include "catch.h"
#include "env.h"
#include "step.h"
#include <string>
#include <iostream>

/*
 * **************************************
 * Methods for the BoolExpr class
 * **************************************
 * */


/* constructor */
BoolExpr::BoolExpr(bool rep) {
	this->rep = rep;
}


/**	NOTES on BoolExpr::equals()
 * 	Uses std::dynamic_cast (in the CAST() macro, see pointer.h) to cast the 
 * 	argument to an BoolExpr, see if it was the same class, then compare each 
 * 	member variable, which in this case is just a bool. This does not interpret 
 * 	the expression.
 * 
 * 	Arg:	Expr pointer
 * 	Return:	bool
 * */
bool BoolExpr::equals(PTR(Expr) e) {
	PTR(BoolExpr) c = CAST(BoolExpr)(e);
	if (c == NULL)
		return false;
	else
		return (this->rep == c->rep);
}


/** NOTES on BoolExpr::interp()
 * 	Returns a new BoolVal object with the same member variable (ie, same
 * 	boolean value).
 * 
 *  Arg:    Env pointer
 *  Return: Val pointer
 * */
PTR(Val) BoolExpr::interp(PTR(Env) env) {
	return NEW(BoolVal)(this->rep);
}


/** NOTES on BoolExpr::step_interp()
 * 	As this is one of the 'end points' of step interpretation, it saves a 
 * 	new BoolVal in the Step::val field, not unlike how BoolExpr::interp() 
 * 	returns a new BoolVal.
 * 
 * 	Arg:	none		(values are stored in the global variables 
 * 						in the Step class)
 * 	Return:	none
 * */
void BoolExpr::step_interp() {
	Step::mode = Step::continue_mode;
	Step::val = NEW(BoolVal)(rep);
	Step::cont = Step::cont;		/* no-op */
}


/** NOTES on BoolExpr::print()
 * 	Prints "_true" or "_false" with an underscore preceding the word, 
 * 	depending on the value of the rep member variable of the calling 
 * 	BoolExpr.
 * 
 * 	Arg:     std::osream reference
 * 	Return:  none
 * */
void BoolExpr::print(std::ostream& stream) {
	
	std::string strToPrint;
	if ( this->rep ) {
		strToPrint = "_true";
	} else {
		strToPrint = "_false";
	}
	stream << strToPrint;
}


/** NOTES on BoolExpr::pretty_print_at()
 * 	Not fully implemented. Currently does the same thing as BoolExpr::print().
 * 
 *	Args:	print_mode_t	(see expr.h)
 *			std::ostream reference
 *	Return:	none
 * */
void BoolExpr::pretty_print_at(print_mode_t mode, std::ostream& stream) {
	stream << this->rep;
}



/**
 * ******************************************
 * TESTS for BoolExpr class
 * ******************************************
 * */

TEST_CASE("BoolExpr equals()") {

	CHECK( NEW(BoolExpr)(true)
			->equals( NEW(BoolExpr)(true)) );

	CHECK( NEW(BoolExpr)(true)
			->equals( NEW(BoolExpr)(false))
			== false );

	CHECK( NEW(BoolExpr)(true)
			->equals( NEW(NumExpr)(1))
			== false );

}


TEST_CASE("BoolExpr interp()") {

	PTR(BoolExpr) boolExprTrue = NEW(BoolExpr)(true);
	PTR(BoolVal) boolValTrue = NEW(BoolVal)(true);
	CHECK( CAST(BoolVal)(boolExprTrue->interp(Env::empty))->rep == boolValTrue->rep );

	PTR(BoolExpr) boolExprFalse = NEW(BoolExpr)(false);
	CHECK( CAST(BoolVal)(boolExprFalse->interp(Env::empty))->rep != boolValTrue->rep );

}


TEST_CASE("BoolExpr print()") {

	PTR(BoolExpr) boolExprTrue = NEW(BoolExpr)(true);
	std::string strTrue = "_true";
	CHECK( boolExprTrue->to_string() == strTrue  );

	PTR(BoolExpr) boolExprFalse = NEW(BoolExpr)(false);
	std::string strFalse = "_false";
	CHECK( boolExprFalse->to_string() == strFalse );

}


