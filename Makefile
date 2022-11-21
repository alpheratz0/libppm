.POSIX:
.PHONY: all clean install

include config.mk

all: libppm.a

ppm.o: ppm.c ppm.h

libppm.a: ppm.o
	$(AR) -rcs libppm.a ppm.o

install: libppm.a
	rm -rf $(PREFIX)/include/ppm
	mkdir $(PREFIX)/include/ppm
	cp ppm.h $(PREFIX)/include/ppm
	cp libppm.a $(PREFIX)/lib

clean:
	rm -f ppm.o libppm.a
