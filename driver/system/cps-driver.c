/*
 *  Base Driver for CONPROSYS (only) by CONTEC .
 * Version 1.0.0
 *
 *  Copyright (C) 2015 Syunsuke Okamoto.<okamoto@contec.jp>
 *
 * cps-driver is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 *
 * cps-driver driver program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <asm/io.h>
#include <linux/device.h>
#include <asm/delay.h>



MODULE_LICENSE("GPL");
MODULE_ALIAS("CONTEC CONPROSYS BASE Driver");
MODULE_AUTHOR("CONTEC");

#include "../../include/cps.h"
#include "../../include/cps_ids.h"
#include "../../include/cps_common_io.h"

#if 1 
#define DEBUG_INITMEMORY(fmt...)	printk(fmt)
#else
#define DEBUG_INITMEMORY(fmt...)	do { } while (0)
#endif

#if 0 
#define DEBUG_EXITMEMORY(fmt...)        printk(fmt)
#else
#define DEBUG_EXITMEMORY(fmt...)        do { } while (0)
#endif

#if 0
#define DEBUG_ADDR_VAL_OUT(fmt...)        printk(fmt)
#else
#define DEBUG_ADDR_VAL_OUT(fmt...)        do { } while (0)
#endif

#if 0
#define DEBUG_ADDR_VAL_IN(fmt...)        printk(fmt)
#else
#define DEBUG_ADDR_VAL_IN(fmt...)        do { } while (0)
#endif


static unsigned char __iomem *map_baseaddr ;
static unsigned char __iomem *map_devbaseaddr[CPS_DEVICE_MAX_NUM];

static unsigned char deviceNumber;

static unsigned char mcs341_deviceInterrupt[6];

static const int AM335X_IRQ_NMI=7;

void *am335x_irq_dev_id = (void *)&AM335X_IRQ_NMI;

static int CompleteDevIrqs ;

irqreturn_t am335x_nmi_isr(int irq, void *dev_instance){

	int cnt = 0;

	int GrpNo, DevIrq;


	if(printk_ratelimit()){
		printk("NMI IRQ interrupt !!\n");
	}
/*
	cnt = CompleteDevIrqs;
 
	// 2015.11.13 add
	while ( cnt < 32 ){
		GrpNo = cnt / 8;
		DevIrq = contec_mcs341_controller_getInterrupt(GrpNo);
		if( DevIrq ){
			//Interrupt 
			for( devcnt = cnt % 8; cnt < 8; cnt ++ ){
				contec_mcs341_device_IsCategory( cnt );
			}		

		}else{
			cnt = GrpNo * 8 ;
		}
	}
*/
	return IRQ_HANDLED;
}


static void contec_mcs341_outb(unsigned int addr, unsigned char valb )
{
	DEBUG_ADDR_VAL_OUT(KERN_INFO " cps-system: Offset Address : %x Value %x \n", addr, valb );
	cps_common_outb( (unsigned long)(map_baseaddr + addr), valb );
}

static void contec_mcs341_inpb(unsigned int addr, unsigned char *valb )
{
	cps_common_inpb( (unsigned long)(map_baseaddr + addr), valb );
	DEBUG_ADDR_VAL_IN(KERN_INFO " cps-system: Offset Address : %x Value %x\n", addr, *valb );
}

/***** SET Paramater's ***************/

static unsigned char contec_mcs341_controller_setPinMode(int Pin3g3, int Pin3g4, int Pincts, int Pinrts  ){
	unsigned char valb = 
		CPS_MCS341_SETPINMODE_3G3(Pin3g3) | 
		CPS_MCS341_SETPINMODE_3G4(Pin3g4) |
		CPS_MCS341_SETPINMODE_CTSSUB(Pincts) | 
		CPS_MCS341_SETPINMODE_RTSSUB(Pinrts);
	contec_mcs341_outb( CPS_CONTROLLER_MCS341_SETPINMODE_WADDR, valb );
	//cps_common_outb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_SETPINMODE_WADDR), valb );

	return 0;
}
EXPORT_SYMBOL_GPL(contec_mcs341_controller_setPinMode);

