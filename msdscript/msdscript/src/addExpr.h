#ifndef ADD_EXPR
#define ADD_EXPR

#include "expr.h"
#include <string>


/** NOTES on AddExpr class
 * 	AddExpr is a type of expression, which specifically represents a mathematical expression 
 * 	where two expressions are being added together. 
 * 
 * 	For example, these inner epressions could be numbers (respresented by the NumExpr class):
 * 
 * 				1 + 1
 * 
 *  variables (VarExpr class):
 * 
 * 				1 + x
 * 
 *  or other larger expressions such as AddExpr, MultExpr, and more:
 * 
 * 				1 + (1 + 3)
 * 				1 + (4 * 8)
 * 				(60 * 43) + (54 * x)
 * 
 *	(Please refer to the msdscriptGuide.docx file for further illustrations of how this works.)
 *
 * 	The plus sign (+) is not accounted for as a member variable in this class, it is known 
 * 	that the left hand side expression (member variable Expr lhs) is being added to the right 
 * 	hand side espression (member variable Expr rhs). The print() method does print the plus 
 * 	sign, and the parser (see parse.h and parse.cpp) uses the plus sign to construct an 
 * 	AddExpr.
 * 
 * */
class AddExpr : public Expr {
public:

	/** Member variables:
	 * 	lhs is the left hand side in an AddExpr, rhs is the right 
	 * 	hand side. They can be any class that inherits from the Expr 
	 * 	class.
	 * */
	PTR(Expr) lhs;
	PTR(Expr) rhs;


	/** Method stubs for AddExpr class. See expr.h for general descriptions 
	 * 	or addExpr.cpp for details.
	 * */

	AddExpr(PTR(Expr)lhs, PTR(Expr)rhs);

	bool equals(PTR(Expr)e);
	PTR(Val) interp(PTR(Env) env);
	void step_interp();
	void print(std::ostream& stream);
	void pretty_print_at(print_mode_t mode, std::ostream& stream);
};

#endif