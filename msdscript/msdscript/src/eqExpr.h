#ifndef EQ_EXPR
#define EQ_EXPR

#include "expr.h"
#include "val.h"
#include <string>
#include <iostream>


/**	NOTES on EqExpr class
 * 	EqExpr is a type of expression, which specifically represents a comparison between 
 * 	two expressions.
 * 
 * 	For example, an EqExpr could be represented visually like this:
 *
 * 				1 == 2
 * 
 * 		where 1 is the lhs member variable and 2 is the rhs member variable.
 * 
 * 	As the member variables are Expr's, it is possible to nest expressions using this 
 * 	class, as follows:
 * 
 * 				2 == (1 + 1)
 * 
 * 		where 2 is the lhs (NumExpr) and (1 + 1) is the rhs (AddExpr).
 * 
 * 	The doubel equals sign (==) is not accounted for as a member variable in this class, 
 * 	it is known that the left hand side expression (member variable Expr lhs) is being 
 * 	compared to the right hand side espression (member variable Expr rhs). The print() 
 * 	method does print the double equals sign, and the parser (see parse.h 
 * 	and parse.cpp) uses the double equals sign to construct an EqExpr.
 * 
 * */
class EqExpr : public Expr {
public:

	/** Member variables:
	 * 	lhs is the left hand side in an EqExpr, rhs is the right 
	 * 	hand side. They can be any class that inherits from the Expr 
	 * 	class.
	 * */
	PTR(Expr) lhs;
	PTR(Expr) rhs;

	/** Method stubs for EqExpr class. See expr.h for general descriptions 
	 * 	or eqExpr.cpp for details.
	 * */
	EqExpr(PTR(Expr) lhs, PTR(Expr) rhs);
	
	bool equals(PTR(Expr) e);
	PTR(Val) interp(PTR(Env) env);
	void step_interp();
	void print(std::ostream& stream);
	void pretty_print_at(print_mode_t mode, std::ostream& stream);
};

#endif