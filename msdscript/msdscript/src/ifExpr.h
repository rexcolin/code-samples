#ifndef IF_EXPR
#define IF_EXPR

#include "expr.h"
#include "val.h"
#include <string>
#include <iostream>


/**	NOTES on IfExpr class
 * 	IfExpr is a type of expression, which specifically represents an if-then-else 
 * 	statement, where the IfExpr::if_side is evaluated as a bool, and if it is true 
 * 	the IfExpr::then_side executes, but if false the IfExpr::else_side executes.
 * 
 * 	When entering in the program, using underscores before the keywords "if", "then", 
 * 	and "else" is necessary. Therefore:
 * 
 * 				_if _true _then 4 _else 5
 * 
 * 	would execute 4. Since each member variable is an Expr, you can nest expressions 
 * 	here, as shown:
 * 
 * 				_if (2 == 1) _then x + 234 _else x + 12
 * 
 * 	would execute x + 12.
 * */
class IfExpr : public Expr {
public:

	/** Member variables:
	 * 	if_side is interpreted as a bool, which then determines 
	 * 	whether then_side or else_side executes. They can be any 
	 * 	class that inherits from the Expr class.
	 * */
	PTR(Expr) if_side;
	PTR(Expr) then_side;
	PTR(Expr) else_side;


	/** Method stubs for AddExpr class. See expr.h for general descriptions 
	 * 	or ifExpr.cpp for details.
	 * */
	IfExpr(PTR(Expr) if_side, PTR(Expr) then_side, PTR(Expr) else_side);
	
	bool equals(PTR(Expr) e);
	PTR(Val) interp(PTR(Env) env);
	void step_interp();
	// NO MORE SUBST
	void print(std::ostream& stream);
	void pretty_print_at(print_mode_t mode, std::ostream& stream);
};

#endif