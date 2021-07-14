#ifndef BOOL_VAL
#define BOOL_VAL

#include <string>
#include <iostream>
#include "val.h"
#include "expr.h"


/** NOTES on the BoolVal class
 *  BoolVal is a value that is returned when interp() is called on a BoolExpr. Its 
 *  member variable is just a bool, like BoolExpr, so the main difference is when we 
 *  use it and what methods it has. When it prints, it prints a std::string to represent 
 *  true or false, as shown:
 * 
 *          _true
 *          _false
 * 
 * */
class BoolVal : public Val {
public:

    /** Member variable:
     *  rep is the 'representation' of a Boolval, which is just 
     *  a boolean.
     * */
    bool rep;


    /** Method stubs for BoolVal class. See val.h for general descriptions 
	 * 	or boolVal.cpp for details.
	 * */
    BoolVal(bool rep);

    virtual bool equals(PTR(Val) v);
    virtual PTR(Val) add_to(PTR(Val) v);
    virtual PTR(Val) mult_to(PTR(Val) v);
    virtual std::string to_string();
    PTR(Val) call(PTR(Val) val);
    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest);
};

#endif