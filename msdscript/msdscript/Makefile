SRC := src
OBJ := obj

DEBUG_OBJ := debug_obj

SOURCES := $(wildcard $(SRC)/*.cpp)
OBJECTS := $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SOURCES))

DEBUG_OBJECTS = $(patsubst $(SRC)/%.cpp, $(DEBUG_OBJ)/%.o, $(SOURCES))

CXXFLAGS = --std=c++14 -fsanitize=undefined -fno-sanitize-recover=undefined -O2
DEBUGFLAGS = --std=c++14 -g

msdscript: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o msdscript

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(SRC) -c $< -o $@




debug: $(DEBUG_OBJECTS)
	$(CXX) $(DEBUGFLAGS) $^ -o debug

$(DEBUG_OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(DEBUGFLAGS) -I$(SRC) -c $< -o $@

coverage: $(SRCS) $(INCS)
	c++ -fprofile-instr-generate -fcoverage-mapping --std=c++14 -O2 -o msd_coverage $(SRCS)

test_msdscript: $(TEST_SRCS) $(TEST_INCS)
	c++ --std=c++14 -O2 -o test_msdscript $(TEST_SRCS)

.PHONY: test
test: msdscript
	./msdscript --test

clean:
	rm -f msdscript
	rm -f debug
	rm -f obj/*.o
	rm -f debug_obj/*.o