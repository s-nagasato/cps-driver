/*** cps.h ******************************/


extern unsigned char contec_mcs341_controller_cpsDevicesInit(void);
extern unsigned char contec_mcs341_device_FindCategory( int *startIndex,int CategoryNum );
extern unsigned char contec_mcs341_device_IsCategory( int targetDevNum ,int CategoryNum );
extern unsigned char contec_mcs341_device_FindDevice( int *startIndex, int DeviceNum );
extern unsigned short contec_mcs341_device_productid_get( int dev );
extern unsigned char contec_mcs341_device_mirror_get( int dev , int num );
extern unsigned char contec_mcs341_device_deviceNum_get( unsigned long baseAddr );
extern unsigned char contec_mcs341_device_serial_channel_get( unsigned long baseAddr );
extern unsigned char contec_mcs341_controller_setInterrupt( int GroupNum , int isEnable );
extern unsigned char contec_mcs341_controller_getDeviceNum( void );
/*********************************************************/
