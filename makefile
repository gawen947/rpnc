CC=gcc
RM=rm -f
INSTALL=install
SRC=gcalc.c
COMMIT=$(shell ./hash.sh)
CFLAGS=-std=c99 -O2
LIBS=-lm
PREF=/usr/local/
BIN=$(PREF)bin/

all: gcalc strip

gcalc: $(SRC)
	@echo -n COMPILING
	@$(CC) $(LIBS) -DCOMMIT="\"$(COMMIT)\"" $(CFLAGS) $^ -o $@
	@echo ... done.
.PHONY : clean install

clean:
	$(RM) $(OBJ) gcalc

strip:
	@echo -n STRIPING
	@strip gcalc
	@echo ... done.

install:
	$(INSTALL) gcalc $(BIN)
