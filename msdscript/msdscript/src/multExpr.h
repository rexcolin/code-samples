#ifndef MULT_EXPR
#define MULT_EXPR

#include "expr.h"
#include <string>
#include <iostream>


/**	NOTES on MultExpr class
 * 	MultExpr is a type of expression, which specifically represents a mathematical expression 
 * 	where two expressions are being multiplied together.
 * 
 * 	For example, these inner expressions could be numbers (represented by the NumExpr class): 
 * 
 * 				2 * 2
 * 
 * variables (VarExpr class):
 * 
 * 				3 * x
 * 
 * 	or other larger expressions such as AddExpr, MultExpr, and more:
 * 
 * 				1 * (1 + 1)
 * 				2 * (2 * 3)
 * 				(12 + 1) * (8 + 12)
 * 
 *	(Please refer to the msdscriptGuide.docx file for further illustrations of how this works.)
 *	
 * 	The multiply sign (*) is not accounted for as a member variable in this class, it is known 
 * 	that the left hand side expression (MultExpr::lhs) is being multiplied to the right 
 * 	hand side expression (MultExpr::rhs). The print() method does print the multiply 
 * 	sign, and the parser (see parse.h and parse.cpp) uses the multiply sign to construct a 
 * 	MultExpr.
 * */
class MultExpr : public Expr {
public:

	/** Member variables:
	 * 	lhs is the left hand side in a MultExpr, rhs is the right 
	 * 	hand side. They can be any class that inherits from the Expr 
	 * 	class.
	 * */
	PTR(Expr) lhs;
	PTR(Expr) rhs;


	/** Method stubs for AddExpr class. See expr.h for general descriptions 
	 * 	or addExpr.cpp for details.
	 * */

	MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);

	bool equals(PTR(Expr) e);
	PTR(Val) interp(PTR(Env) env);
	void step_interp();
	//NO MORE SUBST
	void print(std::ostream& stream);
	void pretty_print_at(print_mode_t mode, std::ostream& stream);
};

#endif