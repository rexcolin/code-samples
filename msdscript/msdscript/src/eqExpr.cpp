#include "expr.h"
#include "eqExpr.h"
#include "numExpr.h"
#include "addExpr.h"
#include "varExpr.h"
#include "val.h"
#include "boolVal.h"
#include "catch.h"
#include "env.h"
#include "step.h"
#include "cont.h"
#include <string>
#include <iostream>

/**
 * ********************************
 * Methods for EqExpr class
 * ********************************
 * */

/* constructor */
EqExpr::EqExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
	this->lhs = lhs;
	this->rhs = rhs;
}


/**	NOTES on EqExpr::equals()
 * 	Uses std::dynamic_cast (in the CAST() macro, see pointer.h) to cast the 
 * 	argument to an EqExpr, see if it was the same class, then compare each 
 * 	member variable. This does not interpret the expression.
 * 
 * 	Arg:	Expr pointer
 * 	Return:	bool
 * */
bool EqExpr::equals(PTR(Expr) e) {
	PTR(EqExpr) temp = CAST(EqExpr)(e);
	if (temp == NULL)
		return false;
	else
		return ( this->lhs->equals(temp->lhs) && this->rhs->equals(temp->rhs) );
}


/**	NOTES on EqExpr::interp()
 * 	As interp() returns a Val object, we interp() each member variable, 
 * 	then use the Val class's equals() method (see val.h) to compare them 
 * 	and know whether we should return a BoolVal(true) or BoolVal(false). 
 * 	See boolVal.h.
 * 
 *  Arg:    Env pointer
 *  Return: Val pointer
 * */
PTR(Val) EqExpr::interp(PTR(Env) env) {
	if ( lhs->interp(env)->equals( rhs->interp(env) ) ) {
		return NEW(BoolVal)(true);
	} else {
		return NEW(BoolVal)(false);
	}
}


/** NOTES on EqExpr::step_interp()
 * 	Same idea as interp() above, but it uses the continuation format 
 * 	so we can perform the operation without using traditional recursion. 
 * 	Does not directly use the Val class's equals() method.
 * 
 * 	Arg:	none		(values are stored in the global variables 
 * 						in the Step class)
 * 	Return:	none
 * */
void EqExpr::step_interp() {
	Step::mode = Step::interp_mode;
	Step::expr = lhs;
	Step::env = Step::env;		/* no-op */
	Step::cont = NEW(RightThenEqCont)(rhs, Step::env, Step::cont);
}


/**	NOTES on EqExpr::print()
 * 	Uses a std::osream reference to print the member variables separated 
 * 	by a double equals sign (with a space on either side of it) and the 
 * 	whole EqExpr surrounded by parentheses, and no space between the 
 * 	member variables and the parentheses.
 * 		Example:
 * 				(1 == 1)
 * 
 * 	Arg:     std::osream reference
 * 	Return:  none
 * */
void EqExpr::print(std::ostream& stream) {
	stream << "(";
	this->lhs->print(stream);
	stream << " == ";
	this->rhs->print(stream);
	stream << ")";
}


/** NOTES on EqExpr::pretty_print_at()
 * 	Checks the print mode, but then currently does the exact same thing as 
 * 	EqExpr::print().
 * 
 *	Args:	print_mode_t	(see expr.h)
 *			std::ostream reference
 *	Return:	none
 * */
void EqExpr:: pretty_print_at(print_mode_t mode, std::ostream& stream) {
	if (mode >= print_group_AddExpr_or_mult) {
			stream << "(";
	}
	this->lhs->pretty_print_at(print_group_AddExpr_or_mult, stream);
	stream << " == ";
	this->rhs->pretty_print_at(print_group_AddExpr, stream);
	if (mode >= print_group_AddExpr_or_mult) {
		stream << ")";
	}
}


/**
 * **************************************
 * TESTS for EqExpr
 * **************************************
 * */

TEST_CASE("EqExpr equals()") {

	CHECK( NEW(EqExpr)( NEW(NumExpr)(1), NEW(NumExpr)(1))
			->equals( NEW(EqExpr)( NEW(NumExpr)(1), NEW(NumExpr)(1)) )
			== true );

	CHECK( NEW(EqExpr)( NEW(NumExpr)(1), NEW(NumExpr)(1))
			->equals( NEW(EqExpr)( NEW(NumExpr)(1), NEW(NumExpr)(2)))
			== false );

	CHECK( NEW(EqExpr)( NEW(NumExpr)(1), NEW(NumExpr)(1))
			->equals( NEW(AddExpr)( NEW(NumExpr)(1), NEW(NumExpr)(1)))
			== false );

}


TEST_CASE("EqExpr interp()") {

	PTR(EqExpr) eqExprTrue = NEW(EqExpr)( NEW(NumExpr)(1), NEW(NumExpr)(1) );
	PTR(BoolVal) boolValTrue = CAST(BoolVal)(eqExprTrue->interp(Env::empty));
	CHECK( boolValTrue->rep == true );

	PTR(EqExpr) eqExprFalse = NEW(EqExpr)( NEW(NumExpr)(1), NEW(NumExpr)(2) );
	PTR(BoolVal) boolValFalse = CAST(BoolVal)(eqExprFalse->interp(Env::empty));
	CHECK( boolValFalse->rep == false );

}


TEST_CASE("EqExpr print()") {

	PTR(EqExpr) eqExprStr = NEW(EqExpr)( NEW(NumExpr)(1), NEW(NumExpr)(2) );
	std::string strExpected = "(1 == 2)";
	CHECK( eqExprStr->to_string() == strExpected );

}