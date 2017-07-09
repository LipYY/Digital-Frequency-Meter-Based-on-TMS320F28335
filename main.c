/*
 * cap_test.c
 *
 *  Created on: 2017-7-3
 *      Author: Administrator
 */

#include "DSP2833x_Device.h"    // DSP2833x 头文件
#include "DSP2833x_Examples.h"
#include"LCD12864.h"
#include"Backups_KEY.h"
#include"Dis_char.h"
#include"cap_header.h"

Uint32 cap1_isr_count = 0;
float32 F = 0;
float32 last_F = 0;
Uint32 flag_ISR = 0;

Uint32 bit,ten,hun,thou,million,Ten_bit,Hun_bit,Thou_bit,mill_bit; //个位，十位，百位，千位,万位，十分位，百分位,千分位；
Uint32 num_temp;
uchar num_list[10] = {'0','1','2','3','4','5','6','7','8','9'};

//void InitCap1Mod1(void);
interrupt void user_cap1_ISR(void);
interrupt void cpu_time0_isr(void);
void InitCap1Gpio_diy(void);
void ConfigCpuTimer_diy(struct CPUTIMER_VARS *Timer, float Freq, float Period);
void InitCpuTimers_diy(void);
void Handle_Decimal(Uint32 dec);
void Dis_Decimal(Uint16 unit);
void Display_F(float32 f);
void Handle_key(Uint16 k);

void main (void)
{

	//step 1：CPU、中断控制的初始化、CPU中断的关闭、标志位的清除
	InitSysCtrl();   	//系统初始化
	DINT;				//关闭CPU中断
	InitPieCtrl();		//初始化中断PIE控制


	InitCap1Gpio_diy();	//初始化CAP1引脚

	Init_KeyGpio();
	InitLcdGpio();

	IER = 0x000;			//禁止cpu中断
	IFR = 0x000;			//清除所有中断标志位

	//step2：中断向量表的初始化；中断函数地址的映射
	InitPieVectTable();	//初始化中断向量表

	EALLOW;				//

	PieVectTable.TINT0 = &cpu_time0_isr;
	//	PieVectTable.ECAP1_INT = &user_cap1_ISR;   //映射中断函数地址至中断向量表

	EDIS;

	InitCpuTimers_diy();

	//使能该组中断，使能全局中断

	//	IER |= M_INT4;		//使能该组中断
	IER |= M_INT1;

	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	//	PieCtrlRegs.PIEIER4.bit.INTx1 = 1;		//使能PIE具体中断（第4组第1个）

	EINT;			//使能全局中断
	ERTM;		//使能实时中断



#if (CPU_FRQ_150MHZ)

	ConfigCpuTimer_diy(&CpuTimer0, 150, 1);				  //Time0中断周期为1us

	//ConfigCpuTimer(&CpuTimer1, 150, 2000000);  //Timer1中断周期为2秒

	//ConfigCpuTimer(&CpuTimer2, 150, 4000000);  //Timer2中断周期为4秒

#endif

#if (CPU_FRQ_100MHZ)

	ConfigCpuTimer_diy(&CpuTimer0, 100, 1);

	//ConfigCpuTimer(&CpuTimer1, 100, 2000000);  //Timer1中断周期为2秒

	//ConfigCpuTimer(&CpuTimer2, 100, 4000000);  //Timer2中断周期为4秒

#endif

	//	CpuTimer0Regs.TCR.bit.TSS = 0; //开启定时器0；=1：为关闭；


	// LCD上电复位，约80ms的L电平
	RST_0;
	delay_loop(632000); //80ms的延时 上电延时
	RST_1;
	delay_loop(632000);

	InitLcdFun();
	LcdClearTxt();
	LcdClearBmp();

	Uint16 flag_key ;


	while (1)
	{
		DispFig8(menu2,1);  //在第一行显示“频率计开发版”
		DispFig8(menu1,2); //在第二行显示“按任意键进入”

		flag_key = GetKey() ;

		if(flag_key != 0)
		{
		Handle_key(flag_key);
		break;
		}
	}
	while(1)
	{
		//等待中断
	}
}

