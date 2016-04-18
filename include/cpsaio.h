/*** cpsaio.h ******************************/
#include <linux/ioctl.h>
#include <linux/sched.h>

/* structure */
typedef struct __cpsaio_device_data{
	char Name[32];
	unsigned int ProductNumber;
	unsigned int aiResolution;
	unsigned int aiChannel;
	unsigned int aoResolution;
	unsigned int aoChannel;
}CPSAIO_DEV_DATA, *PCPSAIO_DEV_DATA;
	
struct cpsaio_ioctl_arg{
	unsigned short val;
};


/*  Error Code */

#define AIO_ERR_SYS_MEMORY										20000
#define AIO_ERR_SYS_NOT_SUPPORTED					20001
#define AIO_ERR_SYS_BOARD_EXECUTING				20002
#define AIO_ERR_SYS_USING_OTHER_PROCESS	20003

/* Driver's Code */

#define CPSAIO_MAGIC	'f'

#define IOCTL_CPSAIO_INIT	_IORW(CPSAIO_MAGIC, 1, struct cpsaio_ioctl_arg)
#define IOCTL_CPSAIO_EXIT	_IORW(CPSAIO_MAGIC, 2, struct cpsaio_ioctl_arg)

#define IOCTL_CPSAIO_INDATA	_IOR(CPSAIO_MAGIC, 3, struct cpsaio_ioctl_arg)
#define IOCTL_CPSAIO_OUTDATA	_IOW(CPSAIO_MAGIC, 4, struct cpsaio_ioctl_arg)
#define IOCTL_CPSAIO_INSTATUS	_IOR(CPSAIO_MAGIC, 5, struct cpsaio_ioctl_arg)
#define IOCTL_CPSAIO_OUTSTATUS	_IOW(CPSAIO_MAGIC, 6, struct cpsaio_ioctl_arg)

/**************************************************/
