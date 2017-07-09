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
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,      //��һ�к���λ��
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,      //�ڶ��к���λ��
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,      //�����к���λ��
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,      //�����к���λ��
};

uchar AC_TABLE2[4][8]=
{
		{0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,},
		{0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97, },
		{0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,} ,
		{0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,}
};
uchar AC_TABLE3[] ={0X80,0x90,0x88,0x98};
extern Uint16 time=1; //��time = 0 ʱ����ѭ��һֱ��ʱ��
/*
 * LCD�ĳ�ʼ��
 * */
void InitLcdFun( void )
{
     WriteLcdCmd(0x30);      //8BitMCU,����ָ���
     WriteLcdCmd(0x03);      //AC��0,���ı�DDRAM����
     WriteLcdCmd(0x0C);      //��ʾON,�α�OFF,�α�λ����OFF
     //WriteLcdCmd(0x0F);      //��ʾON,�α�ON,�α�λ����ON
     WriteLcdCmd(0x01);      //����,AC��0
     WriteLcdCmd(0x06);      //д��ʱ,�α����ƶ�
}

/**
 * LCD���ų�ʼ������
 *
 * */
void InitLcdGpio(void)
{
    EALLOW;

    //GPIO2����������ݣ��ӵ�LCD��5�ţ�
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;  	// GPIO2 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;   	// GPIO2 = output
    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;   	// Enable pullup
    GpioDataRegs.GPASET.bit.GPIO2 = 1;   	// Output latch (H)

    //GPIO3�����λ�źţ��ӵ�LCD��17�ţ�
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;  	// GPIO3 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;   	// GPIO3 = output
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;   	// Enable pullup
    GpioDataRegs.GPASET.bit.GPIO3 = 1;   	// Output latch (H)

    //GPIO4�������ʱ�ӣ��ӵ�LCD��6�ţ�
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;  	// GPIO4 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;   	// GPIO4 = output
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;   	// Enable pullup
    GpioDataRegs.GPASET.bit.GPIO4 = 1;   	// Output latch (H)

    //GPIO5���Ƭѡ�źţ��ӵ�LCD��4�ţ�L��ƽ��Ч��
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;  	// GPIO5 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;   	// GPIO5 = output
    GpioDataRegs.GPASET.bit.GPIO5 = 1;   	// Output latch (H)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;   	// Enable pullup

    EDIS;
}

/******************************************************************************************
* ��������    ��LcdClearTxt
* ��������    �����ȫ���ַ������ص���0,0���������ÿհ��ַ�����ı�����
******************************************************************************************/
void LcdClearTxt( void )
{
     uchar i;
     WriteLcdCmd(0x30);      //8BitMCU,����ָ���
     WriteLcdCmd(0x80);      //AC����ʼλ

     for(i=0;i<64;i++)
    	 WriteLcdData(0x20);
}
/******************************************************************************************
* ��������    ��LcdClearBmp
* ��������    ��ͼ������RAM����
******************************************************************************************/
void LcdClearBmp( void )
{
     uchar i,j;

     WriteLcdCmd(0x34);      //8Bit����ָ�,��ʹ��36HҲҪд����
     WriteLcdCmd(0x36);      //��ͼON,����ָ�����36H���ܿ���ͼ

     for(i=0;i<32;i++)            //12864ʵ��Ϊ256x32
     {
           WriteLcdCmd(0x80|i);      //��λ��
           WriteLcdCmd(0x80);        //��λ��

           for(j=0;j<32;j++)            //256/8=32 byte
                WriteLcdData(0);
     }
}

/*����˵��
 *
 * ���ָ����
 * Uint16 row ������Ҫ������к� ����Ϊ1,2,3,4 �У���4�У�
 *
 *
 * */
