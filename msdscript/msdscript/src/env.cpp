#include "env.h"
#include "numVal.h"
#include "catch.h"


/* definition of Env::empty */
PTR(Env) Env::empty = NEW(EmptyEnv)();



/*
 * **************************************
 * Methods for the EmptyEnv class
 * **************************************
 * */

/** NOTES on EmptyEnv::lookup()
 *  There is no member variable to search, so instead this 
 *  method throws an error, because if the lookup() method 
 *  reaches an EmptyEnv, that means it got to the end of the 
 *  list and the variable wasn't in the list.
 * */
PTR(Val) EmptyEnv::lookup(std::string find_name) {
    throw std::runtime_error("free variable: " + find_name);
}



/*
 * **************************************
 * Methods for the ExtendedEnv class
 * **************************************
 * */

/* constructor */
ExtendedEnv::ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest) {
    this->name = name;
    this->val = val;
    this->rest = rest;    
}


/** NOTES on ExtendedEnv::lookup()
 *  This is the method that searches the list for a variable 
 *  given in the arguments and either returns the Val 
 *  associated with the same Env if it finds the variable, 
 *  or calls lookup() on the next Env in the list.
 * 
 *  Arg:    std::string
 *  Return: Val pointer
 * */
PTR(Val) ExtendedEnv::lookup(std::string find_name) {
    if (find_name == this->name)
        return this->val;
    else
        return this->rest->lookup(find_name);
}



/**
 * ************************************
 *      TESTS for Env
 * ************************************
 * */

TEST_CASE("EmptyEnv lookup()") {

    CHECK_THROWS_WITH( NEW(EmptyEnv)()->lookup( "x" ), "free variable: x" );

}


TEST_CASE("ExtendedEnv lookup()") {

    PTR(ExtendedEnv) envX = NEW(ExtendedEnv)( "x", NEW(NumVal)(1), Env::empty );
    CHECK( envX->lookup( "x" )->equals( NEW(NumVal)(1) ) );

    PTR(ExtendedEnv) envY = NEW(ExtendedEnv)( "y", NEW(NumVal)(2), envX );
    CHECK( envY->lookup( "y" )->equals( NEW(NumVal)(2) ) );
    CHECK( envY->lookup( "x" )->equals( NEW(NumVal)(1) ) );
    CHECK_THROWS_WITH( envY->lookup( "q" ), "free variable: q" );

}