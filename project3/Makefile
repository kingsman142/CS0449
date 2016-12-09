IDIR=../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=
LDRI=

LIBS=-lm

_DEPS=mymalloc.h
DEPS=$(patsubst %,%,$(_DEPS))

_OBJ=mymalloc.o mytestdriver.o mallocdrv.o
OBJ=$(patsubst %,%,$(_OBJ))

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hellomake: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o *~ core $(INCDIR)/*~
