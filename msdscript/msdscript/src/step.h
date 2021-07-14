#ifndef STEP
#define STEP

#include "pointer.h"
#include "expr.h"
#include "val.h"
#include "env.h"
#include "cont.h"


class Val;
class Expr;
class Env;
class Cont;


/** NOTES on Step class
 *  This is the class that makes the alternative to Expr::interp() work. Instead of 
 *  returning values like Expr::interp() does, calling Expr::step_interp() stores the 
 *  relevant values in the Step class member variables, and uses those along with the 
 *  Cont class to interpret the expressions without the same memory restrictions that 
 *  Expr::interp() has when it ercursively calls functions and allocates more and more 
 *  space on the stack.
 * */
CLASS(Step) {
public:
    typedef enum {
        interp_mode,
        continue_mode
    } mode_t;

    /*  Mode indicates whether the next step is to
        start interpreting an expression or start
        delivering a value to a continuation. */
    static mode_t       mode;      /* chooses mode */

    /*  The expression to interpret, meaningful
        only when 'mode' is 'interp_mode': */
    static PTR(Expr)    expr;      /* for interp_mode */

    /*  The environment to be used when interpreting,
        only when 'mode' is 'interp_mode': */
    static PTR(Env)     env;       /* for interp_mode */

    /*  The value to be delivered tot he continuation,
        meaningful only when 'mode' is 'continue_mode': */
    static PTR(Val)     val;        /* for continue_mode */

    /*  The continuation to receive a value, meaningful
        only when 'mode' is 'continue_mode': */
    static PTR(Cont)    cont;       /* all modes */


    /**
     * method stub
     * */
    static PTR(Val) interp_by_steps(PTR(Expr) e);
};


#endif
