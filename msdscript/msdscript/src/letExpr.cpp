#include "expr.h"
#include "letExpr.h"
#include "val.h"
#include "catch.h"
#include "addExpr.h"
#include "numExpr.h"
#include "varExpr.h"
#include "env.h"
#include "numVal.h"
#include "step.h"
#include "cont.h"

#include <string>
#include <iostream>

/*
 * **************************************
 * Methods for the LetExpr class
 * **************************************
 * */


/* constructor */
LetExpr::LetExpr(std::string lhs, PTR(Expr) rhs, PTR(Expr) body) {
	this->lhs = lhs;
	this->rhs = rhs;
	this->body = body;
}


/** NOTES on LetExpr::equals()
 * 	Uses std::dynamic_cast (in the CAST() macro, see pointer.h) to cast the 
 * 	argument to a FunExpr, see if it was the same class, then compare each 
 * 	member variable, which in this case is a std::string and two Exprs. This 
 * 	does not interpret the expression.
 * 
 * 	Arg:	Expr pointer
 * 	Return:	bool 
 * */
bool LetExpr::equals(PTR(Expr) e) {
	PTR(LetExpr) c = CAST(LetExpr)(e);
	if (c == NULL)
		return false;
	else
		return ( this->lhs == c->lhs
		&& this->rhs->equals(c->rhs)
		&& this->body->equals(c->body) );
}


/** NOTES on LetExpr::interp()
 * 	This uses an environment to interpret the LetExpr::rhs 
 * 	Expr, then uses that Val along with LetExpr::lhs to make 
 * 	a new ExtendedEnv, which is used to interpret the 
 * 	LetExpr::body Expr and return the Val that is produced. 
 * 	This is how the lhs string is replaced with the rhs Expr 
 * 	in the body.
 * 
 *  Arg:    Env pointer
 *  Return: Val pointer
 * */
PTR(Val) LetExpr::interp(PTR(Env) env) {
	PTR(Val) rhs_val = this->rhs->interp(env);
	PTR(Env) new_env = NEW(ExtendedEnv)(this->lhs, rhs_val, env);
	return this->body->interp(new_env);
}


/** NOTES on LetExpr::step_interp()
 * 	Same idea as interp() above, but it uses the continuation format 
 * 	so we can perform the operation without using traditional recursion. 
 * 	It passes a lot of the work done in LetExpr::interp() on to the 
 * 	next step in the continuation loop, which in this case is 
 * 	LetBodyCont.
 * 
 * 	Arg:	none		(values are stored in the global variables 
 * 						in the Step class)
 * 	Return:	none
 * */
void LetExpr::step_interp() {
	Step::mode = Step::interp_mode;
	Step::expr = rhs;
	Step::env = Step::env;
	Step::cont = NEW(LetBodyCont)(lhs, body, Step::env, Step::cont);
}


/** NOTES on LetExpr::print()
 * 	Uses a std::ostream reference to print the whole expression in 
 * 	parentheses, with the lhs and body preceded by an underscore and 
 * 	its respective keyword and a space, and the rhs preceded by a 
 * 	space, an equals sign, and a space.
 * 		Example:
 * 			(_let x = 1 _in (x + 1))
 * 
 * 	LetExpr::body will be printed with parentheses around it as well, 
 * 	but that happens when print is called on AddExpr, not here.
 *  
 * 	Arg:     std::osream reference
 * 	Return:  none
 * */
void LetExpr::print(std::ostream& stream) {
	stream << "(_let ";
	stream << this->lhs;
	stream << " = ";
	this->rhs->print(stream);
	stream << " _in ";
	this->body->print(stream);
	stream << ")";
}


/** NOTES on LetExpr::pretty_print_at()
 *  Not implemented, currently just calls print().
 * 
 *  Arg:    std::ostream reference
 *  Return: none
 * */
void LetExpr::pretty_print_at(print_mode_t mode, std::ostream& stream) {
	this->print(stream);
}





/**
 * **************************************
 * TESTS for LetExpr
 * **************************************
 * */