void ClearBmp_item( Uint16 row )
{

    uchar i,j;
    WriteLcdCmd(0x34);      //8Bit����ָ�,��ʹ��36HҲҪд����
    WriteLcdCmd(0x36);      //��ͼON,����ָ�����36H���ܿ���ͼ

    if(row>4)
    {
   	 return;
    }
    if(row<3)
    {
   	 i=(row-1)*16;  //���ѡ��ڶ��У���i=16
   	 //����
   	 for(;i<16+(row-1)*16;i++)            //1/4��Ļ
   	 {
          WriteLcdCmd(0x80|i);      //��λ��i = 0-15 ����������һ����Ļ �Դ�����
          WriteLcdCmd(0x80);        //����ʼλ�ã� ��λ���Զ�����
          for(j=0;j<16;j++)           //��1��--->��8��
          {
        	  WriteLcdData(0);
          }
   	 }
    } else
    {
   	 i=(row-3)*16;
   	 //����
   	 for(;i<16+(row-3)*16;i++)            //��1��--->��32��
   	 {
   		 WriteLcdCmd(0x80|i);      //��λ��
   		 WriteLcdCmd(0x88);        //����ʼλ�ã� ��λ���Զ�����
   		 for(j=0;j<16;j++)           //��1��--->��8��
   		 {
   			 WriteLcdData(0);
   		 }
   	 }
    }
}


/******************************************************************************************
* ��������    ��WriteLcdCmd��дһ��8bit���LCD
*
* ʱ��ǰ׺��11111  RW  RS  0
* 11111 00 0��0xF8�������
* 11111 10 0��0xFC����æ��־�͵�ַ��
* 11111 01 0��0xFA��дRAM���ݣ�
* 11111 11 0��0xFE����RAM���ݣ�
*
******************************************************************************************/
void WriteLcdCmd(Uint16 data)   //lcd ����д
{
	Uint16 tmp1,tmp2;
	CS_1;
	delay_loop(40);
	tmp1= (data&0xF0);           //����λ
	tmp2= ((data&0x0F)<<4);      //����λ����
	// �����ʱ��:1111 1000  D7-D4 0000    D3-D0  0000
	WriteByte(0xF8);      //11111 00 0��0xF8�������
	WriteByte(tmp1);
	WriteByte(tmp2);
	delay_loop(40);
	CS_0;
	execute_72us;
}

/******************************************************************************************
* ��������    ��WriteLcdData��д8λ���ݵ�LCD�������������Ƿ��͵�ַ
* ���룺Uint16 data��8λ���ݣ�
******************************************************************************************/
void WriteLcdData(Uint16 data)
{
	Uint16 tmp1,tmp2;
	CS_1;
	delay_loop(40);
	tmp1= (data&0xF0);           //����λ
	tmp2= ((data&0x0F)<<4);      //����λ����

	// ���ݴ���ʱ��:1111 1010  D7-D4 0000    D3-D0  0000
	WriteByte(0xFA);     //11111 01 0��0xFA��дRAM���ݣ�
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
		SDATA=(0x1&((x)>>(7-i)));  //�ȰѸ�λ�Ƴ�Ȼ����
		SDATA=(0x1&((x)>>(7-i)));
		SCLK_quarter;
		SCLK_1;                    //��ʱ��ĳ������������
		SCLK_half;
		SCLK_0;
		SCLK_quarter;
	}
	SDATA=0;
}

/******************************************************************************************
* ��������    ��DispENG:��ʾӢ���ַ������ִ�
* ��ʾ��ʼλ�ã�
* uchar row���кţ�0-3
* uchar col���кţ�0-7
* �ַ�ָ�룺uchar *puts
* �ַ�����uchar nums��������64�����������ַ�ʱ���Զ���һ���ո�
******************************************************************************************/

