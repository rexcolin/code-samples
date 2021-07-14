#include "expr.h"
#include "ifExpr.h"
#include "val.h"
#include "boolVal.h"
#include "catch.h"
#include "eqExpr.h"
#include "numExpr.h"
#include "numVal.h"
#include "varExpr.h"
#include "env.h"
#include "step.h"
#include "cont.h"
#include <string>
#include <iostream>

/*
 * **************************************
 * Methods for the IfExpr class
 * **************************************
 * */


/* constructor */
IfExpr::IfExpr(PTR(Expr) if_side, PTR(Expr) then_side, PTR(Expr) else_side) {

    this->if_side = if_side;
    this->then_side = then_side;
    this->else_side = else_side;
}


/** NOTES on IfExpr::equals()
 * 	Uses std::dynamic_cast (in the CAST() macro, see pointer.h) to cast the 
 * 	argument to a FunExpr, see if it was the same class, then compare each 
 * 	member variable. This does not interpret the expression.
 * 
 * 	Arg:	Expr pointer
 * 	Return:	bool 
 * */
bool IfExpr::equals(PTR(Expr) e) {
    PTR(IfExpr) c = CAST(IfExpr)(e);
    if (c == NULL)
        return false;
    else
        return ( this->if_side->equals(c->if_side)
                 && this->then_side->equals(c->then_side)
                 && this->else_side->equals(c->else_side) );
}


/** NOTES on IfExpr::interp()
 *  Checks to see if the IfExpr::if_side interprets to true. 
 *  If it does, then the IfExpr::then_side is interpreted and 
 *  returned. If not, the the IfExpr::else_side is interpreted 
 *  returned.
 * 
 *  Arg:    Env pointer
 *  Return: Val pointer
 * */
PTR(Val) IfExpr::interp(PTR(Env) env) {
    if (this->if_side->interp(env)->equals( NEW(BoolVal)(true)))
        return this->then_side->interp(env);
    else
        return this->else_side->interp(env);
}


/** NOTES on IfExpr::step_interp()
 * 	Same idea as interp() above, but it uses the continuation format 
 * 	so we can perform the operation without using traditional recursion. 
 * 	Does not compare against a BoolVal or determine whether to use the 
 *  IfExpr::then_side or IfExpr::else_side, it passes that to the next 
 *  step in the continuation loop, which in this case is IfBranchCont.
 * 
 * 	Arg:	none		(values are stored in the global variables 
 * 						in the Step class)
 * 	Return:	none
 * */
void IfExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = if_side;
    Step::env = Step::env;
    Step::cont = NEW(IfBranchCont)(then_side, else_side, Step::env, Step::cont);
}


/** NOTES on IfExpr::print()
 *  Uses a std::ostream reference to print the whole expression in parentheses, 
 *  with each member variable preceded by an underscore, its respective keyword, 
 *  and a space; "-if ", "_then ", or "_else ".
 *      Example:
 *          (_if (1 == 1) _then 1 _else 2)
 * 
 * 	Arg:     std::osream reference
 * 	Return:  none
 * */
void IfExpr::print(std::ostream& stream) {
    stream << "(";
    stream << "_if ";
    this->if_side->print(stream);
    stream << " _then ";
    this->then_side->print(stream);
    stream << " _else ";
    this->else_side->print(stream);
    stream << ")";
}


/** NOTES on IfExpr::pretty_print_at()
 *  Not implemented, currently just calls print().
 * 
 *  Arg:    std::ostream reference
 *  Return: none
 * */
void IfExpr::pretty_print_at(print_mode_t mode, std::ostream& stream) {
    this->print(stream);
}




/*
 * *************************************************************************
 * TESTS for IfExpr
 * *************************************************************************
 * */

TEST_CASE("IfExpr equals()") {

    PTR(NumExpr) numExprOne = NEW(NumExpr)(1);
    PTR(NumExpr) numExprTwo = NEW(NumExpr)(2);
    PTR(EqExpr) eqExprTrue = NEW(EqExpr)(numExprOne, numExprOne);
    PTR(EqExpr) eqExprFalse = NEW(EqExpr)(numExprOne, numExprTwo);
    PTR(IfExpr) goesToThen = NEW(IfExpr)(eqExprTrue, numExprOne, numExprTwo);
    PTR(IfExpr) goesToElse = NEW(IfExpr)(eqExprFalse, numExprOne, numExprTwo);
    
    CHECK( goesToThen->equals( NEW(IfExpr)(eqExprTrue, numExprOne, numExprTwo)) );
    
    CHECK( goesToElse->equals( NEW(IfExpr)(eqExprTrue, numExprOne, numExprTwo)) == false );
    
    CHECK( goesToThen->equals( NEW(NumExpr)(3)) == false );
}


TEST_CASE("IfExpr interp()") {

    PTR(NumExpr) numExprOne = NEW(NumExpr)(1);
    PTR(NumExpr) numExprTwo = NEW(NumExpr)(2);
    PTR(NumVal) numValOne = NEW(NumVal)(1);
    PTR(NumVal) numValTwo = NEW(NumVal)(2);

    PTR(EqExpr) eqExprTrue = NEW(EqExpr)(numExprOne, numExprOne);
    PTR(IfExpr) goesToThen = NEW(IfExpr)(eqExprTrue, numExprOne, numExprTwo);

    CHECK( goesToThen->interp(Env::empty)->equals(numValOne) );

    CHECK( goesToThen->interp(Env::empty)->equals(numValTwo) == false );

}


TEST_CASE("IfExpr print()") {

    PTR(NumExpr) numExprOne = NEW(NumExpr)(1);
    PTR(NumExpr) numExprTwo = NEW(NumExpr)(2);
    PTR(VarExpr) varExprA = NEW(VarExpr)("A");
    PTR(EqExpr) eqExprTrue = NEW(EqExpr)(numExprOne, numExprOne);

    PTR(IfExpr) ifExprNoVar = NEW(IfExpr)(eqExprTrue, numExprOne, numExprOne);
    std::string noVarExpected = "(_if (1 == 1) _then 1 _else 1)";
    CHECK( ifExprNoVar->to_string() == noVarExpected );

    PTR(IfExpr) ifExprVarInIf = NEW(IfExpr)(varExprA, numExprOne, numExprOne);
    std::string varInIfExpected = "(_if A _then 1 _else 1)";
    CHECK( ifExprVarInIf->to_string() == varInIfExpected );

    PTR(IfExpr) ifExprVarInThen = NEW(IfExpr)(eqExprTrue, varExprA, numExprOne);
    std::string varInThenExpected = "(_if (1 == 1) _then A _else 1)";
    CHECK( ifExprVarInThen->to_string() == varInThenExpected );

    PTR(IfExpr) ifExprVarInElse = NEW(IfExpr)(eqExprTrue, numExprOne, varExprA);
    std::string varInElseExpected = "(_if (1 == 1) _then 1 _else A)";
    CHECK( ifExprVarInElse->to_string() == varInElseExpected );

}
