/*
 * LCD12864.c
 *
 *  Created on: 2017-7-2
 *      Author: Administrator
 */


#include "DSP28x_Project.h"
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include"LCD12864.h"


void WriteLcdCmd(Uint16 data);
void WriteLcdData(Uint16 data);
void WriteByte(Uint16 x);
void DelayLCD(Uint16 Second , Uint16 MS100);

void delay_loop(Uint32 x);

uchar AC_TABLE[]={
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,      //第一行汉字位置
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,      //第二行汉字位置
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,      //第三行汉字位置
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,      //第四行汉字位置
};

uchar AC_TABLE2[4][8]=
{
		{0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,},
		{0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97, },
		{0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,} ,
		{0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,}
};
uchar AC_TABLE3[] ={0X80,0x90,0x88,0x98};
extern Uint16 time=1; //当time = 0 时，死循环一直延时。
/*
 * LCD的初始化
 * */
void InitLcdFun( void )
{
     WriteLcdCmd(0x30);      //8BitMCU,基本指令集合
     WriteLcdCmd(0x03);      //AC归0,不改变DDRAM内容
     WriteLcdCmd(0x0C);      //显示ON,游标OFF,游标位反白OFF
     //WriteLcdCmd(0x0F);      //显示ON,游标ON,游标位反白ON
     WriteLcdCmd(0x01);      //清屏,AC归0
     WriteLcdCmd(0x06);      //写入时,游标右移动
}

/**
 * LCD引脚初始化函数
 *
 * */
void InitLcdGpio(void)
{
    EALLOW;

    //GPIO2输出串行数据，接到LCD的5脚；
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;  	// GPIO2 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;   	// GPIO2 = output
    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;   	// Enable pullup
    GpioDataRegs.GPASET.bit.GPIO2 = 1;   	// Output latch (H)

    //GPIO3输出复位信号，接到LCD的17脚；
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;  	// GPIO3 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;   	// GPIO3 = output
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;   	// Enable pullup
    GpioDataRegs.GPASET.bit.GPIO3 = 1;   	// Output latch (H)

    //GPIO4输出串行时钟，接到LCD的6脚；
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;  	// GPIO4 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;   	// GPIO4 = output
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;   	// Enable pullup
    GpioDataRegs.GPASET.bit.GPIO4 = 1;   	// Output latch (H)

    //GPIO5输出片选信号，接到LCD的4脚，L电平有效；
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;  	// GPIO5 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;   	// GPIO5 = output
    GpioDataRegs.GPASET.bit.GPIO5 = 1;   	// Output latch (H)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;   	// Enable pullup

    EDIS;
}

/******************************************************************************************
* 函数名称    ：LcdClearTxt
* 功能描述    ：清除全部字符，光标回到（0,0）处，（用空白字符填充文本区）
******************************************************************************************/
void LcdClearTxt( void )
{
     uchar i;
     WriteLcdCmd(0x30);      //8BitMCU,基本指令集合
     WriteLcdCmd(0x80);      //AC归起始位

     for(i=0;i<64;i++)
    	 WriteLcdData(0x20);
}
/******************************************************************************************
* 函数名称    ：LcdClearBmp
* 功能描述    ：图形区清RAM函数
******************************************************************************************/
void LcdClearBmp( void )
{
     uchar i,j;

     WriteLcdCmd(0x34);      //8Bit扩充指令集,即使是36H也要写两次
     WriteLcdCmd(0x36);      //绘图ON,基本指令集里面36H不能开绘图

     for(i=0;i<32;i++)            //12864实际为256x32
     {
           WriteLcdCmd(0x80|i);      //行位置
           WriteLcdCmd(0x80);        //列位置

           for(j=0;j<32;j++)            //256/8=32 byte
                WriteLcdData(0);
     }
}

/*函数说明
 *
 * 清除指定行
 * Uint16 row 传入需要清除的行号 ；分为1,2,3,4 行（共4行）
 *
 *
 * */
