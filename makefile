include commands.mk

CFLAGS  := -std=c99 -O2 -fPIC -Wall
LDFLAGS := -lm 

SRC  = $(wildcard *.c)
OBJ  = $(foreach obj, $(SRC:.c=.o), $(notdir $(obj)))
DEP  = $(SRC:.c=.d)

PREFIX  ?= /usr/local
BINDIR  ?= $(PREFIX)/bin

ifdef DEBUG
CFLAGS += -ggdb
endif

commit = $(shell ./hash.sh)
ifneq ($(commit), UNKNOWN)
CFLAGS += -DCOMMIT="\"$(commit)\""
endif

.PHONY: all clean

all: gcalc

gcalc: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -Wp,-MMD,$*.d -c $(CFLAGS) -o $@ $< 

clean:
	$(RM) $(DEP)
	$(RM) $(OBJ)
	$(RM) gcalc

install: gcalc
	$(INSTALL_PROGRAM) gcalc $(BINDIR)

uninstall:
	$(RM) $(BINDIR)/gcalc

-include $(DEP)

