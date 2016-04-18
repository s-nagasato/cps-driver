#CC = gcc
CC = arm-linux-gnueabi-gcc

all:CalibWrite.o CalibRead.o CalibClear.o

CalibWrite.o:
	${CC} -o CalibWrite CalibWrite.c -lCpsSsi -lm

CalibRead.o:
	${CC} -o CalibRead CalibRead.c -lCpsSsi -lm

CalibClear.o:
	${CC} -o CalibClear CalibClear.c -lCpsSsi -lm

clean:
	rm -f ssi CalibWrite CalibRead CalibClear
