#include "numVal.h"
#include "numExpr.h"
#include "catch.h"
#include "boolVal.h"
#include "funVal.h"
#include "addExpr.h"
#include "varExpr.h"
#include <string>


/*
 * **************************************
 * Methods for the BoolVal class
 * **************************************
 * */


/* constructor */
NumVal::NumVal(int rep) {
    this->rep = rep;
}


/**	NOTES on NumVal::equals()
 * 	Uses std::dynamic_cast (in the CAST() macro, see pointer.h) to cast the 
 * 	argument to an NumVal, see if it was the same class, then compare each 
 * 	member variable.
 * 
 * 	Arg:	Val pointer
 * 	Return:	bool
 * */
bool NumVal::equals(PTR(Val) v) {
    PTR(NumVal) c = CAST(NumVal)(v);
    if (c == NULL)
        return false;
    else
        return (this->rep == c->rep);
}


/** NOTES on NumVal::add_to()
 *  Method to add one NumVal to another. Uses a similar technique as the 
 *  equals() methods, where it casts the input to a NumVal, but in this 
 *  case throws an error if the argument Val was not a NumVal. It then 
 *  returns a new NumVal where the NumVal::rep is the sum of the two 
 *  NumVals' reps.
 * 
 *  Arg:    Val pointer
 *  Return: Val pointer
 * */
PTR(Val) NumVal::add_to(PTR(Val) v) {
    PTR(NumVal) c = CAST(NumVal)(v);
    if (c == NULL)
        throw std::runtime_error("not valid input for add_to in NumVal class");
    else
        return NEW(NumVal)((unsigned)this->rep + (unsigned)c->rep);
}


/** NOTES on NumVal::mult_to()
 *  Method to multiply ove NumVal with another. Uses a similar technique 
 *  as the equals() methods, where it casts the input to a NumVal, but in 
 *  this case throws an error if the argument Val was not a NumVal. It 
 *  then returns a new NumVal where the NumVal::rep is the product of the 
 *  two NumVals' reps.
 * 
 *  Arg:    Val pointer
 *  Return: Val pointer
 * */
PTR(Val) NumVal::mult_to(PTR(Val) v) {
    PTR(NumVal) c = CAST(NumVal)(v);
    if (c == NULL)
        throw std::runtime_error("not valid input for mult_to in NumVal class");
    else
        return NEW(NumVal)((unsigned)this->rep * (unsigned)c->rep);
}


/** NOTES on NumVal::to_string()
 *  Method that returns the int stored in NumVal::rep as a std::string.
 * 
 *  Arg:    none
 *  Return: std::string
 * */
std::string NumVal::to_string() {
    return std::to_string(rep);
}


/** NOTES on NumVal::call()
 *  This method is intended to use with FunVals only, so here it throws 
 *  a std::runtime error.
 * */
PTR(Val) NumVal::call(PTR(Val) val) {
    throw std::runtime_error("you can't call a NumVal");
}


/** NOTES on NumVal::call()
 *  This method is intended to use with FunVals only, so here it throws 
 *  a std::runtime error.
 * */
void NumVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) {
   throw std::runtime_error("you can't call a NumVal");
}



/*
 * *************************************************************************
 * TESTS for NumVal
 * *************************************************************************
 * */

TEST_CASE("NumVal equals") {

	CHECK( NEW(NumVal)(1)->equals( NEW(NumVal)(1) ) == true );
	CHECK( NEW(NumVal)(1)->equals( NEW(NumVal)(2) ) == false );
    CHECK( NEW(NumVal)(1)->equals( NEW(BoolVal)(true) ) == false );

}


TEST_CASE("NumVal add_to") {

    CHECK( NEW(NumVal)(1)->add_to( NEW(NumVal)(1) )->equals( NEW(NumVal)(2) ) );
    CHECK( NEW(NumVal)(1)->add_to( NEW(NumVal)(1) )->equals( NEW(NumVal)(3) ) == false );

}


TEST_CASE("NumVal mult_to") {

    CHECK( NEW(NumVal)(2)->mult_to( NEW(NumVal)(2) )->equals( NEW(NumVal)(4) ) );
    CHECK( NEW(NumVal)(1)->mult_to( NEW(NumVal)(1) )->equals( NEW(NumVal)(3) ) == false );

}


TEST_CASE("NumVal toString") {

    CHECK( NEW(NumVal)(1)->to_string() == "1" );
    CHECK( NEW(NumVal)(2)->to_string() != "1" );

}


TEST_CASE("NumVal call()") {

    PTR(NumVal) num = NEW(NumVal)(1);
    PTR(BoolVal) boolValTrue = NEW(BoolVal)(true);

    CHECK_THROWS_WITH( num->call(boolValTrue), "you can't call a NumVal");
}