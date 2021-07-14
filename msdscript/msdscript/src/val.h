#ifndef VAL
#define VAL

#include "pointer.h"
#include <string>
class Expr;
class Cont;


/** NOTES on the Val class
 *  
 *  Val is another of the main classes of msdscript, along with Expr. A Val pointer is 
 *  what is returned when Expr classes call interp(), and we use this class for that 
 *  because its methods add_to() and mult_to() become very important when using 
 *  step_interp(), as well as using call() and call_step with functions.
 * 
 *  The Val class is abstract. and is inherited by the following classes:
 *      BoolVal     (boolVal.h)
 *      FunVal      (funVal.h)
 *      NumVal      (numVal.h)
 * 
 *  These classes and their corresponding Expr version are the 'end point' classes for 
 *  the interp process. Tests for each inheriting class and its methods are included in 
 *  the .cpp file for each class.
 * 
 * */
class Val {
public:

    /** NOTES on equals()
     *  equals() is used to compare values (Val). While each implementation 
     *  in the inheriting classes differ slightly, they all use some form of 
     *  casting the Val given as an argument and seeing if the member variables 
     *  match the Val that is calling the method. This matches what the 
     *  Expr::equals() methods do.
     * */
    virtual bool equals(PTR(Val) val) = 0;

    /** NOTES on add_to()
     *  add_to() only actually performs with the NumVal class, where it adds the 
     *  member variable of that class to the member variable of the Val provided as an 
     *  argument. If the other Val classes call this, an error is thrown.
     * */
    virtual PTR(Val) add_to(PTR(Val) val) = 0;

    /** NOTES on mult_to()
     *  mult_to() only actually performs with the NumVal class, where it multiplies the 
     *  member variable of that class to the member variable of the Val provided as an 
     *  argument. If the other Val classes call this, an error is thrown.
     * */
    virtual PTR(Val) mult_to(PTR(Val) val) =0;

    /** NOTES on to_string()
     *  to_string returns a std::string. For NumVal it returns the member variable 
     *  as a string, for FunVal and BoolVal it returns a custom string.
     * */
    virtual std::string to_string() = 0;

    /** NOTES on call()
     *  call() only actually performs with the FunVal class, where it interps the 
     *  FunVal::body member variable with the FunVal::env member variable. If NumVal 
     *  or BoolVal try to call it, an error is thrown.
     * */
    virtual PTR(Val) call(PTR(Val) val) = 0;

    /** NOETS on call_step()
     *  Same idea as call() above, but using the continuation loop instead of 
     *  traditional recursion.
     * */
    virtual void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) = 0;

    /* destructor */
    virtual ~Val(){};
};

#endif