void ClearBmp_item( Uint16 row )
{

    uchar i,j;
    WriteLcdCmd(0x34);      //8Bit扩充指令集,即使是36H也要写两次
    WriteLcdCmd(0x36);      //绘图ON,基本指令集里面36H不能开绘图

    if(row>4)
    {
   	 return;
    }
    if(row<3)
    {
   	 i=(row-1)*16;  //如果选择第二行，令i=16
   	 //上屏
   	 for(;i<16+(row-1)*16;i++)            //1/4屏幕
   	 {
          WriteLcdCmd(0x80|i);      //行位置i = 0-15 是上屏的上一半屏幕 以此类推
          WriteLcdCmd(0x80);        //列起始位置， 列位置自动增加
          for(j=0;j<16;j++)           //第1列--->第8列
          {
        	  WriteLcdData(0);
          }
   	 }
    } else
    {
   	 i=(row-3)*16;
   	 //下屏
   	 for(;i<16+(row-3)*16;i++)            //第1行--->第32行
   	 {
   		 WriteLcdCmd(0x80|i);      //行位置
   		 WriteLcdCmd(0x88);        //列起始位置， 列位置自动增加
   		 for(j=0;j<16;j++)           //第1列--->第8列
   		 {
   			 WriteLcdData(0);
   		 }
   	 }
    }
}


/******************************************************************************************
* 函数名称    ：WriteLcdCmd：写一个8bit命令到LCD
*
* 时序前缀：11111  RW  RS  0
* 11111 00 0（0xF8）发命令；
* 11111 10 0（0xFC）读忙标志和地址；
* 11111 01 0（0xFA）写RAM数据；
* 11111 11 0（0xFE）读RAM数据；
*
******************************************************************************************/
void WriteLcdCmd(Uint16 data)   //lcd 命令写
{
	Uint16 tmp1,tmp2;
	CS_1;
	delay_loop(40);
	tmp1= (data&0xF0);           //高四位
	tmp2= ((data&0x0F)<<4);      //低四位左移
	// 命令传输时序:1111 1000  D7-D4 0000    D3-D0  0000
	WriteByte(0xF8);      //11111 00 0（0xF8）发命令；
	WriteByte(tmp1);
	WriteByte(tmp2);
	delay_loop(40);
	CS_0;
	execute_72us;
}

/******************************************************************************************
* 函数名称    ：WriteLcdData；写8位数据到LCD，本函数不考虑发送地址
* 输入：Uint16 data：8位数据，
******************************************************************************************/
void WriteLcdData(Uint16 data)
{
	Uint16 tmp1,tmp2;
	CS_1;
	delay_loop(40);
	tmp1= (data&0xF0);           //高四位
	tmp2= ((data&0x0F)<<4);      //低四位左移

	// 数据传输时序:1111 1010  D7-D4 0000    D3-D0  0000
	WriteByte(0xFA);     //11111 01 0（0xFA）写RAM数据；
	WriteByte(tmp1);
	WriteByte(tmp2);
	delay_loop(40);
	CS_0;
	execute_72us;
}

void WriteByte(Uint16 x)
{
	Uint16 i = 0;
	SDATA_OUT;
	SCLK_quarter;
	for(i=0;i<=7;i++)
	{
		SCLK_0;
		SCLK_quarter;
		SDATA=(0x1&((x)>>(7-i)));  //先把高位移出然后发送
		SDATA=(0x1&((x)>>(7-i)));
		SCLK_quarter;
		SCLK_1;                    //把时间改成上升沿锁存的
		SCLK_half;
		SCLK_0;
		SCLK_quarter;
	}
	SDATA=0;
}

/******************************************************************************************
* 函数名称    ：DispENG:显示英语字符或数字串
* 显示起始位置：
* uchar row：行号：0-3
* uchar col：列号：0-7
* 字符指针：uchar *puts
* 字符数：uchar nums，不超过64个，奇数个字符时，自动加一个空格
******************************************************************************************/

void DispENG(uchar row,uchar col,uchar *puts,Uint16 nums)
{
	Uint16 i=0;
	Uint16 odd=0;          // 奇数个字符标志

	if ((nums%2)==1)
	{
		odd=1;
		nums=(nums-1)/2;
	}
	else
	{
		nums=nums/2;
	}

	WriteLcdCmd(0x30);      //8BitMCU,基本指令集合
	WriteLcdCmd(AC_TABLE[8*row+col]);      //起始位置

	for (i=0;i<nums;i++)     //从第一个字符开始
	{
		if(col==8)            //判断换行,若不判断,则自动从第一行到第三行
		{
			 col=0;
			 row++;
		}
		if(row==4) row=0;      //一屏显示完,回到屏左上角

		WriteLcdCmd(AC_TABLE[8*row+col]);
		WriteLcdData(*puts);   //发送8位数据编码
		puts++;
		WriteLcdData(*puts);   //发送8位数据编码
		puts++;
		col++;
     }
	if (odd==1)
	{
		if(col==8)            //判断换行,若不判断,则自动从第一行到第三行
		{
			 col=0;
			 row++;
		}
		if(row==4) row=0;      //一屏显示完,回到屏左上角

		WriteLcdCmd(AC_TABLE[8*row+col]);
		WriteLcdData(*puts);   //发送8位数据编码
		WriteLcdData(0x20);    //0x20空格编码
	}
}

