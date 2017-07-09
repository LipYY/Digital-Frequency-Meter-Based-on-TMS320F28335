/*
 * LCD12864.h
 *
 *  Created on: 2017-7-2
 *      Author: Administrator
 */

#ifndef LCD12864_H_
#define LCD12864_H_



// �궨��
#define uchar unsigned char

#define  BASIC_SET   	0x30           //����ָ�  00110000
#define  EXTEND_SET  	0x34           //��չָ�  00110100
#define  DRAW_ON     	0x36           //��ͼ��ʾ��  00110110
#define  DRAW_OFF    	0x34           //��ͼ��ʾ��  00110100
#define  DISP_ON    	0x0F           //��ʾON,�α�ON,�α�λ����ON 00001111

#define  execute_72us   delay_loop(400)     //ִ��ʱ��
#define  SCLK_quarter   delay_loop(40)       //�ķ�֮һSCLKʱ�� SCLK��LCD����ͨ��ʱ��
#define  SCLK_half      delay_loop(80)       //����֮һSCLKʱ�� SCLK��LCD����ͨ��ʱ��

#define CS_1        GpioDataRegs.GPADAT.bit.GPIO5=1             //GPIO5��ΪCS
#define CS_0        GpioDataRegs.GPADAT.bit.GPIO5=0             //GPIO5��ΪCS

#define SCLK_1      GpioDataRegs.GPADAT.bit.GPIO4=1             //GPIO4��ΪSCLK
#define SCLK_0      GpioDataRegs.GPADAT.bit.GPIO4=0             //GPIO4��ΪSCLK

#define SDATA_OUT   GpioCtrlRegs.GPADIR.bit.GPIO2=1             //GPIO2��ΪSDATA
#define SDATA_IN    GpioCtrlRegs.GPADIR.bit.GPIO2=0             //GPIO2��ΪSDATA
#define SDATA       GpioDataRegs.GPADAT.bit.GPIO2

#define RST_1       GpioDataRegs.GPADAT.bit.GPIO3=1             //GPIO3��RST
#define RST_0       GpioDataRegs.GPADAT.bit.GPIO3=0             //GPIO3��RST




/*
 * LCD12864����ʹ�õ��ĺ���
 *
 *
 * */
extern void InitLcdFun( void );    	 //LCD��ʾ���ĳ�ʼ��
extern void InitLcdGpio(void);		//���ŵĳ�ʼ��
extern void LcdClearTxt( void );		//���ȫ���ַ�
extern void LcdClearBmp(void);		//���ȫ��ͼƬ
extern void ClearBmp_item( Uint16 row );		//���ָ����ͼƬ���д�1��ʼ��
extern void DispENG(uchar row,uchar col,uchar *puts,Uint16 nums);		//��ʾӢ���ַ������ִ� �����д�0��ʼ��
extern void DispFig8(uchar *fig,Uint16 row);		//��ʾһ��ͼƬ��128*64����Uint16 row ָ����ʾ���У���1��ʼ��
extern void DisSingleChar(Uint16 line,Uint16 list,uchar c);		//��ָ���к��г���ʾ�����ַ��������֣����ж���1��ʼ��4�У�8�У�
extern void DisSingleENG(Uint16 line,uchar c);
extern void WriteLcdData(Uint16 data);
extern void WriteLcdCmd(Uint16 data);
extern void delay_loop(Uint32 x);
extern void DelayLCD(Uint16 Second , Uint16 MS100); 		//��ʱ������Uint16 Second ��




#endif /* LCD12864_H_ */