interrupt void cpu_time0_isr(void)
{
	//定时中断，计数
	flag_ISR++;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void user_cap1_ISR(void)
{
	//cap 中断计数
	cap1_isr_count ++;

	switch(cap1_isr_count)
	{
	case 1 :
		//开启定时器0
		CpuTimer0Regs.TCR.all =0x4001;
		break;
	case 4:
		//低频信号的测量。Time0作为计数器；cap作为定时器

		CpuTimer0Regs.TCR.bit.TSS = 1;			//禁止time0定时器
		ECap1Regs.ECEINT.bit.CEVT1 = 0;			// 禁止中断

		last_F = F ;
		F = (float32)(3.0/ (float32)flag_ISR) * 1000000.0;

		Display_F((last_F +F)/2.0);  // 显示频率

		DelayLCD(15,0);

		cap1_isr_count = 0;
		flag_ISR = 0;
		ECap1Regs.ECEINT.bit.CEVT1 = 1;			// 开启中断
		break;
	default :
		break;

	}

	ECap1Regs.ECCLR.all = 0xFFFF;              // 清除 all CAP  中断标志（很重要，去掉无法继续产生中断）
	ECap1Regs.ECCTL2.bit.REARM = 1;  		//在下一次触发沿继续触发中断事件（在cap功能初始化中已经有过，这里去掉不会影响下次中断）

	PieCtrlRegs.PIEACK.all = PIEACK_GROUP4 ;
}



void Display_F(float32 f)
{
	if(f > 0)
	{
		LcdClearTxt();

		DisSingleENG(2,'F');
		WriteLcdData('r');
		WriteLcdData('e');
		WriteLcdData('q');
		WriteLcdData('=');


		if( f < 10)		//小于10Hz
		{

			bit = (Uint32)f;
			num_temp = (Uint32)((f-bit)*100000.0);
			Handle_Decimal(num_temp);  //处理小数
			WriteLcdData(num_list[bit]);
			WriteLcdData('.');
			Dis_Decimal(0);		//显示小数 4位数
			num_temp = 0;
		}else if(f<100)			//小于100Hz
		{

			num_temp = (Uint32)f*10000;
			Handle_Decimal(num_temp%10000);
			num_temp = (Uint32)f;
			bit = num_temp%10;
			ten = num_temp/10;
			WriteLcdData(num_list[ten]);
			WriteLcdData(num_list[bit]);
			WriteLcdData('.');
			Dis_Decimal(0);
			num_temp = 0;
		}else if( f<1000)  //小于999Hz
		{

			num_temp = (Uint32)((f-((Uint32)f)) * 10000.0);
			Handle_Decimal(num_temp);
			num_temp = (Uint32)f;
			bit = num_temp%10;
			hun = num_temp/100;
			ten = (num_temp/10)%10;
			WriteLcdData(num_list[hun]);
			WriteLcdData(num_list[ten]);
			WriteLcdData(num_list[bit]);
			WriteLcdData('.');
			Dis_Decimal(0);
			num_temp = 0;
		}else if( f<10000)			//小于9KHz
		{

			WriteLcdData(num_list[((Uint32)f )/1000]);
			WriteLcdData('.');
			num_temp = (Uint32)((Uint32)f*10.0)%10000;
			Handle_Decimal(num_temp);
			Dis_Decimal(1);
			num_temp = 0;
		}

	}else if(f == 0)
	{
		WriteLcdData('0');
		num_temp = 0;
	}else if(f >100000)
	{
		WriteLcdData('1');
		WriteLcdData('0');
		WriteLcdData('0');
		WriteLcdData('+');
		WriteLcdData('K');
		WriteLcdData('H');
		WriteLcdData('z');
		num_temp = 0;
	}
}


//处理4位小数 ，给小数位赋值，传进来的是4位整数,从高到低分别赋给了 Ten_bit,Hun_bit,Thou_bit,mill_bit
void Handle_Decimal(Uint32 dec)
{

	Ten_bit = dec /1000;
	mill_bit = dec % 10;
	dec = dec/10 - Ten_bit*100;
	Hun_bit = dec/10;
	Thou_bit = dec%10;

}

/*
 * 显示小数
 * （显示4位数 并加上单位）
 * unit = 0 ---单位为Hz
 * unit = 1 ---------KHz
 * unit = 2---------MHz
 * */

void Dis_Decimal(Uint16 unit)
{
	if(unit == 0)
	{
		WriteLcdData(num_list[Ten_bit]);
		WriteLcdData(num_list[Hun_bit]);
		WriteLcdData(num_list[Thou_bit]);
		WriteLcdData(num_list[mill_bit]);
		WriteLcdData('H');
		WriteLcdData('z');
	}
	if(unit == 1)
	{
		WriteLcdData(num_list[Ten_bit]);
		WriteLcdData(num_list[Hun_bit]);
		WriteLcdData(num_list[Thou_bit]);
		WriteLcdData(num_list[mill_bit]);
		WriteLcdData('K');
		WriteLcdData('H');
		WriteLcdData('z');
	}
	if(unit == 2)
	{
		WriteLcdData(num_list[Ten_bit]);
		WriteLcdData(num_list[Hun_bit]);
		WriteLcdData(num_list[Thou_bit]);
		WriteLcdData(num_list[mill_bit]);
		WriteLcdData('M');
		WriteLcdData('H');
		WriteLcdData('z');
	}
}



// 处理按键  将对按键进行选择 按键1 为频率测量 ；按键2  TODO 按键3 TODO
void Handle_key(Uint16 k)
{
	flag_ISR = 0;
	cap1_isr_count = 0;
	F = 0;
	switch (k)
	{
	case 1:
		LcdClearBmp();
		DispFig8(menu3,1);


			//开启cap中断

			EALLOW;
			PieVectTable.ECAP1_INT = &user_cap1_ISR;   //映射中断函数地址至中断向量表
			EDIS;

			IER |= M_INT4;		//使能该组中断
			PieCtrlRegs.PIEIER4.bit.INTx1 = 1;		//使能PIE具体中断（第4组第1个）
			InitCap1Mod1();

			break;
	case 2:
		DisSingleENG(2,'T');
		WriteLcdData('O');
		WriteLcdData('D');
		WriteLcdData('O');
		break;
	case 3:
		DisSingleENG(2,'T');
		WriteLcdData('O');
		WriteLcdData('D');
		WriteLcdData('O');
		break;
	default :
		break;

	}

}
