#include "expr.h"
#include "val.h"
#include "stdio.h"
#include "catch.h"
#include "addExpr.h"
#include "boolExpr.h"
#include "boolVal.h"
#include "eqExpr.h"
#include "letExpr.h"
#include "multExpr.h"
#include "numExpr.h"
#include "numVal.h"
#include "varExpr.h"
#include "env.h"
#include <stdexcept>
#include <iostream>
#include <sstream>


/*
 * **************************************
 * Methods for the Expr class
 * **************************************
 * */


/** NOTES on Expr::to_string()
 * 	Calls print() internally and returns a string;
 * 
 * 	Arg:	none
 * 	Return:	std::string
 * */
std::string Expr::to_string() {
	std::stringstream out("");
	this->print(out);
	return out.str();
}


/** NOTES on Expr::to_string_pretty()
 * 	In theory this would do the same as to_string() above, 
 * 	but it is not fully implemented.
 * 
 * 	Arg:	none
 * 	Return:	std::string
 * */
std::string Expr::to_string_pretty() {
	std::stringstream out("");
	this->pretty_print(out);
	return out.str();
}


/** NOTES on Expr::pretty_print()
 * 	For use in pretty_print mode, which is not implemented.
 * 
 * 	Arg:	none
 * 	Return:	none
 * */
void Expr::pretty_print(std::ostream& stream) {
	pretty_print_at(print_group_none, stream);
}


/** NOTES on Expr::pretty_print_at()
 * 	Used in pretty_print(), which is not implemented.
 * 
 *	Arg:	none
 *	Return:	none
 * */
void Expr::pretty_print_at(print_mode_t mode, std::ostream& stream) {
	print(stream);
}

/*
 * *******************************************
 * TESTS for Expr class
 * *******************************************
 * */

/*
 * pretty_print() tests
 * */

TEST_CASE("Pretty Print AddExpr") {
	std::string expected = "1 + 2";
	PTR(AddExpr) test = NEW(AddExpr)( NEW(NumExpr)(1), NEW(NumExpr)(2) );
	CHECK(test->to_string_pretty() == expected);
}

TEST_CASE("Pretty print AddExpr with MultExpr") {
	std::string expected = "1 + 2 * 3";
	PTR(MultExpr) testMult = NEW(MultExpr)( NEW(NumExpr)(2), NEW(NumExpr)(3) );
	PTR(AddExpr) testAddExpr = NEW(AddExpr)( NEW(NumExpr)(1), testMult);
	CHECK(testAddExpr->to_string_pretty() == expected);
}

TEST_CASE("Pretty print MultExpr with AddExpr") {
	std::string expected = "2 * (1 + 2)";
	PTR(AddExpr) testAddExpr = NEW(AddExpr)( NEW(NumExpr)(1), NEW(NumExpr)(2) );
	PTR(MultExpr) testMult = NEW(MultExpr)( NEW(NumExpr)(2), testAddExpr);
	CHECK(testMult->to_string_pretty() == expected);
}

