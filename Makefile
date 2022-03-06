CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -std=c++11 -fdiagnostics-color=always
LDFLAGS=-g

BIN=bin
SRC=src
INCLUDE=include

SRCS=IQpuzzler_diag.cpp IQpuzzler_read_input.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: $(BIN)/IQpuzzler_rect $(BIN)/IQpuzzler_diag $(BIN)/IQpuzzler_pyra

$(BIN)/IQpuzzler_diag: $(SRC)/IQpuzzler_diag.cpp $(SRC)/IQpuzzler_read_input.cpp
	$(CXX) $(LDFLAGS) -I$(INCLUDE) $^ -o $@

$(BIN)/IQpuzzler_rect: $(SRC)/IQpuzzler_rect.cpp $(SRC)/IQpuzzler_read_input.cpp
	$(CXX) $(LDFLAGS) -I$(INCLUDE) $^ -o $@

$(BIN)/IQpuzzler_pyra: $(SRC)/IQpuzzler_pyra.cpp $(SRC)/IQpuzzler_read_input.cpp
	$(CXX) $(LDFLAGS) -I$(INCLUDE) $^ -o $@	

clean:
	$(RM) $(BIN)/*