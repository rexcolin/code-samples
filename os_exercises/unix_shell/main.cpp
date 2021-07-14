#include <iostream>
#include "shelpers.hpp"
#include <sstream>

int main(int argc, const char **argv) {

    std::string outsideString;

    std::cout << "% ";
    while (getline(std::cin, outsideString)) {

        std::string tmp;
        std::stringstream ss(outsideString);
        std::vector<std::string> inputs;
        
        while(ss >> tmp) {
            inputs.push_back(tmp);
        }

        std::vector<Command> commands = getCommands(inputs);

        for (int i = 0; i < commands.size(); i++) {
            
            int id = fork();
            if ( id < 0 ) {
                perror("fork didn't fork");
            } else if (id == 0) {
                
                /**
                 * THIS IS THE CHILD
                 * */

                // checks to see if a file was setup as input, if so use it
                if ( commands[i].fdStdin != 0) {
                    if ( dup2(commands[i].fdStdin, 0) == -1 ) {
                        perror("dup2 for stdin didn't work");
                    }
                }

                // checks to see if a file was setup as output, if so use it
                if ( commands[i].fdStdout != 1) {
                    if ( dup2(commands[i].fdStdout, 1) == -1 ) {
                        perror("dup2 for stdout didn't work");
                    }
                }

                // if the command exec is "cd", don't call exec in the child, just return
                // otherwise execute the command with arguments
                if ( commands[i].exec == "cd" ) {
                    return 0;
                } else if( execvp(commands[i].exec.c_str(), const_cast<char* const *>(commands[i].argv.data())) != 0 ) {
                    perror("command didn't execute");
                }
            
            } else {
                /**
                 * THIS IS THE PARENT
                 * */

                // if there's an ampersand in the command, don't wait
                if ( commands[i].background == false ) {
                    if ( waitpid(id, NULL, 0) == -1 ) {
                        perror("didn't wait!");
                    }
                }

                // closes the stdin if it's open
                if ( commands[i].fdStdin != 0 ) {
                    if ( close(commands[i].fdStdin) == -1 ) {
                        perror("stdin didn't close");
                    }
                }
                
                // closes the stdout if it's open
                if ( commands[i].fdStdout != 1 ) {
                    if ( close(commands[i].fdStdout) == -1 ) {
                        perror("stdout didn't close");
                    }
                }
                
                // checks if the command exec is "cd", if it is the calls the appropriate chdir
                if( commands[i].exec == "cd" ) {
                    if ( commands[i].argv.size() == 2 && commands[i].argv[1] == NULL ) {
                        if ( chdir(getenv("HOME")) != 0 ) {
                            perror("couldn't go home");
                        }
                    }
                    else if ( commands[i].argv.size() > 2 ) {
                        if ( chdir(commands[i].argv[1]) != 0 ) {
                            perror(NULL);
                        }
                    }
                }           
            }
        }
        // print a character for the start of the next line
        std::cout << "% ";
    }
}