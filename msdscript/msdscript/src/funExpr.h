#ifndef FUN_EXPR
#define FUN_EXPR

#include "expr.h"
#include <string>
#include <iostream>


/** NOTES on FunExpr class
 *  FunExpr is a type of expression, which specifically represents a function. One of 
 *  the member variables is an Expr, so you can nest other expressions in this class. 
 *  The FunExpr::body member variable is the function, the FunExpr::formal_arg member 
 *  variable is the string to use as a variable in the function. When writing a function
 *  in msdscript, it should be in this format:
 * 
 *              _fun(x)(x + 1)
 * 
 *          where _fun is the keyword to the parser that a function is about to start, 
 *          (x) is the FunExpr::formal_arg member variable, and (x + 1) is the 
 *          FunExpr::body member variable.
 * 
 *  Note that when printing, it will just print:
 *  
 *              [function]
 * 
 * */
class FunExpr : public Expr {
public:

    /** Member variables:
     *  The FunExpr::body member variable is the function, the FunExpr::formal_arg 
     *  member variable is the string to use as a variable in the function. 
     *  FunExpr::body can be any class that inherits from the Expr class.
     * */
    std::string formal_arg;
    PTR(Expr) body;


    /** Method stubs for FunExpr class. See expr.h for general descriptions or 
     *  funExpr.cpp for details.
     * */
    
    FunExpr(std::string formal_arg, PTR(Expr) body);

    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    void step_interp();
    // NO MORE SUBST
    void print(std::ostream& stream);
    void pretty_print_at(print_mode_t mode, std::ostream& stream);

};

#endif