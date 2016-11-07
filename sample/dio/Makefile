#CC = gcc
CC = arm-linux-gnueabi-gcc
LD = arm-linux-gnueabi-ld

INC = -I../../include/
BAT = -DCONFIG_DIO_BL

LIBCPSNAME=CpsDio

SRC_NAME001=dio
SRC_NAME002=dio_bl

all:${SRC_NAME001}.o ${SRC_NAME002}.o



${SRC_NAME001}.o:	
	${CC} ${INC} -o ${SRC_NAME001} ${SRC_NAME001}.c -l$(LIBCPSNAME)

${SRC_NAME002}.o:
	${CC} ${INC} -o ${SRC_NAME002} ${BAT} ${SRC_NAME001}.c -l$(LIBCPSNAME)

clean:
	rm -f ${SRC_NAME001} ${SRC_NAME002} 
