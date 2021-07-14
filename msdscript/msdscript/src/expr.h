#ifndef EXPR
#define EXPR

#include "pointer.h"
#include <string>

/* Examples of keywords to be used in pretty_print_at() below */
typedef enum {
	print_group_none,
	print_group_AddExpr,
	print_group_AddExpr_or_mult,
} print_mode_t;

class Val;
class Env;


/** NOTES on the Expr class
 * 	
 *	Expr is the fundamental type in msdscript. It is used to represent mathematical expressions, booleans, functions and 
 *	conditional statements. This includes a wide range of ideas, including addition, multiplication, functions, if-then 
 *	statements, and others. No subtraction though. These expressions can be nested within each other using the inheriting 
 *	classes' member vairables.
 * 
 * 	The Expr class is abstract, and is inherited by the following classes:
 * 		AddExpr 	(addExpr.h)
 * 		BoolExpr	(boolExpr.h)
 * 		CallExpr	(callExpr.h)
 * 		EqExpr		(eqExpr.h)
 * 		FunExpr		(funExpr.h)
 * 		IfExpr		(ifExpr.h)
 * 		LetExpr		(letExpr.h)
 * 		MultExpr	(multExpr.h)
 * 		NumExpr		(numExpr.h)
 * 		VarExpr		(varExpr.h)
 * 		
 * 	Tests for each inheriting class and its methods are included in the .cpp files for each 
 * 	class (including expr.cpp).
 * */

CLASS(Expr) {
public:

	/** NOTES on equals()
	 * 	equals() is used to compare expressions. While each implementation 
	 * 	in the inheriting classes differs slightly, they all use some form of 
	 * 	casting the Expr given as an argument and seeing if the member variables 
	 * 	match the Expr that is calling the method.
	 * */
	virtual bool equals(PTR(Expr)e) = 0;
	
	/** NOTES on interp()
	 *	interp() takes an expression (including nested espressions) and 
	 *	produces the result. It does this on the stack, and so is limited 
	 *	if you are trying to interpret large expressions, such as counting 
	 *	down from 1,000,000.
	 * */
	virtual PTR(Val) interp(PTR(Env) env) = 0;
	
	/** NOTES on step_interp()
	 * 	step_interp() takes an expression and produces the result, like interp() 
	 * 	above. However, it does not perform traditional recursion to do so, so 
	 * 	it is not as limited as interp() for large expressions. It can countdown 
	 * 	from 1,000,000.
	 * */
	virtual void step_interp() = 0;

	/** NOTES on print()
	 * 	print() prints an expression (including nested expressions). It follows a 
	 * 	fairly simple format, some examples are found in the implementations of 
	 * 	this method in the inheriting classes. It uses 
	 * */
	virtual void print(std::ostream& stream) = 0;
	
	
	/**	NOTES ON pretty_print()
	 * 	pretty_print() is currently set up to just call print(). If desired, you 
	 * 	could format this print option however you would like.
	 * */
	void pretty_print(std::ostream& stream);

	/** NOTES on pretty_print_at()
	 * 	used in the pretty_print() method, it takes a mode and prints a certain way 
	 * 	accordingly.
	 * */
	virtual void pretty_print_at(print_mode_t mode, std::ostream& stream) = 0;

	/** NOTES on to_string()
	 * 	to_string takes an expression and converts it to a string by using the 
	 *	print() method above. This method is implemented for the Expr class, and 
	 *	is not changed by inheriting classes.
	 * */
	std::string to_string();

	/** NOTES on to_string_pretty()
	 * 	to_string_pretty() takes an expression and converts it to a string like 
	 * 	to_string() above, but uses the optional (and currently not implemented) 
	 * 	custom printing format in pretty_print(), rather than the regular print().
	 * */
	std::string to_string_pretty();

	/** Destructor **/
	virtual ~Expr(){};

};


#endif