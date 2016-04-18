#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#include "../../include/libcpsdio.h"

int main(int argc, char *argv[]){
	short Id;

	unsigned char dat, dat2;

	/* デバイスをopenする */
	ContecCpsDioInit("cpsdio0", &Id);

	dat = 0x55;

	ContecCpsDioOutByte(Id, 0, dat );

	ContecCpsDioInpByte(Id, 0, &dat2 );

	if( dat == dat2 ) printf(" In = Out \n");
	else	printf(" In %x Out %x \n", dat, dat2 ); 

	/* デバイスを閉じる*/
	ContecCpsDioExit(Id);
	return 0;
}

