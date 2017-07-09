/*
 * Backups_KEY.h
 *
 *  Created on: 2016-11-6
 *      Author: Administrator
 */

#ifndef BACKUPS_KEY_H_
#define BACKUPS_KEY_H_

/**************************************宏定义************************************************/
//#define SET_KY4		GpioDataRegs.GPASET.bit.GPIO0 = 1						//Y4拉高
#define SET_KY3			GpioDataRegs.GPBSET.bit.GPIO48 = 1    						//Y3拉高
#define SET_KY2			GpioDataRegs.GPBSET.bit.GPIO49 = 1						//Y2拉高
#define SET_KY1			GpioDataRegs.GPBSET.bit.GPIO50 = 1						//Y1拉高

//#define RST_KY4		GpioDataRegs.GPACLEAR.bit.GPIO0 = 1						//Y4拉低
#define RST_KY3			GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1 					//Y3拉低
#define RST_KY2			GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1 						//Y2拉低
#define RST_KY1			GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1						//Y1拉低

//#define KX4_STATUS	 	GpioDataRegs.GPBDAT.bit.GPIO50							//X4状态
#define KX3_STATUS	 	GpioDataRegs.GPBDAT.bit.GPIO51							//X3状态
#define KX2_STATUS	 	GpioDataRegs.GPBDAT.bit.GPIO52							//X2状态
#define KX1_STATUS	 	GpioDataRegs.GPBDAT.bit.GPIO53							//X1状态
/*****************************************************************************************************/

/**************************************宏定义************************************************/
// LED1~8在ZQ28335板右侧：从上到下依次编号,  L电平亮。
// DSP控制端口: GPIO0--->LED1,...,GPIO5--->LED6，
//           GPIO12---LED7，GPIO36---LED8.
#define LED1_OFF		GpioDataRegs.GPASET.bit.GPIO0 = 1							//LED D10 点亮
#define LED1_ON			GpioDataRegs.GPACLEAR.bit.GPIO0 = 1						//LED D10 熄灭

#define LED2_OFF		GpioDataRegs.GPASET.bit.GPIO1   = 1							//LED D11 点亮
#define LED2_ON			GpioDataRegs.GPACLEAR.bit.GPIO1 = 1						//LED D11 熄灭

#define LED3_OFF		GpioDataRegs.GPASET.bit.GPIO2 = 1							//LED D12 点亮
#define LED3_ON			GpioDataRegs.GPACLEAR.bit.GPIO2 = 1						//LED D12 熄灭

#define LED4_OFF		GpioDataRegs.GPASET.bit.GPIO3 = 1							//LED D13 点亮
#define LED4_ON			GpioDataRegs.GPACLEAR.bit.GPIO3 = 1						//LED D13 熄灭

#define LED5_OFF		GpioDataRegs.GPASET.bit.GPIO4 = 1							//LED D14 点亮
#define LED5_ON			GpioDataRegs.GPACLEAR.bit.GPIO4 = 1						//LED D14 熄灭

#define LED6_OFF		GpioDataRegs.GPASET.bit.GPIO5 = 1							//LED D15 点亮
#define LED6_ON			GpioDataRegs.GPACLEAR.bit.GPIO5 = 1						//LED D15 熄灭

#define LED7_OFF		GpioDataRegs.GPASET.bit.GPIO12 = 1							//LED D16 点亮
#define LED7_ON			GpioDataRegs.GPACLEAR.bit.GPIO12 = 1						//LED D16 熄灭

#define LED8_OFF		GpioDataRegs.GPBSET.bit.GPIO36 = 1							//LED D17 点亮
#define LED8_ON			GpioDataRegs.GPBCLEAR.bit.GPIO36 = 1							//LED D17 熄灭

#define DELAY_TIME		2000000

extern void Init_KeyGpio(void);
void delay(Uint32 t);
void RstAllKY(void);
void GetKX(void);
void GetKX_On(void);
void SetKY(Uint16 x);
void RstKY(Uint16 x);
extern Uint16 GetKey(void);

#endif /* BACKUPS_KEY_H_ */
