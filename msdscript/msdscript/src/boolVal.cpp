#include "boolVal.h"
#include "boolExpr.h"
#include "numVal.h"
#include "catch.h"


/*
 * **************************************
 * Methods for the BoolVal class
 * **************************************
 * */


/* constructor */
BoolVal::BoolVal(bool rep) {
    this->rep = rep;
}


/**	NOTES on BoolVal::equals()
 * 	Uses std::dynamic_cast (in the CAST() macro, see pointer.h) to cast the 
 * 	argument to an BoolVal, see if it was the same class, then compare each 
 * 	member variable.
 * 
 * 	Arg:	Val pointer
 * 	Return:	bool
 * */
bool BoolVal::equals(PTR(Val) v) {
    PTR(BoolVal) c = CAST(BoolVal)(v);
    if (c == NULL)
        return false;
    else
        return (this->rep == c->rep);
}


/** NOTES on BoolVal::add_to()
 *  This method is intended to use with NumVals only, so here it throws 
 *  a std::runtime error.
 * */
PTR(Val) BoolVal::add_to(PTR(Val) v) {
    throw std::runtime_error("You can't add a BoolVal");
}


/** NOTES on BoolVal::mult_to()
 *  This method is intended to use with NumVals only, so here it throws 
 *  a std::runtime error.
 * */
PTR(Val) BoolVal::mult_to(PTR(Val) v) {
    throw std::runtime_error("You can't mult a BoolVal");
}


/** NOTES on BoolVal::to_string()
 *  Depending on the bool stored in BoolVal::rep, returns one of two 
 *  strings:
 * 
 *          _true
 *          _false
 * 
 *  Arg:     none
 *  Return:  std::string
 * */
std::string BoolVal::to_string() {
    if (rep) 
        return "_true";
    else
        return "_false";
}


/** NOTES on BoolVal::call()
 *  This method is intended to use with FunVals only, so here it throws 
 *  a std::runtime error.
 * */
PTR(Val) BoolVal::call(PTR(Val) val) {
    throw std::runtime_error("you can't call a BoolVal");
}


/** NOTES on BoolVal::call_step()
 *  This method is intended to use with FunVals only, so here it throws 
 *  a std::runtime error.
 * */
void BoolVal::call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) {
   throw std::runtime_error("you can't call a BoolVal");
}



/**
 * ***************************************
 * TESTS for BoolVal
 * ***************************************
 * */

TEST_CASE("BoolVal equals()") {

    CHECK( NEW(BoolVal)(true)
            ->equals( NEW(BoolVal)(true) ) );

    CHECK( NEW(BoolVal)(true)
            ->equals( NEW(BoolVal)(false) )
            == false );

    CHECK( NEW(BoolVal)(true)
            ->equals( NEW(NumVal)(1) )
            == false );

}


TEST_CASE("BoolVal toString()") {

    PTR(BoolVal) boolValtrue = NEW(BoolVal)(true);
    PTR(BoolVal) boolValFalse = NEW(BoolVal)(false);

    CHECK( (boolValtrue->to_string()) == "_true" );
    
    CHECK( (boolValFalse->to_string()) == "_false" );

}


TEST_CASE("BoolVal call") {
    PTR(BoolVal) boolValTrue = NEW(BoolVal)(true);
    PTR(NumVal) numVal = NEW(NumVal)(1);

    CHECK_THROWS_WITH( boolValTrue->call(numVal), "you can't call a BoolVal" );
}