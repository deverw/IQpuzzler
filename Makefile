CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -std=c++11 -fdiagnostics-color=always
LDFLAGS=-g

BIN=bin
SRC=src
INCLUDE=include

RECT=IQpuzzler_rect
DIAG=IQpuzzler_diag
PYRA=IQpuzzler_pyra
READ=IQpuzzler_read_input.cpp

all: $(BIN)/$(RECT) $(BIN)/$(DIAG) $(BIN)/$(PYRA)

$(BIN)/$(RECT): $(SRC)/$(RECT).cpp $(SRC)/$(READ)
	$(CXX) $(LDFLAGS) -I$(INCLUDE) $^ -o $@

$(BIN)/$(DIAG): $(SRC)/$(DIAG).cpp $(SRC)/$(READ)
	$(CXX) $(LDFLAGS) -I$(INCLUDE) $^ -o $@

$(BIN)/$(PYRA): $(SRC)/$(PYRA).cpp $(SRC)/$(READ)
	$(CXX) $(LDFLAGS) -I$(INCLUDE) $^ -o $@	

clean:
	$(RM) $(BIN)/*