static unsigned char contec_mcs341_controller_setInterrupt( int GroupNum , int isEnable ){

	unsigned char valb;

	if ( GroupNum >= 4 ) return 1;
	if ( isEnable > 1 || isEnable < 0 ) return 1;

	valb = mcs341_deviceInterrupt[0] & ~CPS_MCS341_INTERRUPT_GROUP_SET( GroupNum,isEnable );

	contec_mcs341_outb(CPS_CONTROLLER_MCS341_INTERRUPT_ADDR(0), valb);

	return 0;
}
EXPORT_SYMBOL_GPL(contec_mcs341_controller_setInterrupt);

static unsigned char contec_mcs341_controller_setDioDirection( int dioNum , int isDir ){

	unsigned char valb;

	if ( dioNum >= 4 && dioNum < 0 ) return 1;

	if ( isDir < 0 || 1 < isDir )	return 2;
	contec_mcs341_inpb(CPS_CONTROLLER_MCS341_DIO_DIRECTION_ADDR, &valb);

	valb = ( valb & 0xF0 ) | CPS_MCS341_DIO_DIRECTION_SET( isDir, dioNum );

	contec_mcs341_outb(CPS_CONTROLLER_MCS341_DIO_DIRECTION_ADDR, valb);
	return 0;
}
EXPORT_SYMBOL_GPL(contec_mcs341_controller_setDioDirection);


static unsigned char contec_mcs341_controller_setDioFilter( int FilterNum ){

	unsigned char valb;

	if ( FilterNum < 0 || 15 < FilterNum )	return 1;

	contec_mcs341_inpb(CPS_CONTROLLER_MCS341_DIO_DIRECTION_ADDR, &valb);

	valb = ( valb & 0x0F ) | CPS_MCS341_DIO_FILTER_SET(FilterNum);

	contec_mcs341_outb(CPS_CONTROLLER_MCS341_DIO_DIRECTION_ADDR, valb);

	return 0;
}
EXPORT_SYMBOL_GPL(contec_mcs341_controller_setDioFilter);

static unsigned char contec_mcs341_controller_setDoValue( int dioNum, int value ){

	unsigned char valb;

	valb = CPS_MCS341_DIO_DOVALUE_SET( dioNum, value );

	cps_common_outb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_DIO_VALUE_ADDR), valb );

	return 0;
}
EXPORT_SYMBOL_GPL(contec_mcs341_controller_setDoValue);


/***** GET Paramater's ***************/

static unsigned char contec_mcs341_controller_getProductVersion(void){
	unsigned char valb = 0;

	cps_common_inpb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_PRODUCTVERSION_ADDR), &valb );

	return CPS_MCS341_PRODUCT_VERSION(valb);
}
EXPORT_SYMBOL_GPL(contec_mcs341_controller_getProductVersion);

static unsigned char contec_mcs341_controller_getProductType(void){
	unsigned char valb = 0;

	cps_common_inpb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_PRODUCTVERSION_ADDR), &valb );

	return CPS_MCS341_PRODUCT_TYPE(valb);
}
EXPORT_SYMBOL_GPL(contec_mcs341_controller_getProductType);

static unsigned short contec_mcs341_controller_getFpgaVersion(void){
	unsigned char valb = 0;

	cps_common_inpb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_FPGAVERSION_ADDR), &valb );
	return valb;
}
EXPORT_SYMBOL_GPL(contec_mcs341_controller_getFpgaVersion);


static unsigned char contec_mcs341_controller_getUnitId(void){
	unsigned char valb = 0;

	cps_common_inpb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_ROTARYSW_RADDR), &valb );

	return CPS_MCS341_ROTARYSW_UNITID(valb);
}
EXPORT_SYMBOL_GPL(contec_mcs341_controller_getUnitId);

