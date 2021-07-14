#ifndef BOOL_EXPR
#define BOOL_EXPR

#include "expr.h"
#include "val.h"
#include <string>
#include <iostream>


/**	NOTES on BoolExpr class
 * 	BoolExpr is an expression that represents a boolean. It is one of the 'end points' of the 
 * 	Expr subclasses, which means that other expressions cannot be nested inside of it. The only 
 * 	member variable is a boolean, not another Expr.
 * 
 * */
class BoolExpr : public Expr {
public:

	/** Member variable:
	 *	rep is the 'respresentation' of a BoolExpr, which is just 
	 *	a boolean.
	 * */
	bool rep;


	/** Method stubs for BoolExpr class. See expr.h for general descriptions 
	 * 	or boolExpr.cpp for details.
	 * */
	
	BoolExpr(bool rep);

	bool equals(PTR(Expr) e);
	PTR(Val) interp(PTR(Env) env);
	void step_interp();
	void print(std::ostream& stream);
	void pretty_print_at(print_mode_t mode, std::ostream& stream);
};

#endif