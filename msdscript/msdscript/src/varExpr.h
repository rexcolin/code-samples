#ifndef VAR_EXPR
#define VAR_EXPR

#include "expr.h"
#include "val.h"
#include <string>
#include <iostream>


/** NOTES on VarExpr class
 * 	VarExpr is a type of expression, whcih specifically represents a variable. It is an 
 * 	'end point' Expr, meaning that it does not have an Expr as a member variable, it 
 * 	just has the VarExpr::rep, which is a std::string.
 * 
 * */
class VarExpr : public Expr {
public:

	/** Member variables:
	 * 	VarExpr::rep stores a std::string.
	 * */
	std::string rep;


	/** Method stubs for VarExpr class. See expr.h for general descriptions 
	 * 	or varExpr.cpp for details.
	 * */

	VarExpr(std::string rep);

	bool equals(PTR(Expr) e);
	PTR(Val) interp(PTR(Env) env);
	void step_interp();
	// NO MORE SUBST
	void print(std::ostream& stream);
	void pretty_print_at(print_mode_t mode, std::ostream& stream);
};

#endif