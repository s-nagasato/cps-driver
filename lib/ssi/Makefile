#CC = gcc
#LD = ld
#LIB_DIR=/usr/lib
CC = arm-linux-gnueabi-gcc
LD = arm-linux-gnueabi-ld
LIB_DIR=/usr/arm-linux-gnueabi/lib/
CFLAGS= -g -Wall
VERSION =1.0.0

all:libcpsssi.o libCpsSsi.so

libcpsssi.o:	
	${CC} -z defs libcpsssi.c -c -fPIC -o libcpsssi.o -lm

libCpsSsi.so:
	${CC} -z defs -shared -O2 -Wl,-soname,libCpsSsi.so -o libCpsSsi.so.$(VERSION) libcpsssi.o -lm

install:
	cp libCpsSsi.so.$(VERSION) $(LIB_DIR)
	ldconfig

install-cross:
	rm -f libCpsSsi.so
	ln -s libCpsSsi.so.$(VERSION) libCpsSsi.so
	cp libCpsSsi.so.$(VERSION) $(LIB_DIR)
	mv libCpsSsi.so $(LIB_DIR)

release_copy:
	cp ./libCpsSsi.so.$(VERSION) ../release/

clean:
	rm -f *.o *.so *.so.$(VERSION)
