#include "expr.h"
#include "parse.h"
#include "addExpr.h"
#include "boolExpr.h"
#include "callExpr.h"
#include "eqExpr.h"
#include "ifExpr.h"
#include "letExpr.h"
#include "multExpr.h"
#include "numExpr.h"
#include "val.h"
#include "numVal.h"
#include "varExpr.h"
#include "funExpr.h"
#include "pointer.h"
#include "env.h"

#include "catch.h"

#include <iostream>
#include <sstream>
#include <string>



/**
 * ------------------------- Consume method ------------------------------------------
 * 
 *  Consumes one char from the std::instream, 
 *  then compares it to what is expected. If 
 *  it is different, it throws an error.
 * 
 *  Arg:    std::istream
 *          int
 *  Return: none
 * */
void consume(std::istream &in, int expect) {
    int c = in.get();
    if (c != expect)
        throw std::runtime_error("consume mismatch");
}


/**
 * ------------------------- Skip_whitespace method ------------------------------------
 *  
 *  While there is whitespace in the std::instream 
 *  provided, this skips it. It does this by peeking 
 *  at the next char, and only consuming it if it is 
 *  a space.
 * 
 *  Arg:    std::istream
 *  Return: none
 * */
void skip_whitespace(std::istream &in) {
    while (1) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}


/**
 * 
 * 
 * 
 * ************************************************************************
 * 
 *                          methods to actually call in cmdline or tests
 * 
 * ************************************************************************
 * 
 * 
 * */


/**
 * ------------------------- parse method ------------------------------------------
 *  This function is used to call the series of 
 *  different parsing functions, starting with 
 *  parse_comparg().
 * 
 *  Arg:    std::istream
 *  Return: Expr pointer
 * */
PTR(Expr) parse(std::istream &in) {
    //std::cout << "in parse()\n";
    skip_whitespace(in);
    
    return parse_comparg(in);
}


/**
 * ------------------------- parse_string method ----------------------------------------
 *  Does the same thing as parse() in calling 
 *  parse_comparg(), but makes an std::istream 
 *  from a string. This method is used for 
 *  testing, not actually running msdscript.
 * 
 *  Arg:    std::string
 *  Return: Expr pointer
 * */
PTR(Expr) parse_string(std::string s) {
    std::istringstream stream (s);
    return parse_comparg(stream);
}



/**
 * 
 * 
 * 
 * ************************************************************************
 * 
 *                          methods called within parse()
 * 
 * ************************************************************************
 * 
 * 
 * */

/**
 * ------------------------- parse_expr method ------------------------------------------
 *  Calls parse_comparg() on the left side, skips 
 *  whitespace, checks for the double equals sign, 
 *  if it is there calls parse_expr on the right 
 *  side and returns a new EqExpr built with those 
 *  pieces, or else returns what it got back from 
 *  the first parse_comparg()
 * 
 *  Arg:    std::istream
 *  Return: Expr pointer
 * */
PTR(Expr) parse_expr(std::istream &in) {

    PTR(Expr) e;
    e = parse_comparg(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '=') {
        consume(in, '=');
        consume(in, '=');
        PTR(Expr) rhs = parse_expr(in);
        return NEW(EqExpr)(e, rhs);
    } else
        return e;
}


/**
 * ------------------------- parse_comparg method ---------------------------------------
 *  Calls parse_addend on the left side, skips 
 *  whitespace, then looks for a plus sign. If 
 *  there is one, calls parse_comparg on the 
 *  right side and returns an AddExpr built from 
 *  those pieces. If not, returns what it got 
 *  from the first parse_addend().
 * 
 *  Arg:    std::istream
 *  Return: Expr pointer
 * */
PTR(Expr) parse_comparg(std::istream &in) {
    
    PTR(Expr) e;

    e = parse_addend(in);
    
    skip_whitespace(in);

    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        PTR(Expr) rhs = parse_comparg(in);
        return NEW(AddExpr)(e, rhs);
    } else return e;
}