static unsigned char contec_mcs341_controller_getGroupId(void){
	unsigned char valb = 0;

	cps_common_inpb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_ROTARYSW_RADDR), &valb );

	return CPS_MCS341_ROTARYSW_GROUPID(valb);
}
EXPORT_SYMBOL_GPL(contec_mcs341_controller_getGroupId);

static unsigned char contec_mcs341_controller_getDeviceNum(void){
	unsigned char valb = 0;

	cps_common_inpb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_DEVICENUM_RADDR), &valb );

	return CPS_MCS341_DEVICENUM_VALUE(valb);
}
EXPORT_SYMBOL_GPL(contec_mcs341_controller_getDeviceNum);

static unsigned char contec_mcs341_controller_getInterrupt( int GroupNum ){

	unsigned char valb;

	if ( GroupNum >= 8 ) return 1;

	contec_mcs341_inpb(CPS_CONTROLLER_MCS341_INTERRUPT_ADDR(0), &valb);

	return CPS_MCS341_INTERRUPT_GROUP_GET(GroupNum, valb);
}
EXPORT_SYMBOL_GPL(contec_mcs341_controller_getInterrupt);

static unsigned char contec_mcs341_controller_getDoEchoValue( int dioNum ){

	unsigned char valb = 0;
	
	if( dioNum < 0 || dioNum > 3 ) return -1;
	cps_common_inpb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_DIO_VALUE_ADDR), &valb );

	return CPS_MCS341_DIO_DOECHOVALUE_GET(dioNum, valb);
}
EXPORT_SYMBOL_GPL(contec_mcs341_controller_getDoEchoValue);

static unsigned char contec_mcs341_controller_getDiValue( int dioNum ){

	unsigned char valb = 0;

	if( dioNum < 0 || dioNum > 3 ) return -1;
	cps_common_inpb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_DIO_VALUE_ADDR), &valb );

	return CPS_MCS341_DIO_DIVALUE_GET(dioNum, valb);
}
EXPORT_SYMBOL_GPL(contec_mcs341_controller_getDiValue);

/*
	@note :: Id-Sel of MCS341 complete sub routine.
*/ 
static void contec_mcs341_device_idsel_complete( void ){
	int cnt;
	int nInterrupt;

	deviceNumber = contec_mcs341_controller_getDeviceNum();
	DEBUG_INITMEMORY(KERN_INFO " cps-system : device number : %d \n", deviceNumber );
	if( deviceNumber != 0x3f && deviceNumber != 0 ){
		for( cnt = 0; cnt < deviceNumber ; cnt++ ){
			 map_devbaseaddr[cnt] = 
				cps_common_mem_alloc( (0x08000000 + (cnt + 1) * 0x100 ),
				0x10,
				"cps-mcs341-common-dev",
				CPS_COMMON_MEM_NONREGION	 );
				DEBUG_INITMEMORY(KERN_INFO "cps-system: device %d Address:%lx \n",cnt,(unsigned long)map_devbaseaddr[cnt]);
		}
		// 
		cps_common_outb( (unsigned long) ( map_devbaseaddr[0] ) , (deviceNumber | 0x80 ) );
		udelay( 1000 );

		nInterrupt = (deviceNumber / 4) + 1;
		for( cnt = 0; cnt < nInterrupt; cnt++ )
			contec_mcs341_controller_setInterrupt( 0 , cnt );
	}
}

