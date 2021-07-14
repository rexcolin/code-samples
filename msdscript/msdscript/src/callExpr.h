#ifndef CALL_EXPR
#define CALL_EXPR

#include "expr.h"


/** NOTES on CallExpr class
 *  CallExpr is a type of expression, to be used explicity to 'call' a function, represented by a 
 *  FunExpr (see funExpr.h). 
 * 
 * */
class CallExpr : public Expr {
public:

    /** Member variables:
     *  to_be_called is the function, and actual_arg is the number to insert in 
     *  the function.
     * */
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;

	/** Method stubs for CallExpr class. See expr.h for general descriptions 
	 * 	or callExpr.cpp for details.
	 * */
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);

    bool equals(PTR(Expr) e);
    PTR(Val) interp(PTR(Env) env);
    void step_interp();
    void print(std::ostream& stream);
    void pretty_print_at(print_mode_t mode, std::ostream& stream);

};


#endif