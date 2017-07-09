/*
 * Backups_KEY.h
 *
 *  Created on: 2016-11-6
 *      Author: Administrator
 */

#ifndef BACKUPS_KEY_H_
#define BACKUPS_KEY_H_

/**************************************�궨��************************************************/
//#define SET_KY4		GpioDataRegs.GPASET.bit.GPIO0 = 1						//Y4����
#define SET_KY3			GpioDataRegs.GPBSET.bit.GPIO48 = 1    						//Y3����
#define SET_KY2			GpioDataRegs.GPBSET.bit.GPIO49 = 1						//Y2����
#define SET_KY1			GpioDataRegs.GPBSET.bit.GPIO50 = 1						//Y1����

//#define RST_KY4		GpioDataRegs.GPACLEAR.bit.GPIO0 = 1						//Y4����
#define RST_KY3			GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1 					//Y3����
#define RST_KY2			GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1 						//Y2����
#define RST_KY1			GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1						//Y1����

//#define KX4_STATUS	 	GpioDataRegs.GPBDAT.bit.GPIO50							//X4״̬
#define KX3_STATUS	 	GpioDataRegs.GPBDAT.bit.GPIO51							//X3״̬
#define KX2_STATUS	 	GpioDataRegs.GPBDAT.bit.GPIO52							//X2״̬
#define KX1_STATUS	 	GpioDataRegs.GPBDAT.bit.GPIO53							//X1״̬
/*****************************************************************************************************/

/**************************************�궨��************************************************/
// LED1~8��ZQ28335���Ҳࣺ���ϵ������α��,  L��ƽ����
// DSP���ƶ˿�: GPIO0--->LED1,...,GPIO5--->LED6��
//           GPIO12---LED7��GPIO36---LED8.
#define LED1_OFF		GpioDataRegs.GPASET.bit.GPIO0 = 1							//LED D10 ����
#define LED1_ON			GpioDataRegs.GPACLEAR.bit.GPIO0 = 1						//LED D10 Ϩ��

#define LED2_OFF		GpioDataRegs.GPASET.bit.GPIO1   = 1							//LED D11 ����
#define LED2_ON			GpioDataRegs.GPACLEAR.bit.GPIO1 = 1						//LED D11 Ϩ��

#define LED3_OFF		GpioDataRegs.GPASET.bit.GPIO2 = 1							//LED D12 ����
#define LED3_ON			GpioDataRegs.GPACLEAR.bit.GPIO2 = 1						//LED D12 Ϩ��

#define LED4_OFF		GpioDataRegs.GPASET.bit.GPIO3 = 1							//LED D13 ����
#define LED4_ON			GpioDataRegs.GPACLEAR.bit.GPIO3 = 1						//LED D13 Ϩ��

#define LED5_OFF		GpioDataRegs.GPASET.bit.GPIO4 = 1							//LED D14 ����
#define LED5_ON			GpioDataRegs.GPACLEAR.bit.GPIO4 = 1						//LED D14 Ϩ��

#define LED6_OFF		GpioDataRegs.GPASET.bit.GPIO5 = 1							//LED D15 ����
#define LED6_ON			GpioDataRegs.GPACLEAR.bit.GPIO5 = 1						//LED D15 Ϩ��

#define LED7_OFF		GpioDataRegs.GPASET.bit.GPIO12 = 1							//LED D16 ����
#define LED7_ON			GpioDataRegs.GPACLEAR.bit.GPIO12 = 1						//LED D16 Ϩ��

#define LED8_OFF		GpioDataRegs.GPBSET.bit.GPIO36 = 1							//LED D17 ����
#define LED8_ON			GpioDataRegs.GPBCLEAR.bit.GPIO36 = 1							//LED D17 Ϩ��

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