/*
	@note :: cps-driver CPS-Devices Initialize. Success 0 , Failed not 0.
*/ 
static unsigned char contec_mcs341_controller_cpsDevicesInit(void){
	unsigned char valb = 0;
	unsigned int timeout = 0;
	cps_common_inpb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_SYSTEMINIT_ADDR), &valb ); 
	if( CPS_MCS341_SYSTEMINIT_BUSY( valb ) ){
		cps_common_outb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_SYSTEMINIT_ADDR),
			CPS_MCS341_SYSTEMINIT_SETRESET );

		do{
			udelay(5);
			cps_common_inpb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_SYSTEMINIT_ADDR), &valb );
			if( timeout >= CPS_DEVICE_INIT_TIMEOUT ) return 1;
			timeout ++;
		}while( !(valb & CPS_MCS341_SYSTEMINIT_INIT_END) );

		/*
			When many devices was connected more than 15, getDeviceNumber gets 14 values.
			CPS-MCS341 must wait 1 msec.
		*/ 		
		udelay( 1000 );	
		contec_mcs341_device_idsel_complete();

/*
		cps_common_outb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_RESET_WADDR) ,
			CPS_MCS341_RESET_SET_IDSEL_COMPLETE );
		do{
			udelay(5);
			cps_common_inpb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_SYSTEMINIT_ADDR), &valb);
		}while( valb & CPS_MCS341_SYSTEMINIT_INITBUSY );
*/
		cps_common_outb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_SYSTEMINIT_ADDR),
			( CPS_MCS341_SYSTEMINIT_SETRESET | CPS_MCS341_SYSTEMINIT_SETINTERRUPT) );
		
		timeout = 0;
		do{
			udelay(5);
			cps_common_inpb( (unsigned long)(map_baseaddr + CPS_CONTROLLER_MCS341_SYSTEMINIT_ADDR), &valb );
			if( timeout >= CPS_DEVICE_INIT_TIMEOUT ) return 2;
			timeout ++; 
		}while( !(valb & CPS_MCS341_SYSTEMINIT_INTERRUPT_END)  );
	}
		/*
			When many devices was connected more than 15, getDeviceNumber gets 14 values.
			CPS-MCS341 must wait 1 msec.
		*/ 	
	udelay( 1000 );
	return 0;

}
EXPORT_SYMBOL_GPL(contec_mcs341_controller_cpsDevicesInit);

/*
	@param startIndex : Start Index ( >= 1 )
	@param CategoryNum : Product Category Number
	@note :: Success : 1 , Fail : 0
*/  
static unsigned char contec_mcs341_device_FindCategory( int *startIndex,int CategoryNum ){

	int cnt;
	unsigned char valb;
	
	if( *startIndex < 1 || 
	    *startIndex >= deviceNumber
	 ) return 0;

	for( cnt = *startIndex ; cnt < CPS_DEVICE_MAX_NUM ; cnt ++ ){
		cps_common_inpb( (unsigned long)(map_devbaseaddr[cnt] + CPS_DEVICE_COMMON_CATEGORY_ADDR ), &valb );
		if ( valb  == CategoryNum ) break;
 	} 

	if (cnt > CPS_DEVICE_MAX_NUM ) return 0;
	else return 1;

}
EXPORT_SYMBOL_GPL(contec_mcs341_device_FindCategory);

/*
	@param startIndex : Start Index ( >= 1 )
	@param CategoryNum : Product Category Number
	@note :: Success : 1 , Fail : 0
*/  
static unsigned char contec_mcs341_device_IsCategory( int targetDevNum ,int CategoryNum ){

	unsigned char valb;
	
	if( targetDevNum >= CPS_DEVICE_MAX_NUM ||
		targetDevNum >= deviceNumber ) return 0;

	cps_common_inpb( (unsigned long)(map_devbaseaddr[targetDevNum] + CPS_DEVICE_COMMON_CATEGORY_ADDR ), &valb );
	if ( valb == CategoryNum ) return 1;

 return 0;
}
EXPORT_SYMBOL_GPL(contec_mcs341_device_IsCategory);