TEST_CASE("LetExpr equals()") {
	
	PTR(NumExpr) one = NEW(NumExpr)(1);
	PTR(VarExpr) varX = NEW (VarExpr)("x");
	PTR(AddExpr) xPlusOne = NEW(AddExpr)( varX, one );
	PTR(LetExpr) let1 = NEW(LetExpr)( "x", one, xPlusOne );

	CHECK( let1->equals( NEW(LetExpr)( "x", one, xPlusOne ) ) );
	CHECK( let1->equals( NEW(LetExpr)( "y", one, xPlusOne ) ) == false );

}


TEST_CASE("LetExpr interp()") {

	PTR(NumExpr) one = NEW(NumExpr)(1);
	PTR(VarExpr) varX = NEW (VarExpr)("x");
	PTR(AddExpr) xPlusOne = NEW(AddExpr)( varX, one );
	PTR(LetExpr) let1 = NEW(LetExpr)( "x", one, xPlusOne );

	CHECK( let1->interp(Env::empty)->equals( NEW(NumVal)(2) ) );
	CHECK( let1->interp(Env::empty)->equals( NEW(NumVal)(3) ) == false );

}


TEST_CASE("LetExpr print()") {

	PTR(NumExpr) one = NEW(NumExpr)(1);
	PTR(VarExpr) varX = NEW (VarExpr)("x");
	PTR(AddExpr) xPlusOne = NEW(AddExpr)( varX, one );
	PTR(LetExpr) let1 = NEW(LetExpr)( "x", one, xPlusOne );

	CHECK( let1->to_string() == "(_let x = 1 _in (x + 1))");
}