/**
 * ------------------------- parse_addend method ----------------------------------------
 *  Calls parse_multicand() o the left side, 
 * skips whitespace, and then checks for a 
 * multiplication sign. If there is one, calls 
 * parse_addend() on the right side and 
 * returns a MultExpr built from those pieces. 
 * If not, returns what it got from the first 
 * parse_multicand().
 * 
 *  Arg:    std::istream
 *  Return: Expr pointer
 * */
PTR(Expr) parse_addend(std::istream &in) {
    
    PTR(Expr) e;
	e = parse_mulitcand(in);

	skip_whitespace(in);

    int c = in.peek();
    if (c == '*') {
		consume(in, '*');
		PTR(Expr) rhs = parse_addend(in);
		return NEW(MultExpr)(e, rhs);
	} else {
        return e;
    }
}


/**
 * ------------------------- parse_multicand method -------------------------------------
 *  Calls parse_inner() on the left side, 
 *  checks for opening parenthesis. If there 
 *  is one, consumes it and calls parse_expr() 
 *  on what is in the parentheses, then uses 
 *  those pieces to build and return a CallExpr. 
 *  If not, returns what it got from the first 
 *  parse_inner().
 * 
 *  Arg:    std::istream
 *  Return: Expr pointer
 * */
PTR(Expr) parse_mulitcand(std::istream &in) {

    skip_whitespace(in);

    PTR(Expr) expr = parse_inner(in);

    while (in.peek() == '(') {
        consume(in, '(');
        PTR(Expr) actual_arg = parse_expr(in);
        consume(in, ')');
        expr = NEW(CallExpr)( expr, actual_arg );
    }

    return expr;

}



/**
 * ------------------------- parse_inner method -----------------------------------------
 *  If there is a negative sign or a number, returns what it gets from parse_num().
 *  If there are parentheses, returns what it gets from parse_expr().
 *  If there is a letter, returns what it gets from parse_var().
 *  If there is an underscore, returns what it gets from the appropriate keyword-needed
 *      class, whether its a let statement from parse_let(), a BoolExpr, an if statement 
 *      from parse_let(), or a function from parse_fun().
 * 
 *  Arg:    std::istream
 *  Return: Expr pointer
 * */
PTR(Expr) parse_inner(std::istream &in) {
    
    skip_whitespace(in);

    int c = in.peek();
    if ((c == '-') || isdigit(c))
        return parse_num(in);
    else if (c == '(') {
        consume(in, '(');
        PTR(Expr) e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')')
            throw std::runtime_error("missing close parenthesis");
        return e;
    } else if (isalpha(c)){
		return parse_var(in);
	} else if (c == '_') {
        std::string keyword = parse_keyword(in);

        if (keyword == "_let") {
            return parse_let(in);
        }
        else if (keyword == "_true") {
            return NEW(BoolExpr)(true);
        }
        else if (keyword == "_false") {
            return NEW(BoolExpr)(false);
        }
        else if (keyword == "_if") {
            return parse_if(in);
        }
        else if (keyword == "_fun") {
            return parse_fun(in);
        }
        else {
            throw std::runtime_error("unexpected char in parse_multicand");
        }
    } else {
        consume(in, c);
        throw std::runtime_error("invalid input");
    }
}


/**
 * ------------------------- parse_num method ------------------------------------------
 *  Checks for a negative sign, and then consumes 
 *  the number, checks for unsigned int overflow, 
 *  adjusts if there was a negative sign, and 
 *  builds and returns a new NumExpr.
 * 
 *  Arg:    std::istream
 *  Return: Expr pointer
 * */
