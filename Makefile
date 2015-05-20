.PHONY: all clean check-c check-cpp check

CLANG_CXX := clang++
GCC_CXX := g++
CXX := $(shell if hash $(CLANG_CXX) 2>/dev/null; then echo $(CLANG_CXX); else \
	if hash $(GCC_CXX) 2>/dev/null; then echo $(GCC_CXX); else echo oops; \
	fi; fi)
ifeq ($(CXX), oops)
$(error no suitable c++ compiler found! install $(CLANG_CXX) or $(GCC_CXX))
endif

CXXFLAGS := -std=c++14 -Wall -Wextra -Werror -g -O0
LDFLAGS := -lclang

DEPS := cursor.hpp cursor-index.hpp
AST_OBJ := walk-ast.o cursor.o cursor-index.o

AST_DRIVER := walk-ast

all: $(AST_DRIVER)

%.o: %.cpp $(DEPS)
	$(CXX) -c $< $(CXXFLAGS)

$(AST_DRIVER): $(AST_OBJ)
	$(CXX) $(AST_OBJ) -o $@ $(LDFLAGS)

TEST_DIR := test
TEST_C := $(TEST_DIR)/hello.c
TEST_C_OBJ := $(TEST_DIR)/outfile-c
# TEST_CXX := $(TEST_DIR)/hello.cpp
# TEST_CXX_OBJ := $(TEST_DIR)/outfile-cpp

# just c for now; we'll add c++ support later
check: check-c
# check: check-c check-cpp

# TODO: this shouldn't need to be here
INCLUDE_ARG := -I/usr/lib/clang/3.6.0/include

$(TEST_C_OBJ): $(TEST_C) all
	./$(AST_DRIVER) $< $(INCLUDE_ARG)
check-c: $(TEST_C_OBJ)

# $(TEST_CXX_OBJ): $(TEST_CXX) all
# 	./$(AST_DRIVER) $< -std=c++14 $(INCLUDE_ARG) > $(TEST_CXX_OBJ)
# check-cpp: $(TEST_CXX_OBJ)

clean:
	@rm -f $(AST_DRIVER)
	@rm -f $(AST_OBJ)
	@rm -f $(TEST_C_OBJ)
#	@rm -f $(TEST_CXX_OBJ)
