#ifndef NUM_EXPR
#define NUM_EXPR

#include "expr.h"
#include <iostream>
#include <string>


/**	NOTES on  NumExpr class
 * 	NumExpr is a type of expression, which specifically represents a number. It is an 
 * 	'end point' Expr, meaning that it does not have an Expr as a member variable, it 
 * 	just has the NumExpr::rep, which is an integer.
 * 
 * */
class NumExpr : public Expr {
public:

	/** Member variables:
	 * 	NumExpr::rep stores an integer.
	 * */
	int rep;


	/** Method stubs for NumExpr class. See expr.h for general descriptions 
	 * 	or numExpr.cpp for details.
	 * */

	NumExpr(int rep);

	bool equals(PTR(Expr) e);
	PTR(Val) interp(PTR(Env) env);
	void step_interp();
	void print(std::ostream& stream);
	void pretty_print_at(print_mode_t mode, std::ostream& stream);
};

#endif