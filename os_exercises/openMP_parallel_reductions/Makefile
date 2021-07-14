SRCS = main.cpp

INCS = stdSum.hpp ompCustomSum.hpp

main: $(SRCS) $(INCS)
	c++ --std=c++14 -O3 -Xpreprocessor -fopenmp -lomp -o main $(SRCS)