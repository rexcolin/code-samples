#ifndef FUN_VAL
#define FUN_VAL

#include "val.h"
#include "expr.h"
#include "step.h"
#include "cont.h"

#include <string>


/** NOTES on the FunVal class
 *  FunVal is a value that is returned when interp() is called on a FunExpr. its member 
 *  variables include a std::string that represents an argument in the function, an Expr 
 *  body of the function, and an Env environment to interpret the variable in the 
 *  function.
 * */
class FunVal : public Val {
public:

    /** Member variables:
     *  formal_arg is a std::string that represents the variable to 
     *  be used in the function. body is the body of the function, 
     *  and env is the environment to replace the variable in the 
     *  body with another value.
     * */
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;


    /** Method stubs for FunVal class. See val.h for general descriptions 
	 * 	or funVal.cpp for details.
	 * */
    FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env);

    bool equals(PTR(Val) val);
    // PTR(Expr) to_expr();
    PTR(Val) add_to(PTR(Val) val);
    PTR(Val) mult_to(PTR(Val) val);
    std::string to_string();
    PTR(Val) call(PTR(Val) val);
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};


#endif