PTR(Expr) parse_num(std::istream &in) {

    int n = 0;
    bool negative = false;

    if (in.peek() == '-') {
        negative = true;
        consume(in, '-');
    }

    while (1) {
        int c = in.peek();
        if (isdigit(c)) {
            consume(in, c);
            n = (unsigned)n*10 + (c - '0');
            if (n < 0) {
                std::runtime_error("unsigned int overflow!");
            }

        } else
            break;
    }

    if (negative)
        n = -n;

    return NEW(NumExpr)(n);
}


/**
 * ------------------------- parse_var() method -----------------------------------------
 *  Checks for a letter, while there is one 
 *  it adds it to the string, then builds 
 *  and returns a VarExpr with that string.
 * 
 *  Arg:    std::istream
 *  Return: Expr pointer 
 * */
PTR(Expr) parse_var(std::istream &in) {

    std::string n = "";

    while (1) {
        char c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            n += c;
        } else
            break;
    }
    return NEW(VarExpr)(n);
}


/**
 * ------------------------- parse_let() method ----------------------------------------
 *  Already parsed the first keyword in parse_inner(), 
 *  and so we look for a string on the left side. If 
 *  there is one, we skip whitespace, check for an 
 *  equals sign, consume it, call parse_expr() on the 
 *  right side, check for the keyword "_in", parse the 
 *  body, and then return an IfExpr built with those 
 *  pieces. All pieces must be present, or errors will 
 *  be thrown.
 * 
 *  Arg:    std::istream
 *  Return: Expr pointer
 * */
PTR(Expr) parse_let(std::istream &in) {

    std::string lhs = "";
    PTR(Expr) rhs;
    PTR(Expr) body;
    
    skip_whitespace(in);

    if (isalpha(in.peek())) {
        while(1) {
            char c = in.peek();
            if(isalpha(c)){
                lhs += in.get();
            }
            else {
                break;
            }
        }
    }
    else {
        throw std::runtime_error("parse_let: invalid lhs");
    }

    skip_whitespace(in);

    if (in.peek() != '=') {
        throw std::runtime_error("parse_let: Invalid input; no '=' sign");
    }
    else {
        consume(in, '=');
    }

    skip_whitespace(in);

    rhs = parse_expr(in);

    skip_whitespace(in);

    check_keyword(in, "_in");

    skip_whitespace(in);

    body = parse_expr(in);

    return NEW(LetExpr)(lhs, rhs, body);
    
}


/**
 * ------------------------- parse_if method ------------------------------------------
 *  Calls parse_expr() on the if side, checks for 
 * the "_then" keyword, calls parse_expr() on the 
 * then side, checks for the "_else" keyword, 
 * calls parse_expr() on the else side, and builds 
 * and returns an IfExpr built with those pieces.
 * 
 *  Arg:    std::istream
 *  Return: Expr pointer
 * */
PTR(Expr) parse_if(std::istream &in) {
    
    skip_whitespace(in);

    PTR(Expr) ifSide = parse_expr(in);

    skip_whitespace(in);

    // check_keyword checks the string to see if it matches the input
    check_keyword(in, "_then");
    PTR(Expr) thenSide = parse_expr(in);
    check_keyword(in, "_else");
    PTR(Expr) elseSide = parse_expr(in);

    return NEW(IfExpr)(ifSide, thenSide, elseSide);

}


/**
 * ------------------------- parse_fun() method -----------------------------------------
 *  Consumes the open parenthesis, calls 
 *  parse_str_helper() on the formal arg, consumes 
 *  the close parenthesis, then calls parse_expr() 
 *  on the body, and returns a new FunExpr built 
 *  with those pieces.
 * 
 *  Arg:    std::istream
 *  Return: Expr pointer
 * */
PTR(Expr) parse_fun(std::istream &in) {

    skip_whitespace(in);

    consume(in, '(');

    skip_whitespace(in);

    std::string arg = parse_str_helper(in);

    skip_whitespace(in);

    consume(in, ')');

    PTR(Expr) body = parse_expr(in);

    return NEW(FunExpr)(arg, body);

}


