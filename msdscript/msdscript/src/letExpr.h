#ifndef LET_EXPR
#define LET_EXPR

#include "expr.h"
#include "val.h"
#include "varExpr.h"
#include <string>
#include <iostream>


/** NOTES on LetExpr class
 * 	LetExpr is almost like a function that calls itself. The LetExpr::lhs member 
 * 	variable is a std::string that probably (but not necessarily) exists in the 
 * 	LetExpr::body member variable. The LetExpr::rhs member variable is what will 
 * 	replace the variable (LetExpr::lhs) in LetExpr::body when the expression is	
 * 	interpreted. This is the syntax, with the member variable names in []:
 * 
 * 			_let [lhs] = [rhs] _in [body]
 * 
 * 	So:
 * 
 * 			_let x = 1 _in x + 1
 * 
 * 	would interpret to 2.
 * 
 * 	Since two of the member variables are Exprs, you can nest expressions in those 
 * 	variables.
 * */
class LetExpr : public Expr {
public:

	/** Member variables:
	 * 	lhs is a string that presumably exists in the body 
	 * 	member variable. rhs is what replaces the lhs string 
	 * 	within body when interpreted.
	 * */
	std::string lhs;
	PTR(Expr) rhs;
	PTR(Expr) body;


	/** Method stubs for LetExpr class. See expr.h for general descriptions 
	 * 	or letExpr.cpp for details.
	 * */
	LetExpr(std::string lhs, PTR(Expr) rhs, PTR(Expr) body);

	bool equals(PTR(Expr) e);
	PTR(Val) interp(PTR(Env) env);
	void step_interp();
	void print(std::ostream& stream);
	void pretty_print_at(print_mode_t mode, std::ostream& stream);
};

#endif // LET_EXPR