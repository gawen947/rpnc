CC=gcc
RM=rm -f
INSTALL=install
OBJS=gcalc.o
LIBS=-lm
PREF=/usr/local/
BIN=$(PREF)bin/

gcalc : $(OBJ) $(LIBS)
.PHONY : clean install


clean:
	$(RM) $(OBJ) gcalc

install:
	$(INSTALL) gcalc $(BIN)
