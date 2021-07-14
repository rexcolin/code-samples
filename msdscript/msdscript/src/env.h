#ifndef ENV
#define ENV

#include "pointer.h"
#include "val.h"
#include <string>


/** NOTES on Env class
 *  The Env class is basically a dictionary for the values that variables should be 
 *  replaced with when expressions are interpreted, whether through the Expr::interp() 
 *  method or the Expr::step_interp(). It has one member variable, an EmptyEnv.
 * 
 *  Classes that inherit from Env:
 *      EmptyEnv
 *      ExtendedEnv
 *  
 * */
CLASS(Env) {
public:

    /** Member variable:
     *  The Env::empy variable is a placeholder for a call to the 
     *  constructor for an EmptyEnv, which is defined in env.cpp.
     * */
    static PTR(Env) empty;

    /** Methods:
     *  The Env::lookup() method is to search the list for a certain 
     *  variable stored there, and will return the Val associtated 
     *  with that variable.
     * */
    virtual PTR(Val) lookup(std::string find_name) = 0;

};




/** NOTES on EmptyEnv class
 *  An EmptyEnv basically is a placeholder for a list that hasn't been filled in yet. 
 *  As the Extended Env is built, it adds to the linked list that functions as a 
 *  dictionary.
 * 
 * */
class EmptyEnv : public Env {
public:

    /** Methods:
     *  See env.cpp file for description.
     * */
    PTR(Val) lookup(std::string find_name);

};




/** NOTES on ExtendedEnv class
 *  The ExtendedEnv class is the main class that inherits from Env. It is used as a 
 *  linked list that functions as a dinctionary for std::string variables and the Vals 
 *  associated with them. As a linked list, it also has a pointer to the next Env in the 
 *  list.
 * 
 * */
class ExtendedEnv : public Env {
public:

    /** Member variables:
     *  ExtendedEnv::name is the variable that functions as 
     *  the thing that is looked up in the dictionary, 
     *  ExtendedEnv::val is the value associated with that 
     *  variable, and ExtendedEnv::rest is the pointer to 
     *  the next item in the list.
     * */
    std::string name;
    PTR(Val) val;
    PTR(Env) rest;

    /** Methods:
     *  See env.cpp file for description.
     * */
    ExtendedEnv(std::string name, PTR(Val) val, PTR(Env) rest);
    PTR(Val) lookup(std::string find_name);

};




#endif