/**
 * ------------------------- parse_str_helper() method ----------------------------------
 *  
 *  Arg:    std::istream
 *  Return: std::string
 * */
std::string parse_str_helper(std::istream &in) {
    
    skip_whitespace(in);
    std::string ret = "";
    while(true) {
        int c = in.peek();
        if (c == '_') {
            consume(in, '_');
        } else if (isalpha(c)) {
            ret += in.get();
        } else {
            break;
        }
    }
    return ret;
}


/**
 * ------------------------- parse_keyword() method -------------------------------------
 * Consumes and returns a keyword.
 * 
 *  Arg:    std::istream
 *  Return: std::string
 * */
std::string parse_keyword(std::istream &in) {
    
    std::string keyword = "_";

    consume(in, '_');
    while  ( isalpha(in.peek()) ) {
        keyword += in.get();
    }

    return keyword;
}


/**
 * ------------------------- check_keyword() method -------------------------------------
 *  Consumes keyword against an expected string, 
 *  throws error if the string doesn't match expected.
 * 
 *  Arg:    std::istream
 *          std::string
 *  Return: none
 * */
void check_keyword(std::istream &in, std::string strExpected) {
    std::string strFromStream = "";

    for (int i = 0; i < strExpected.length(); i++) {
        strFromStream += in.get();
    }

    if (strFromStream != strExpected) {
        throw std::runtime_error("check_keyword doesn't match input string");
    }
}





/**
 * ********************************************
 * TESTS FOR PARSE
 * ********************************************
 * */

TEST_CASE("consume") {

    std::stringstream colin("colin");

    consume(colin, 'c');
    CHECK( colin.peek() == 'o' );

    consume(colin, 'o');
    CHECK( colin.peek() == 'l' );

    CHECK_THROWS_WITH( consume(colin, 'w'), "consume mismatch" );

}


TEST_CASE("skip_whitespace") {
    
    std::stringstream space("  s  space");

    skip_whitespace(space);

    CHECK( space.peek() == 's' );

    consume( space, 's' );

    skip_whitespace(space);

    CHECK( space.peek() == 's' );

    consume( space, 's' );

    CHECK( space.peek() == 'p' );

}


TEST_CASE("parse_expr") {

    std::string one = "1";
    CHECK( parse_string(one)->to_string() == one );

    /**
     * The next CHECK basically runs through everything, so there we go.
     * */
    std::string fac = "_let factrl = _fun (factrl) _fun (x) _if x == 1 _then 1 _else x * factrl(factrl)(x + -1) _in  factrl(factrl)(10)";
    CHECK( CAST(NumVal)((parse_string(fac))->interp(Env::empty))->rep == 3628800 );

}


TEST_CASE("parse add") {

    std::string test_add = "1 + 1";
    CHECK( parse_string(test_add)->to_string_pretty() == test_add );

}


TEST_CASE("parse mult") {

    std::string test_mult = "2 * 2";
    CHECK( parse_string(test_mult)->to_string_pretty() == test_mult );

}


TEST_CASE("parse add and mult") {

    std::string test_add_mult = "1 + 2 * 3";
    CHECK( parse_string(test_add_mult)->to_string_pretty() == test_add_mult );

    std::string test_mult_and = "1 * 2 + 3";
    CHECK( parse_string(test_mult_and)->to_string_pretty() == test_mult_and );

}


TEST_CASE("parse var") {

    std::string test_var = "a + 1";
    CHECK( parse_string(test_var)->to_string_pretty() == test_var );

}


TEST_CASE("parse let") {

    std::string test_let = "(_let x = 2 _in (x + 2))";
    CHECK( parse_string(test_let)->to_string_pretty() == test_let );
}


TEST_CASE("parse if") {

    std::string test_if = "(_if _true _then 1 _else 2)";
    CHECK( parse_string(test_if)->to_string() == test_if );

}


