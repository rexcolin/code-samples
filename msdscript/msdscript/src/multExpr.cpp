#include "expr.h"
#include "multExpr.h"
#include "val.h"
#include "catch.h"
#include "numExpr.h"
#include "numVal.h"
#include "varExpr.h"
#include "env.h"
#include "step.h"
#include <string>
#include <iostream>

/*
 * **************************************
 * Methods for the MultExpr class
 * **************************************
 * */

/* constructor */
MultExpr::MultExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
	this->lhs = lhs;
	this->rhs = rhs;
}


/**	NOTES on MultExpr::equals()
 * 	Uses std::dynamic_cast (in the CAST() macro, see pointer.h) to cast the 
 * 	argument to an MultExpr, see if it was the same class, then compare each 
 * 	member variable. This does not interpret the expression.
 * 
 * 	Arg:	Expr pointer
 * 	Return:	bool
 * */
bool MultExpr::equals(PTR(Expr) e) {
	PTR(MultExpr) c = CAST(MultExpr)(e);
	if (c == NULL)
		return false;
	else
		return (this->lhs->equals(c->lhs) && this->rhs->equals(c->rhs));
}


/**	NOTES on AddExpr::interp()
 * 	As interp() returns a Val object, we interp() each member variable, 
 * 	then use the Val class's mult_to() method (see val.h) to combine them 
 * 	both into one Val using addition.
 * 
 *  Arg:    Env pointer
 *  Return: Val pointer
 * */
PTR(Val) MultExpr::interp(PTR(Env) env) {
	return this->lhs->interp(env)->mult_to(rhs->interp(env));
}


/** NOTES on AddExpr::step_interp()
 * 	Same idea as interp() above, but it uses the continuation format 
 * 	so we can perform the operation without using traditional recursion. 
 * 	Does not directly use the Val class's mult_to() method, but passes 
 * 	info on to the next step in the continuation loop, which in this case 
 * 	is RightThenMultCont.
 * 
 * 	Arg:	none		(values are stored in the global variables 
 * 						in the Step class)
 * 	Return:	none
 * */
void MultExpr::step_interp() {
	Step::mode = Step::interp_mode;
	Step::expr = lhs;
	Step::env = Step::env;		/* no-op */
	Step::cont = NEW(RightThenMultCont)(rhs, Step::env, Step::cont);
}


/**	NOTES on MultExpr::print()
 * 	Uses a std::ostream reference to print the member variables separated 
 * 	by a plus sign (with a space on either side of the multiply sign) and 
 * 	the whole AddExpr surrounded by parentheses, and no space between the 
 * 	member variables and the parentheses.
 * 		Example:
 * 				(1 * 1)
 * 
 * 	Arg:     std::osream reference
 * 	Return:  none
 * */
void MultExpr::print(std::ostream& stream) {
	stream << "(";
	this->lhs->print(stream);
	stream << " * ";
	this->rhs->print(stream);
	stream << ")";
}


/** NOTES on MultExpr::pretty_print_at()
 * 	Checks the print mode, but then currently does the exact same thing as 
 * 	MultExpr::print().
 * 
 *	Args:	print_mode_t	(see expr.h)
 *			std::ostream reference
 *	Return:	none
 * */
void MultExpr::pretty_print_at(print_mode_t mode, std::ostream& stream) {
	if (mode >= print_group_AddExpr_or_mult) {
			stream << "(";
	}
	this->lhs->pretty_print_at(print_group_AddExpr_or_mult, stream);
	stream << " * ";
	this->rhs->pretty_print_at(print_group_AddExpr, stream);
	if (mode >= print_group_AddExpr_or_mult) {
		stream << ")";
	}
}



/*
 * *************************************************************************
 * TESTS for MultExpr
 * *************************************************************************
 * */

TEST_CASE("MultExpr equals") {
	CHECK( NEW(MultExpr)( NEW(NumExpr)(2), NEW(NumExpr)(2) )
			->equals( NEW(MultExpr)( NEW(NumExpr)(2), NEW(NumExpr)(2) ) )
			== true );
	CHECK( NEW(MultExpr)( NEW(NumExpr)(2), NEW(NumExpr)(1))
			->equals( NEW(MultExpr)( NEW(NumExpr)(2), NEW(NumExpr)(1) ) )
			== true );
	CHECK( NEW(MultExpr)( NEW(NumExpr)(2), NEW(NumExpr)(1))
			->equals( NEW(MultExpr)( NEW(NumExpr)(1), NEW(NumExpr)(2) ) )
			== false );
}


TEST_CASE("MultExpr interp") {
	CHECK( NEW(MultExpr)( NEW(NumExpr)(1), NEW(NumExpr)(2) )
			->interp(Env::empty)->equals( NEW(NumVal)(2) ) );
	CHECK( NEW(MultExpr)( NEW(NumExpr)(2), NEW(NumExpr)(3) )
			->interp(Env::empty)->equals( NEW(NumVal)(6))
			!= false );
}


TEST_CASE("MultExpr print") {
	PTR(NumExpr) one = NEW(NumExpr)(1);
	PTR(NumExpr) two = NEW(NumExpr)(2);
	PTR(MultExpr) test = NEW(MultExpr)(one, two);
	CHECK(test->to_string() == "(1 * 2)");
}