/*
	@param startIndex : Start Index ( >= 1 )
	@param DeviceNum : Product Device Number
	@note :: Success : Find Physical Number , Fail : 0
*/  
static unsigned char contec_mcs341_device_FindDevice( int *startIndex, int DeviceNum ){

	int cnt;
	unsigned short valw;

	if( *startIndex < 1 || *startIndex >= deviceNumber ) return 0;

	for( cnt = *startIndex ; cnt < CPS_DEVICE_MAX_NUM ; cnt ++ ){
		cps_common_inpw( (unsigned long)(map_devbaseaddr[cnt] + CPS_DEVICE_COMMON_PRODUCTID_ADDR), &valw );
		if ( valw == DeviceNum ) break;
 	} 

	if (cnt > CPS_DEVICE_MAX_NUM ) return 0;
	else return cnt;

}
EXPORT_SYMBOL_GPL(contec_mcs341_device_FindDevice);

/*
	@param dev : Target DeviceNumber ( < deviceNumber )
	@note :: Success : product Id , Fail : 0
*/  
static unsigned short contec_mcs341_device_productid_get( int dev ){

	unsigned short valw;
	
	if( dev >= deviceNumber ) return 0;
	cps_common_inpw( (unsigned long)(map_devbaseaddr[dev] + CPS_DEVICE_COMMON_PRODUCTID_ADDR), &valw );

	return valw;
}
EXPORT_SYMBOL_GPL(contec_mcs341_device_productid_get);

/*
	@param dev : Target DeviceNumber ( < deviceNumber )
	@param num : Mirror Address ( 0 or 1 )
	@note :: Success : Mirroring Register Values Get , Fail : 0
*/  
static unsigned char contec_mcs341_device_mirror_get( int dev , int num ){

	unsigned char valb;

	if( dev >= deviceNumber ) return 0;
	if( num < 0 || num > 1 ) return 0;
	cps_common_inpb( (unsigned long)(map_devbaseaddr[dev] + CPS_DEVICE_COMMON_MIRROR_REG_ADDR(num)), &valb );

	return valb;
}
EXPORT_SYMBOL_GPL(contec_mcs341_device_mirror_get);

/*
	@param BaseAddr : Base Address ( not Virtual Memory Address )
	@note :: Is used cpscom driver. return Target Device Number
*/ 
static unsigned char contec_mcs341_device_deviceNum_get( unsigned long baseAddr )
{
		return (  (baseAddr & 0x00002F00 ) >> 8 );
}
EXPORT_SYMBOL_GPL(contec_mcs341_device_deviceNum_get);

/*
	@param BaseAddr : Base Address ( not Virtual Memory Address )
	@note :: Is used cpscom driver. return Current Serial Channel. 
*/ 
static unsigned char contec_mcs341_device_serial_channel_get( unsigned long baseAddr )
{
		return (  ( ( baseAddr - 0x10 ) & 0x00000038 ) >> 3);
}
EXPORT_SYMBOL_GPL(contec_mcs341_device_serial_channel_get);

/*
	@note :: cps-driver initialize. 
*/ 
static int contec_mcs341_controller_init(void)
{
	int ret = 0;
	int cnt = 0;
	void __iomem *allocaddr;

	allocaddr = cps_common_mem_alloc(
		 0x08000000,
		 0x100,
		 "cps-mcs341",
		 CPS_COMMON_MEM_REGION	 );

	if( !allocaddr ) return -ENOMEM;
	else map_baseaddr = allocaddr;
	
	cps_fpga_init();

	contec_mcs341_controller_cpsDevicesInit();

	return ret;
}

/*
	@note :: cps-driver exit. 
*/ 
static void contec_mcs341_controller_exit(void)
{
	int cnt = 0;

	if( deviceNumber != 0x3f ){
		for( cnt = 0; cnt < deviceNumber ; cnt++ ){
			cps_common_mem_release( (0x08000000 + (cnt + 1) * 0x100 ),
				0x10,
				map_devbaseaddr[cnt],
				CPS_COMMON_MEM_NONREGION	 );
		}
	} 

	cps_common_mem_release( 0x08000000,
		0x100,
		map_baseaddr ,
		CPS_COMMON_MEM_REGION);
	return;
}

module_init(contec_mcs341_controller_init);
module_exit(contec_mcs341_controller_exit);



