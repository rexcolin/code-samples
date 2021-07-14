#include "addExpr.h"
#include "expr.h"
#include "val.h"
#include "catch.h"
#include "numExpr.h"
#include "numVal.h"
#include "varExpr.h"
#include "env.h"
#include "step.h"
#include "cont.h"
#include <string>

/*
 * **************************************
 * Methods for the AddExpr class
 * **************************************
 * */


/* constructor */
AddExpr::AddExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
	this->lhs = lhs;
	this->rhs = rhs;
}


/**	NOTES on AddExpr::equals()
 * 	Uses std::dynamic_cast (in the CAST() macro, see pointer.h) to cast the 
 * 	argument to an AddExpr, see if it was the same class, then compare each 
 * 	member variable. This does not interpret the expression.
 * 
 * 	Arg:	Expr pointer
 * 	Return:	bool
 * */
bool AddExpr::equals(PTR(Expr) e) {
	PTR(AddExpr) c = CAST(AddExpr)(e);
	if (c == NULL)
		return false;
	else
		return (this->lhs->equals(c->lhs) && this->rhs->equals(c->rhs));
}


/**	NOTES on AddExpr::interp()
 * 	As interp() returns a Val object, we interp() each member variable, 
 * 	then use the Val class's add_to() method (see val.h) to combine them 
 * 	both into one Val using addition.
 * 
 *  Arg:    Env pointer
 *  Return: Val pointer
 * */
PTR(Val) AddExpr::interp(PTR(Env) env) {
	return this->lhs->interp(env)->add_to(rhs->interp(env));
}


/** NOTES on AddExpr::step_interp()
 * 	Same idea as interp() above, but it uses the continuation format 
 * 	so we can perform the operation without using traditional recursion. 
 * 	Does not directly use the Val class's add_to() method, but passes 
 * 	info on to the next step in the continuation loop, which in this case 
 * 	is RightThenAddCont.
 * 
 * 	Arg:	none		(values are stored in the global variables 
 * 						in the Step class)
 * 	Return:	none
 * */
void AddExpr::step_interp() {
	Step::mode = Step::interp_mode;
	Step::expr = lhs;
	Step::env = Step::env;		/* non-operational, so could omit */
	Step::cont = NEW(RightThenAddCont)(rhs, Step::env, Step::cont);
}


/**	NOTES on AddExpr::print()
 * 	Uses a std::ostream reference to print the member variables separated 
 * 	by a plus sign (with a space on either side of the plus sign) and the 
 * 	whole AddExpr surrounded by parentheses, and no space between the 
 * 	member variables and the parentheses.
 * 		Example:
 * 				(1 + 1)
 * 
 * 	Arg:     std::osream reference
 * 	Return:  none
 * */
void AddExpr::print(std::ostream& stream) {
	stream << "(";
	this->lhs->print(stream);
	stream << " + ";
	this->rhs->print(stream);
	stream << ")";
}


/** NOTES on AddExpr::pretty_print_at()
 * 	Checks the print mode, but then currently does the exact same thing as 
 * 	AddExpr::print().
 * 
 *	Args:	print_mode_t	(see expr.h)
 *			std::ostream reference
 *	Return:	none
 * */
void AddExpr::pretty_print_at(print_mode_t mode, std::ostream& stream) {
		if (mode >= print_group_AddExpr) {
			stream << "(";
		}
		this->lhs->pretty_print_at(print_group_AddExpr, stream);
		stream << " + ";
		this->rhs->pretty_print_at(print_group_none, stream);
		if (mode >= print_group_AddExpr) {
			stream << ")";
		}
}




/*
 * **************************************
 * TESTS for AddExpr
 * **************************************
 * */

TEST_CASE("AddExpr equals") {
	CHECK( NEW(AddExpr)( NEW(NumExpr)(2), NEW(NumExpr)(2) )
			->equals( NEW(AddExpr)( NEW(NumExpr)(2), NEW(NumExpr)(2)) )
			== true );
	CHECK( NEW(AddExpr)( NEW(NumExpr)(2), NEW(NumExpr)(1) )
			->equals( NEW(AddExpr)( NEW(NumExpr)(2), NEW(NumExpr)(1)) )
			== true );
	CHECK( NEW(AddExpr)( NEW(NumExpr)(2), NEW(NumExpr)(1))
			->equals( NEW(AddExpr)( NEW(NumExpr)(1), NEW(NumExpr)(2)) )
			== false );
}


TEST_CASE("AddExpr interp") {
	CHECK( NEW(AddExpr)( NEW(NumExpr)(1), NEW(NumExpr)(2) )
			->interp(Env::empty)
			->equals( NEW(NumVal)(3)) );
	CHECK( NEW(AddExpr)( NEW(NumExpr)(2), NEW(NumExpr)(3) )
			->interp(Env::empty)
			->equals( NEW(NumVal)(5) )
			!= false );
}


TEST_CASE("AddExpr print") {
	PTR(NumExpr) one = NEW(NumExpr)(1);
	PTR(NumExpr) two = NEW(NumExpr)(2);
	PTR(AddExpr) test = NEW(AddExpr)(one, two);
	CHECK(test->to_string() == "(1 + 2)");
}
