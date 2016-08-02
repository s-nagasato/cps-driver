#CC = gcc
#LD = ld
#LIB_DIR=/usr/lib
CC = arm-linux-gnueabi-gcc
LD = arm-linux-gnueabi-ld
LIB_DIR=/usr/arm-linux-gnueabi/lib/
CFLAGS= -g -Wall
VERSION =1.0.1

all:libcpsdio.o libCpsDio.so

libcpsdio.o:	
	${CC} libcpsdio.c -c -fPIC -o libcpsdio.o

libCpsDio.so:
	${CC} -shared -O2 -Wl,-soname,libCpsDio.so -o libCpsDio.so.$(VERSION) libcpsdio.o

install:
	cp libCpsDio.so.$(VERSION) $(LIB_DIR)
	ldconfig

install-cross:
	rm -f libCpsDio.so
	ln -s libCpsDio.so.$(VERSION) libCpsDio.so
	cp libCpsDio.so.$(VERSION) $(LIB_DIR)
	mv libCpsDio.so $(LIB_DIR)

release_copy:
	cp ./libCpsDio.so.$(VERSION) ../release/

clean:
	rm -f *.o *.so *.so.$(VERSION)
