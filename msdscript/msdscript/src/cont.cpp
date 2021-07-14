#include "cont.h"
#include "step.h"
#include "val.h"
#include "env.h"
#include "boolVal.h"
#include "funVal.h"
#include "expr.h"
#include <stdexcept>

PTR(Cont) Cont::done = NEW(DoneCont)();


/****************************
 *      DoneCont
 * **************************/

DoneCont::DoneCont() {}

void DoneCont::step_continue() {
    throw std::runtime_error("can't continue done");
}


/****************************
 *      RightThenAddCont
 * **************************/

RightThenAddCont::RightThenAddCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest) {
    this->rhs = rhs;
    this->env = env;
    this->rest = rest;
}

void RightThenAddCont::step_continue() {
    PTR(Val) lhs_val = Step::val;
    Step::mode = Step::interp_mode;
    Step::expr = rhs;
    Step::env = env;
    Step::cont = NEW(AddCont)(lhs_val, rest);
}


/****************************
 *      AddCont
 * **************************/

AddCont::AddCont(PTR(Val) lhs_val, PTR(Cont) rest) {
    this->lhs_val = lhs_val;
    this->rest = rest;
}

void AddCont::step_continue() {
    PTR(Val) rhs_val = Step::val;
    Step::mode = Step::continue_mode;
    Step::val = lhs_val->add_to(rhs_val);
    Step::cont = rest;
}


/****************************
 *      RightThenMultCont
 * **************************/

RightThenMultCont::RightThenMultCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest) {
    this->rhs = rhs;
    this->env = env;
    this->rest = rest;
}

void RightThenMultCont::step_continue() {
    PTR(Val) lhs_val = Step::val;
    Step::mode = Step::interp_mode;
    Step::expr = rhs;
    Step::env = env;
    Step::cont = NEW(MultCont)(lhs_val, rest);
}


/****************************
 *      MultCont
 * **************************/

MultCont::MultCont(PTR(Val) lhs_val, PTR(Cont)rest) {
    this->lhs_val = lhs_val;
    this->rest = rest;
}

void MultCont::step_continue() {
    PTR(Val) rhs_val = Step::val;
    Step::mode = Step::continue_mode;
    Step::val = lhs_val->mult_to(rhs_val);
    Step::cont = rest;
}


/****************************
 *      RightThenEqCont
 * **************************/

RightThenEqCont::RightThenEqCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest) {
    this->rhs = rhs;
    this->env = env;
    this->rest = rest;
}

void RightThenEqCont::step_continue() {
    PTR(Val) lhs_val = Step::val;
    Step::mode = Step::interp_mode;
    Step::expr = rhs;
    Step::env = env;
    Step::cont = NEW(EqCont)(lhs_val, rest);
}


/****************************
 *      EqCont
 * **************************/

EqCont::EqCont(PTR(Val) lhs_val, PTR(Cont) rest) {
    this->lhs_val = lhs_val;
    this->rest = rest;
}

void EqCont::step_continue() {
    PTR(Val) rhs_val = Step::val;
    Step::mode = Step::continue_mode;
    Step::val = NEW(BoolVal)(lhs_val->equals(rhs_val));
    Step::cont = rest;
}


/****************************
 *      IfBranchCont
 * **************************/

IfBranchCont::IfBranchCont(PTR(Expr) then_part, PTR(Expr) else_part, PTR(Env) env, PTR(Cont) rest) {
    this->then_part = then_part;
    this->else_part = else_part;
    this->env = env;
    this->rest = rest;
}

void IfBranchCont::step_continue() {
    PTR(Val) test_val = Step::val;
    Step::mode = Step::interp_mode;
    if (test_val->equals(NEW(BoolVal)(true)))
        Step::expr = then_part;
    else
        Step::expr = else_part;
    Step::env = env;
    Step::cont = rest;
}


/****************************
 *      LetBodyCont
 * **************************/

LetBodyCont::LetBodyCont(std::string var, PTR(Expr) body, PTR(Env) env, PTR(Cont) rest) {
    this->var = var;
    this->body = body;
    this-> env = env;
    this->rest = rest;
}

void LetBodyCont::step_continue() {
    Step::mode = Step::interp_mode;
    Step::expr = body;
    Step::env = NEW(ExtendedEnv)(var, Step::val, env);
    Step::cont = rest;
}


/****************************
 *      ArgThenCallCont
 * **************************/

ArgThenCallCont::ArgThenCallCont(PTR(Expr) actual_arg, PTR(Env) env, PTR(Cont) rest) {
    this->actual_arg = actual_arg;
    this->env = env;
    this->rest = rest;
}

void ArgThenCallCont::step_continue() {
    Step::mode = Step::interp_mode;
    Step::expr = this->actual_arg;
    Step::env = this->env;
    Step::cont = NEW(CallCont)(Step::val, rest);
}


/****************************
 *      CallCont
 * **************************/

CallCont::CallCont(PTR(Val) to_be_called_val, PTR(Cont) rest) {
    this->to_be_called_val = to_be_called_val;
    this->rest = rest;
}

void CallCont::step_continue() {
    this->to_be_called_val->call_step(Step::val, rest);
}

