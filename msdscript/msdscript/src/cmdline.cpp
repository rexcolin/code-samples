#include "cmdline.h"
#define CATCH_CONFIG_RUNNER
#include "catch.h"
#include "expr.h"
#include "parse.h"
#include "val.h"
#include "env.h"
#include "step.h"
#include "cont.h"
#include "pointer.h"
#include <iostream>
#include <string.h>
#include <stdio.h>

/** NOTES on use_arguments() function:
 * 	This function takes arguments from the command line to first decide which mode to 
 * 	enter, and then to parse the script that is entered afterward. It makes use of the 
 * 	various parsing methods found in parse.h.
 * */
void use_arguments(int argc, char **argv) {

	bool test_bool = false;
	char help[] = "--help";
	char test[] = "--test";
	char interp[] = "--interp";
	char print[] = "--print";
	char pretty_print[] = "--pretty-print";
	char step[] = "--step";

	for (int i = 1; i < argc; i++) {

		if (strcmp(argv[i], help) == 0) {
			std::cout << "Run the program again, but with a word to check this time.\n";
			exit(1);
	
		} else if (strcmp(argv[i], test) == 0 && test_bool == false) {
			Catch::Session().run(1, argv);

			test_bool = true;	
		} else if (strcmp(argv[i], test) == 0 && test_bool == true) {
			std::cerr << "ERROR: already tested\n";
			exit(1);
		} else if (strcmp(argv[i], interp) == 0) {
			while (1) {
				PTR(Expr) e = parse(std::cin);

				std::cout << e->interp(Env::empty)->to_string();
				std::cout << "\n";

				skip_whitespace(std::cin);
				if (std::cin.eof())
					break;
			}
		} else if (strcmp(argv[i], step) == 0) {
			while (1) {
				PTR(Expr) e = parse(std::cin);

				PTR(Val) val = Step::interp_by_steps(e);
				std::cout << val->to_string();
				std::cout << "\n";

				skip_whitespace(std::cin);
				if (std::cin.eof())
					break;
			}
		} else if (strcmp(argv[i], print) == 0) {
			while (1) {
				PTR(Expr) e = parse(std::cin);

				e->print(std::cout);
				std::cout << "\n";

				skip_whitespace(std::cin);
				if (std::cin.eof())
					break;
			}
		} else if (strcmp(argv[i], pretty_print) == 0) {
			while (1) {
				PTR(Expr) e = parse(std::cin);

				e->pretty_print(std::cout);
				std::cout << "\n";

				skip_whitespace(std::cin);
				if (std::cin.eof())
					break;
			}
        } else {
			std::cerr << "ERROR";
			exit(1);
		}
	}

	return;
}

