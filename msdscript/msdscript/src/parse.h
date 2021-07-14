#ifndef PARSE
#define PARSE

#include "expr.h"
#include <iostream>
#include <string>

void consume(std::istream &in, int expect);

void skip_whitespace(std::istream &in);

PTR(Expr) parse(std::istream &in);

PTR(Expr) parse_string(std::string s);

PTR(Expr) parse_comparg(std::istream &in);

PTR(Expr) parse_expr(std::istream &in);

PTR(Expr) parse_addend(std::istream &in);

PTR(Expr) parse_if(std::istream &in);

PTR(Expr) parse_mulitcand(std::istream &in);

PTR(Expr) parse_inner(std::istream &in);

PTR(Expr) parse_num(std::istream &in);

PTR(Expr) parse_var(std::istream &in);

PTR(Expr) parse_let(std::istream &in);

PTR(Expr) parse_fun(std::istream &in);

std::string parse_str_helper(std::istream &in);

// PTR(Expr) parse_bool(std::istream &in);

std::string parse_keyword(std::istream &in);

void check_keyword(std::istream &in, std::string strexpected);

#endif