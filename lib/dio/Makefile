#CC = gcc
CC = arm-linux-gnueabi-gcc
CFLAGS= -g -Wall
VERSION =1.0.0

all:libcpsdio.o libCpsDio.so

libcpsdio.o:	
	${CC} libcpsdio.c -c -fPIC -o libcpsdio.o

libCpsDio.so:
	${CC} -shared -O2 -Wl,-soname,libCpsDio.so -o libCpsDio.so.$(VERSION) libcpsdio.o
install:
	cp libCpsDio.so.1.0.0 /usr/lib/
	ldconfig
clean:
	rm -f *.o *.so
