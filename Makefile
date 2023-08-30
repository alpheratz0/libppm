.POSIX:
.PHONY: all clean install

include config.mk

all: libppm.a

ppm.o: ppm.c ppm.h

libppm.a: ppm.o
	$(AR) -rcs libppm.a ppm.o

example: ppm.o example.o
	$(CC) example.o ppm.o -o example

install: libppm.a
	rm -rf $(PREFIX)/include/ppm
	mkdir $(PREFIX)/include/ppm
	cp ppm.h $(PREFIX)/include/ppm
	cp libppm.a $(PREFIX)/lib

clean:
	rm -f *.o libppm.a example
