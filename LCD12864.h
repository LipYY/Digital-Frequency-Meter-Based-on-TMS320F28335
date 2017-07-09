/*
 * LCD12864.h
 *
 *  Created on: 2017-7-2
 *      Author: Administrator
 */

#ifndef LCD12864_H_
#define LCD12864_H_



// 宏定义
#define uchar unsigned char

#define  BASIC_SET   	0x30           //基本指令集  00110000
#define  EXTEND_SET  	0x34           //扩展指令集  00110100
#define  DRAW_ON     	0x36           //绘图显示开  00110110
#define  DRAW_OFF    	0x34           //绘图显示关  00110100
#define  DISP_ON    	0x0F           //显示ON,游标ON,游标位反白ON 00001111

#define  execute_72us   delay_loop(400)     //执行时间
#define  SCLK_quarter   delay_loop(40)       //四分之一SCLK时钟 SCLK是LCD串行通信时钟
#define  SCLK_half      delay_loop(80)       //二分之一SCLK时钟 SCLK是LCD串行通信时钟

#define CS_1        GpioDataRegs.GPADAT.bit.GPIO5=1             //GPIO5作为CS
#define CS_0        GpioDataRegs.GPADAT.bit.GPIO5=0             //GPIO5作为CS

#define SCLK_1      GpioDataRegs.GPADAT.bit.GPIO4=1             //GPIO4作为SCLK
#define SCLK_0      GpioDataRegs.GPADAT.bit.GPIO4=0             //GPIO4作为SCLK

#define SDATA_OUT   GpioCtrlRegs.GPADIR.bit.GPIO2=1             //GPIO2作为SDATA
#define SDATA_IN    GpioCtrlRegs.GPADIR.bit.GPIO2=0             //GPIO2作为SDATA
#define SDATA       GpioDataRegs.GPADAT.bit.GPIO2

#define RST_1       GpioDataRegs.GPADAT.bit.GPIO3=1             //GPIO3做RST
#define RST_0       GpioDataRegs.GPADAT.bit.GPIO3=0             //GPIO3做RST




/*
 * LCD12864可能使用到的函数
 *
 *
 * */
extern void InitLcdFun( void );    	 //LCD显示器的初始化
extern void InitLcdGpio(void);		//引脚的初始化
extern void LcdClearTxt( void );		//清除全部字符
extern void LcdClearBmp(void);		//清除全部图片
extern void ClearBmp_item( Uint16 row );		//清除指定行图片（行从1开始）
extern void DispENG(uchar row,uchar col,uchar *puts,Uint16 nums);		//显示英文字符或数字串 （行列从0开始）
extern void DispFig8(uchar *fig,Uint16 row);		//显示一行图片（128*64），Uint16 row 指定显示的行（从1开始）
extern void DisSingleChar(Uint16 line,Uint16 list,uchar c);		//在指定行和列出显示单个字符或者数字（行列都从1开始，4行，8列）
extern void DisSingleENG(Uint16 line,uchar c);
extern void WriteLcdData(Uint16 data);
extern void WriteLcdCmd(Uint16 data);
extern void delay_loop(Uint32 x);
extern void DelayLCD(Uint16 Second , Uint16 MS100); 		//延时函数，Uint16 Second 秒




#endif /* LCD12864_H_ */