/******************************************************************************************
* 函数名称    ：DispFig8:显示128x64图片，横向取模，字节正序，8位数据格式（数据横排，8个像素对应一个字节，位顺序左高右低）
*         亮暗方式：0x55的图形为：0101  0101，0表示暗，1表示亮。
*         Uint16 row: 选择在那一行显示，总共汉子可以显示4行
*
******************************************************************************************/
void DispFig8(uchar *fig,Uint16 row)
{
     uchar i,j;
     WriteLcdCmd(0x34);      //8Bit扩充指令集,即使是36H也要写两次
     WriteLcdCmd(0x36);      //绘图ON,基本指令集里面36H不能开绘图

     if(row>4)
     {
    	 return;
     }
     if(row<3)
     {
    	 i=(row-1)*16;  //如果选择第二行，令i=16
    	 //上屏
    	 for(;i<16+(row-1)*16;i++)            //1/4屏幕
    	 {
           WriteLcdCmd(0x80|i);      //行位置i = 0-15 是上屏的上一半屏幕 以此类推
           WriteLcdCmd(0x80);        //列起始位置， 列位置自动增加
           for(j=0;j<8;j++)           //第1列--->第8列
           {
              WriteLcdData(*fig);
              fig++;
              WriteLcdData(*fig);
              fig++;
           }
    	 }
     } else
     {
    	 i=(row-3)*16;
    	 //下屏
    	 for(;i<16+(row-3)*16;i++)            //第1行--->第32行
    	 {
    		 WriteLcdCmd(0x80|i);      //行位置
    		 WriteLcdCmd(0x88);        //列起始位置， 列位置自动增加
    		 for(j=0;j<8;j++)           //第1列--->第8列
    		 {
    			 WriteLcdData(*fig);
    			 fig++;
    			 WriteLcdData(*fig);
    			 fig++;
    		 }
    	 }
     }

}

/*
 * DisSingleChar
 *功能：
 * 		在指定行、列显示单个字符或数字(行列都从1开始)
 * 		共4行，每行8个字符，缺陷：这是按中文大小显示的，所以一行只能显示8个字符，理论应该是显示16个非中文字符的。
 * 		注意 ：第一行从0x80开始，0x87结束，第三行从0x88开始，0x8f结束，
 * 												  第二行从0x90开始，0x97结束；第四行从0x98开始，0x9f结束
 * */
void DisSingleChar(Uint16 line,Uint16 list,uchar c)
{
	if(line>4 || list>8)
		{
		return;
		}
	WriteLcdCmd(0x30);
	WriteLcdCmd(AC_TABLE2[line-1][list-1]);
	WriteLcdData(c);
}
/*
 * DisSingleChar
 *功能：
 * 		在指定行、列显示单个字符或数字(行列都从1开始)
 * 		共4行，每行16个字符，
 * 		注意 ：第一行从0x80开始，第三行从0x88开始，第二行从0x90开始；第四行从0x98开始
 * 		这只是显示第一个字符，之后的字符需要使用WriteLcdData(c);来写接下来的
 *
 * */

void DisSingleENG(Uint16 line,uchar c)
{
	if(line>4 )
		{
		return;
		}

	WriteLcdCmd(0x30);
	WriteLcdCmd(AC_TABLE3[line-1]);
	WriteLcdData(c);

}



//延时函数
void delay_loop(Uint32 x)
{
    Uint32      i;
     for (i = 0; i < x; i++) {}
}
/******************************************************************************************
* 函数名称    ：Delay，时间单位：ms
******************************************************************************************/
void Delay(Uint16 MS)
{
     uchar us,usn;
     while(MS!=0)            //for 12M
      {
         usn = 2;
         while(usn!=0)
          {
            us = 0xf5;
            while (us!=0)
             {
               us--;
             };
            usn--;
           }
         MS--;
      }
}
/******************************************************************************************
* 函数名称    ：DelayLCD：LCD延迟函数：时间待定
******************************************************************************************/
void DelayLCD(Uint16 Second , Uint16 MS100)
{                              //输入精确到0.1S,是用","
	Uint16 i;
     for(i=0;i<Second*100+MS100*10;i++)
     {
           if(time==0)
           {
                 Delay(20);
                 while(time==0)
                   {
                   	Delay(20);
                   }
                 break;
           }
           else Delay(10);
     }
}

