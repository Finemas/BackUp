# Makefile for Backup program
## Constants
CXX = g++
CXXFLAGS = -std=c++14
CPPFLAGS = -Wall -pedantic -Wextra -g -Wno-long-long -O2

SRC_DIR = ./src/
APP_NAME = provaja6

### All cpp file and object files
SRCS = $(wildcard $(SRC_DIR)*.cpp)
OBJS = $(SRCS:.cpp=.o)

## Commands
all: doc compile

compile: $(OBJS)
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OBJS) -o $(APP_NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@
	@$(CXX) $(CXXFLAGS) -MM -MT $*.o $*.cpp > $*.d

doc: 
	doxygen Doxyfile

### 'opt' specifies parameters eg. make run opt="--show"	
run:
	@./$(APP_NAME) $(opt)

count: clean
	@wc -l src/*

clean:
	@$(RM) -r $(SRC_DIR)*.o ./doc $(APP_NAME) $(SRC_DIR)*.d

help:
	@printf "%s\n" "Help for make:"
	@printf "%s\n" "  all			Build documentation and executable file"
	@printf "%s\n" "  compile		Create executable binary file"
	@printf "%s\n" "  clean			Remove all generated files"
	@printf "%s\n" "  doc			Build documentation"
	@printf "%s\n" "  count			Characters counter"

-include $(SRCS:.cpp=.d)