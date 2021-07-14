TO BUILD PROGRAM:

    - Just run 'make' in terminal


TO RUN PROGRAM:

    - in terminal, run:
            ./main argv[1] argv[2] argv[3]

        where   argv[1] is the size of the array being tested
                argv[2] is the number of threads to be tested
                argv[3] is the number of times to repeat the test

    - SO running the command:
            ./main 10000 4 1000
        would use an array of 10000 and add it using 4 threads, and then average the timings over 1000 repetitions.


NOTES:  Don't enter a thread number of 0.
        All elements in the array are set to the value of 1, to help assert that the functions are adding the elements correctly.
