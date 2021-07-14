#ifndef NUM_VAL
#define NUM_VAL

#include "val.h"
#include "expr.h"


/** NOTES on NumVal class
 *  NumVal is a value that is returned when interp() is called on a NumExpr. Its member 
 *  variable is an int, like a NumExpr. The add_to() and mult_to() methods are used for 
 *  this class in the continuation loop.
 * 
 * */
class NumVal : public Val {
public:

    /** Member variable:
     *  rep is the 'representation' of a NumVal, which is 
     *  an int.
     * */
    int rep;


    /** Method stubs for NumVal class. See val.h for general descriptions 
	 * 	or numVal.cpp for details.
	 * */
    NumVal(int rep);
    
    bool equals(PTR(Val) v);
    // PTR(Expr) to_expr();
    PTR(Val) add_to(PTR(Val) v);
    PTR(Val) mult_to(PTR(Val) v);
    std::string to_string();
    PTR(Val) call(PTR(Val) val);
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
    
};

#endif