#CC = gcc
CC = arm-linux-gnueabi-gcc

SRC_NAME01=cnt
SRC_NAME02=check_cnts

all:${SRC_NAME01}.o ${SRC_NAME02}.o



${SRC_NAME01}.o :	
	${CC} -o ${SRC_NAME01} ${SRC_NAME01}.c -lCpsCnt

${SRC_NAME02}.o :	
	${CC} -o ${SRC_NAME02} ${SRC_NAME02}.c -lCpsCnt

clean:
	rm -f ${SRC_NAME01} ${SRC_NAME02}