// TEST_CASE("more subst") {

	// LetExpr x = 1
	// _in  x + 2  -> subst("x", y+3)
	// =
	// LetExpr x = 1
	// _in  x + 2
	// PTR(Expr) let0 = NEW(LetExpr)( "x",
	// 					NEW(NumExpr)(1),
	// 					NEW(AddExpr)( NEW(VarExpr)("x"), NEW(NumExpr)(2)) );
	// PTR(Expr) let1 = NEW(LetExpr)( "x",
	// 					NEW(NumExpr)(1),
	// 					NEW(AddExpr)( NEW(VarExpr)("x"), NEW(NumExpr)(2)) );
	// CHECK( let0->subst( "x", NEW(AddExpr)( NEW(VarExpr)("y"), NEW(NumExpr)(3)) )
	// 		->equals(let1) );

	// LetExpr x = x
	// _in  x + 2  -> subst("x", y+3)
	// =
	// LetExpr x = y+3
	// _in  x + 2
	// PTR(Expr) let2 = ( NEW(LetExpr)( "x",
	// 						NEW(VarExpr)("x"),
	// 						NEW(AddExpr)( NEW(VarExpr)("x"), NEW(NumExpr)(2))) );
	// PTR(Expr) let20 = NEW(LetExpr)( "x",
	// 						NEW(AddExpr)( NEW(VarExpr)("y"), NEW(NumExpr)(3) ),
	// 						NEW(AddExpr)( NEW(VarExpr)("x"), NEW(NumExpr)(2)) );
	// CHECK( let2->subst("x", NEW(AddExpr)( NEW(VarExpr)("y"), NEW(NumExpr)(3) ))
	// 		->equals(let20) );
  
	// LetExpr x = y
	// _in  x + 2 -> subst("y", 8)
	// =
	// LetExpr x = 8
	// _in  x + 2
	// PTR(Expr) let3 = NEW(LetExpr)( "x",
	// 					NEW(VarExpr)("y"), 
	// 					NEW(AddExpr)( NEW(VarExpr)("x"), NEW(NumExpr)(2) ));
	// PTR(Expr) let4 = NEW(LetExpr)( "x",
	// 					NEW(NumExpr)(8), 
	// 					NEW(AddExpr)( NEW(VarExpr)("x"), NEW(NumExpr)(2) ));
	//CHECK( let3->subst("y", NEW(NumExpr)(8))->equals(let4) );
  
	// LetExpr x = 8
	// _in  x + 2 + y -> subst("y", 9)
	// =
	// LetExpr x = 8
	// _in  x + 2 + 9
	// PTR(Expr) let5 = NEW(LetExpr)( "x",
	// 					NEW(NumExpr)(8),
	// 					NEW(AddExpr)( NEW(VarExpr)("x"), 
	// 							NEW(AddExpr)( NEW(NumExpr)(2), NEW(VarExpr)("y") )) );
	// PTR(Expr) let6 = NEW(LetExpr)( "x",
	// 					NEW(NumExpr)(8),
	// 					NEW(AddExpr)( NEW(VarExpr)("x"), 
	// 							NEW(AddExpr)( NEW(NumExpr)(2), NEW(NumExpr)(9) )) );
	//CHECK( let5->subst("y", NEW(NumExpr)(9))->equals(let6) );

  // y + y -> subst("y", 8)
  // =
  // 8 + 8
	// PTR(Expr) AddExpr1 = NEW(AddExpr)( NEW(VarExpr)("y"),
	// 					NEW(VarExpr)("y") );
	// PTR(Expr) AddExpr2 = NEW(AddExpr)( NEW(NumExpr)(8),
	// 					NEW(NumExpr)(8) );
	//CHECK( AddExpr1->subst("y", NEW(NumExpr)(8))->equals(AddExpr2) );

  // LetExpr x = y
  // _in  x + y -> subst("y", 8)
  // =
  // LetExpr x = 8
  // _in  x + 8
	// PTR(Expr) let7 = NEW(LetExpr)( "x",
	// 					NEW(VarExpr)("y"),
	// 					NEW(AddExpr)( NEW(VarExpr)("x"), NEW(VarExpr)("y")) );
	// PTR(Expr) let8 = NEW(LetExpr)( "x",
	// 					NEW(NumExpr)(8),
	// 					NEW(AddExpr)( NEW(VarExpr)("x"), NEW(NumExpr)(8) ) );
	//CHECK( let7->subst("y", NEW(NumExpr)(8))->equals(let8) );

  // LetExpr z = x
  // _in  z + 32 -> subst("z", 0)
  // =
  // LetExpr z = x
  // _in  z + 32
	// PTR(Expr) let9 = NEW(LetExpr)( "z",
	// 					NEW(VarExpr)("x"),
	// 					NEW(AddExpr)( NEW(VarExpr)("z"), NEW(NumExpr)(32) ) );
	// PTR(Expr) let10 = NEW(LetExpr)( "z",
	// 					NEW(VarExpr)("x"),
	// 					NEW(AddExpr)( NEW(VarExpr)("z"), NEW(NumExpr)(32)) );
	//CHECK( let9->subst( "z", NEW(NumExpr)(0) )->equals(let10) );
  
  // LetExpr z = z
  // _in  z + 32 -> subst("z", 0)
  // =
  // LetExpr z = 0
  // _in  z + 32
	// PTR(Expr) let11 = NEW(LetExpr)( "z",
	// 					NEW(VarExpr)("z"),
	// 					NEW(AddExpr)( NEW(VarExpr)("z"), NEW(NumExpr)(32) ) );
	// PTR(Expr) let12 = NEW(LetExpr)( "z",
	// 					NEW(NumExpr)(0),
	// 					NEW(AddExpr)( NEW(VarExpr)("z"), NEW(NumExpr)(32) ) );
	//CHECK( let11->subst( "z", NEW(NumExpr)(0) )->equals(let12) );

  // LetExpr z = z + 2
  // _in  z + 32 -> subst("z", 0)
  // =
  // LetExpr z = 0 + 2
  // _in  z + 32
	// PTR(Expr) let13 = NEW(LetExpr)( "z",
	// 					NEW(AddExpr)( NEW(VarExpr)("z"), NEW(NumExpr)(2) ),
	// 					NEW(AddExpr)( NEW(VarExpr)("z"), NEW(NumExpr)(32)) );
	// PTR(Expr) let14 = NEW(LetExpr)( "z",
	// 					NEW(AddExpr)( NEW(NumExpr)(0), NEW(NumExpr)(2) ),
	// 					NEW(AddExpr)( NEW(VarExpr)("z"), NEW(NumExpr)(32)) );
	//CHECK( let13->subst( "z", NEW(NumExpr)(0) )->equals(let14) );

// }