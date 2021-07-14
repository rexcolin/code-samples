#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "exec.h"

static std::string random_expr_string();

void test_helper(const char * arg0, const char * arg1);

void test_helper_internal(const char * arg1, const char * arg2, const char * mode);

void test_helper_single_arg(const char * arg);

int main (int argc, char **argv) {

    if (argc == 3) {
        test_helper(argv[1], argv[2]);
    }
    else if (argc == 2) {
        test_helper_single_arg(argv[1]);
    }
    else if (argc == 1) {
        test_helper_single_arg( "msdscript" );
    } else {
        std::runtime_error("Too many arguments");
        return 1;
    }

    return 0;
}

// TODO: improve error messaging, so I can find differences
static std::string random_expr_string() {
    int n = (rand() % 10);
    if (n < 4) {
        return std::to_string(rand());
    } else if (n < 6) {
        return random_expr_string() + "+" + random_expr_string();
    } else if (n < 8) {
        return random_expr_string() + "*" + random_expr_string();
    } else {
        return "_let x = " + random_expr_string() + "_in " + random_expr_string();
    }
}

void test_helper(const char * arg1, const char * arg2) {
    const char * interp = "--interp";
    const char * print = "--print";
    const char * pretty_print = "--pretty-print";
    
    test_helper_internal(arg1, arg2, "--interp");
    test_helper_internal(arg1, arg2, "--print");
    test_helper_internal(arg1, arg2, "--pretty-print");
}

void test_helper_internal(const char * arg1, const char * arg2, const char * mode) {
    
    const char * const interp1_argv[] = { arg1, mode };
    const char * const interp2_argv[] = { arg2, mode };

    srand(clock());

    for (int i = 0; i < 100; i++) {
        std::string in = random_expr_string();
        std::cout << "Trying " << in << "\n";

        ExecResult interp1_result = exec_program(2, interp1_argv, in);
        ExecResult interp2_result = exec_program(2, interp2_argv, in);

        if (interp1_result.out != interp2_result.out) {
            std::string modeString = mode;
            throw std::runtime_error("different results for " + modeString);
        }
    }
}

void test_helper_single_arg(const char * arg) {
    const char * const interp_argv[] = { arg, "--interp" };
    const char * const print_argv[] = { arg, "--print" };

    srand(clock());

    for (int i = 0; i < 100; i++) {
        std::string in = random_expr_string();
        std::cout << "Trying " << in << "\n";

        ExecResult interp_result = exec_program(2, interp_argv, in);
        ExecResult print_result = exec_program(2, print_argv, in);

        ExecResult interp_again_result = exec_program(2, interp_argv, print_result.out);
        if (interp_again_result.out != interp_result.out) {
            throw std::runtime_error("different results for printed");
        } 
    }
}