ROOT = ../bcl

CPPFLAGS = -I$(ROOT)/include -I.
CFLAGS   = -Wall -Wextra -Werror -std=c99
LDFLAGS  = -L$(ROOT)/lib
LDLIBS   = -lbcl -lm

BIN = \
	move\
	shear\
	scroll\
	flip\
	rotate\
	wave\
	sphere

.PHONY: all
all: $(BIN)

shear: domain.o

rotate: domain.o

wave: domain.o

sphere: domain.o

.PHONY: clean cleanall
clean:
	$(RM) *.o
cleanall: clean
	$(RM) $(BIN)
