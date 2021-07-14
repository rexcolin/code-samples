#ifndef CONT
#define CONT

#include "pointer.h"
#include "step.h"
#include <string>

class Expr;
class Env;
class Val;


/** NOTES on Cont class
 *  The Cont class is basically what the continuation loop uses to determine what to do 
 *  with the next step, which 'side' to continue with, what to do with the other info, 
 *  when to finish, etc. The description is basically in the name of the classes.
 * 
 *  Classes that inherit from Cont:
 *      DoneCont
 *      RightThenAddCont
 *      AddCont
 *      RightThenMultCont
 *      MultCont
 *      RightThenEqCont
 *      EqCont
 *      IfBranchCont
 *      LetBodyCont
 *      ArgThenCallCont
 *      CallCont
 * 
 * */
CLASS(Cont) {
public:

    static PTR(Cont) done;

    virtual void step_continue() = 0;

};


class DoneCont : public Cont {
public:

    DoneCont();
    void step_continue();

};


class RightThenAddCont : public Cont {
public:

    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;

    RightThenAddCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);

    void step_continue();
};


class AddCont : public Cont {
public:

    PTR(Val) lhs_val;
    PTR(Cont) rest;

    AddCont(PTR(Val) lhs_val, PTR(Cont) rest);

    void step_continue();
};


class RightThenMultCont : public Cont {
public:

    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;

    RightThenMultCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);

    void step_continue();
};


class MultCont : public Cont {
public:

    PTR(Val) lhs_val;
    PTR(Cont) rest;

    MultCont(PTR(Val) lhs_val, PTR(Cont) rest);

    void step_continue();
};


class RightThenEqCont : public Cont {
public:

    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;

    RightThenEqCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest);

    void step_continue();
};


class EqCont : public Cont {
public:

    PTR(Val) lhs_val;
    PTR(Cont) rest;

    EqCont(PTR(Val) lhs_val, PTR(Cont) rest);

    void step_continue();
};


class IfBranchCont : public Cont {
public:

    PTR(Expr) then_part;
    PTR(Expr) else_part;
    PTR(Env) env;
    PTR(Cont) rest;

    IfBranchCont(PTR(Expr) then_part, PTR(Expr) else_part, PTR(Env) env, PTR(Cont) rest);

    void step_continue();
};


class LetBodyCont : public Cont {
public:

    std::string var;
    PTR(Expr) body;
    PTR(Env) env;
    PTR(Cont) rest;

    LetBodyCont(std::string var, PTR(Expr) body, PTR(Env) env, PTR(Cont) rest);

    void step_continue();
};


class ArgThenCallCont : public Cont {
public:

    PTR(Expr) actual_arg;
    PTR(Env) env;
    PTR(Cont) rest;

    ArgThenCallCont(PTR(Expr) actual_arg, PTR(Env) env, PTR(Cont) rest);

    void step_continue();
};


class CallCont : public Cont {
public:

    PTR(Val) to_be_called_val;
    PTR(Cont) rest;

    CallCont(PTR(Val) to_be_called_val, PTR(Cont) rest);

    void step_continue();
};


#endif