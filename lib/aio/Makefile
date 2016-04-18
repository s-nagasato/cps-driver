#CC = gcc
#LD = ld
#LIB_DIR=/usr/lib
CC = arm-linux-gnueabi-gcc
LD = arm-linux-gnueabi-ld
LIB_DIR=/usr/arm-linux-gnueabi/lib/
CFLAGS= -g -Wall
VERSION =1.0.4

all:libcpsaio.o libCpsAio.so

libcpsaio.o:	
	${CC} libcpsaio.c -c -fPIC -o libcpsaio.o -lm

libCpsAio.so:
	${CC} -shared -O2 -Wl,-soname,libCpsAio.so -o libCpsAio.so.$(VERSION) libcpsaio.o -lm

install:
	cp libCpsAio.so.$(VERSION) $(LIB_DIR)
	ldconfig

install-cross:
	rm -f libCpsAio.so
	ln -s libCpsAio.so.$(VERSION) libCpsAio.so
	cp libCpsAio.so.$(VERSION) $(LIB_DIR)
	mv libCpsAio.so $(LIB_DIR)

release_copy:
	cp ./libCpsAio.so.$(VERSION) ../release/

clean:
	rm -f *.o *.so *.so.$(VERSION)