void DispENG(uchar row,uchar col,uchar *puts,Uint16 nums)
{
	Uint16 i=0;
	Uint16 odd=0;          // �������ַ���־

	if ((nums%2)==1)
	{
		odd=1;
		nums=(nums-1)/2;
	}
	else
	{
		nums=nums/2;
	}

	WriteLcdCmd(0x30);      //8BitMCU,����ָ���
	WriteLcdCmd(AC_TABLE[8*row+col]);      //��ʼλ��

	for (i=0;i<nums;i++)     //�ӵ�һ���ַ���ʼ
	{
		if(col==8)            //�жϻ���,�����ж�,���Զ��ӵ�һ�е�������
		{
			 col=0;
			 row++;
		}
		if(row==4) row=0;      //һ����ʾ��,�ص������Ͻ�

		WriteLcdCmd(AC_TABLE[8*row+col]);
		WriteLcdData(*puts);   //����8λ���ݱ���
		puts++;
		WriteLcdData(*puts);   //����8λ���ݱ���
		puts++;
		col++;
     }
	if (odd==1)
	{
		if(col==8)            //�жϻ���,�����ж�,���Զ��ӵ�һ�е�������
		{
			 col=0;
			 row++;
		}
		if(row==4) row=0;      //һ����ʾ��,�ص������Ͻ�

		WriteLcdCmd(AC_TABLE[8*row+col]);
		WriteLcdData(*puts);   //����8λ���ݱ���
		WriteLcdData(0x20);    //0x20�ո����
	}
}

/******************************************************************************************
* ��������    ��DispFig8:��ʾ128x64ͼƬ������ȡģ���ֽ�����8λ���ݸ�ʽ�����ݺ��ţ�8�����ض�Ӧһ���ֽڣ�λ˳������ҵͣ�
*         ������ʽ��0x55��ͼ��Ϊ��0101  0101��0��ʾ����1��ʾ����
*         Uint16 row: ѡ������һ����ʾ���ܹ����ӿ�����ʾ4��
*
******************************************************************************************/
void DispFig8(uchar *fig,Uint16 row)
{
     uchar i,j;
     WriteLcdCmd(0x34);      //8Bit����ָ�,��ʹ��36HҲҪд����
     WriteLcdCmd(0x36);      //��ͼON,����ָ�����36H���ܿ���ͼ

     if(row>4)
     {
    	 return;
     }
     if(row<3)
     {
    	 i=(row-1)*16;  //���ѡ��ڶ��У���i=16
    	 //����
    	 for(;i<16+(row-1)*16;i++)            //1/4��Ļ
    	 {
           WriteLcdCmd(0x80|i);      //��λ��i = 0-15 ����������һ����Ļ �Դ�����
           WriteLcdCmd(0x80);        //����ʼλ�ã� ��λ���Զ�����
           for(j=0;j<8;j++)           //��1��--->��8��
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
    	 //����
    	 for(;i<16+(row-3)*16;i++)            //��1��--->��32��
    	 {
    		 WriteLcdCmd(0x80|i);      //��λ��
    		 WriteLcdCmd(0x88);        //����ʼλ�ã� ��λ���Զ�����
    		 for(j=0;j<8;j++)           //��1��--->��8��
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
 *���ܣ�
 * 		��ָ���С�����ʾ�����ַ�������(���ж���1��ʼ)
 * 		��4�У�ÿ��8���ַ���ȱ�ݣ����ǰ����Ĵ�С��ʾ�ģ�����һ��ֻ����ʾ8���ַ�������Ӧ������ʾ16���������ַ��ġ�
 * 		ע�� ����һ�д�0x80��ʼ��0x87�����������д�0x88��ʼ��0x8f������
 * 												  �ڶ��д�0x90��ʼ��0x97�����������д�0x98��ʼ��0x9f����
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
 *���ܣ�
 * 		��ָ���С�����ʾ�����ַ�������(���ж���1��ʼ)
 * 		��4�У�ÿ��16���ַ���
 * 		ע�� ����һ�д�0x80��ʼ�������д�0x88��ʼ���ڶ��д�0x90��ʼ�������д�0x98��ʼ
 * 		��ֻ����ʾ��һ���ַ���֮����ַ���Ҫʹ��WriteLcdData(c);��д��������
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



//��ʱ����
void delay_loop(Uint32 x)
{
    Uint32      i;
     for (i = 0; i < x; i++) {}
}
/******************************************************************************************
* ��������    ��Delay��ʱ�䵥λ��ms
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
* ��������    ��DelayLCD��LCD�ӳٺ�����ʱ�����
******************************************************************************************/
void DelayLCD(Uint16 Second , Uint16 MS100)
{                              //���뾫ȷ��0.1S,����","
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

