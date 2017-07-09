/*
 * cap_test.c
 *
 *  Created on: 2017-7-3
 *      Author: Administrator
 */

#include "DSP2833x_Device.h"    // DSP2833x ͷ�ļ�
#include "DSP2833x_Examples.h"
#include"LCD12864.h"
#include"Backups_KEY.h"
#include"Dis_char.h"
#include"cap_header.h"

Uint32 cap1_isr_count = 0;
float32 F = 0;
float32 last_F = 0;
Uint32 flag_ISR = 0;

Uint32 bit,ten,hun,thou,million,Ten_bit,Hun_bit,Thou_bit,mill_bit; //��λ��ʮλ����λ��ǧλ,��λ��ʮ��λ���ٷ�λ,ǧ��λ��
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

	//step 1��CPU���жϿ��Ƶĳ�ʼ����CPU�жϵĹرա���־λ�����
	InitSysCtrl();   	//ϵͳ��ʼ��
	DINT;				//�ر�CPU�ж�
	InitPieCtrl();		//��ʼ���ж�PIE����


	InitCap1Gpio_diy();	//��ʼ��CAP1����

	Init_KeyGpio();
	InitLcdGpio();

	IER = 0x000;			//��ֹcpu�ж�
	IFR = 0x000;			//��������жϱ�־λ

	//step2���ж�������ĳ�ʼ�����жϺ�����ַ��ӳ��
	InitPieVectTable();	//��ʼ���ж�������

	EALLOW;				//

	PieVectTable.TINT0 = &cpu_time0_isr;
	//	PieVectTable.ECAP1_INT = &user_cap1_ISR;   //ӳ���жϺ�����ַ���ж�������

	EDIS;

	InitCpuTimers_diy();

	//ʹ�ܸ����жϣ�ʹ��ȫ���ж�

	//	IER |= M_INT4;		//ʹ�ܸ����ж�
	IER |= M_INT1;

	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	//	PieCtrlRegs.PIEIER4.bit.INTx1 = 1;		//ʹ��PIE�����жϣ���4���1����

	EINT;			//ʹ��ȫ���ж�
	ERTM;		//ʹ��ʵʱ�ж�



#if (CPU_FRQ_150MHZ)

	ConfigCpuTimer_diy(&CpuTimer0, 150, 1);				  //Time0�ж�����Ϊ1us

	//ConfigCpuTimer(&CpuTimer1, 150, 2000000);  //Timer1�ж�����Ϊ2��

	//ConfigCpuTimer(&CpuTimer2, 150, 4000000);  //Timer2�ж�����Ϊ4��

#endif

#if (CPU_FRQ_100MHZ)

	ConfigCpuTimer_diy(&CpuTimer0, 100, 1);

	//ConfigCpuTimer(&CpuTimer1, 100, 2000000);  //Timer1�ж�����Ϊ2��

	//ConfigCpuTimer(&CpuTimer2, 100, 4000000);  //Timer2�ж�����Ϊ4��

#endif

	//	CpuTimer0Regs.TCR.bit.TSS = 0; //������ʱ��0��=1��Ϊ�رգ�


	// LCD�ϵ縴λ��Լ80ms��L��ƽ
	RST_0;
	delay_loop(632000); //80ms����ʱ �ϵ���ʱ
	RST_1;
	delay_loop(632000);

	InitLcdFun();
	LcdClearTxt();
	LcdClearBmp();

	Uint16 flag_key ;


	while (1)
	{
		DispFig8(menu2,1);  //�ڵ�һ����ʾ��Ƶ�ʼƿ����桱
		DispFig8(menu1,2); //�ڵڶ�����ʾ������������롱

		flag_key = GetKey() ;

		if(flag_key != 0)
		{
		Handle_key(flag_key);
		break;
		}
	}
	while(1)
	{
		//�ȴ��ж�
	}
}

interrupt void cpu_time0_isr(void)
{
	//��ʱ�жϣ�����
	flag_ISR++;
	PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void user_cap1_ISR(void)
{
	//cap �жϼ���
	cap1_isr_count ++;

	switch(cap1_isr_count)
	{
	case 1 :
		//������ʱ��0
		CpuTimer0Regs.TCR.all =0x4001;
		break;
	case 4:
		//��Ƶ�źŵĲ�����Time0��Ϊ��������cap��Ϊ��ʱ��

		CpuTimer0Regs.TCR.bit.TSS = 1;			//��ֹtime0��ʱ��
		ECap1Regs.ECEINT.bit.CEVT1 = 0;			// ��ֹ�ж�

		last_F = F ;
		F = (float32)(3.0/ (float32)flag_ISR) * 1000000.0;

		Display_F((last_F +F)/2.0);  // ��ʾƵ��

		DelayLCD(15,0);

		cap1_isr_count = 0;
		flag_ISR = 0;
		ECap1Regs.ECEINT.bit.CEVT1 = 1;			// �����ж�
		break;
	default :
		break;

	}

	ECap1Regs.ECCLR.all = 0xFFFF;              // ��� all CAP  �жϱ�־������Ҫ��ȥ���޷����������жϣ�
	ECap1Regs.ECCTL2.bit.REARM = 1;  		//����һ�δ����ؼ��������ж��¼�����cap���ܳ�ʼ�����Ѿ��й�������ȥ������Ӱ���´��жϣ�

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


		if( f < 10)		//С��10Hz
		{

			bit = (Uint32)f;
			num_temp = (Uint32)((f-bit)*100000.0);
			Handle_Decimal(num_temp);  //����С��
			WriteLcdData(num_list[bit]);
			WriteLcdData('.');
			Dis_Decimal(0);		//��ʾС�� 4λ��
			num_temp = 0;
		}else if(f<100)			//С��100Hz
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
		}else if( f<1000)  //С��999Hz
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
		}else if( f<10000)			//С��9KHz
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


//����4λС�� ����С��λ��ֵ������������4λ����,�Ӹߵ��ͷֱ𸳸��� Ten_bit,Hun_bit,Thou_bit,mill_bit
void Handle_Decimal(Uint32 dec)
{

	Ten_bit = dec /1000;
	mill_bit = dec % 10;
	dec = dec/10 - Ten_bit*100;
	Hun_bit = dec/10;
	Thou_bit = dec%10;

}

/*
 * ��ʾС��
 * ����ʾ4λ�� �����ϵ�λ��
 * unit = 0 ---��λΪHz
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



// ������  ���԰�������ѡ�� ����1 ΪƵ�ʲ��� ������2  TODO ����3 TODO
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


			//����cap�ж�

			EALLOW;
			PieVectTable.ECAP1_INT = &user_cap1_ISR;   //ӳ���жϺ�����ַ���ж�������
			EDIS;

			IER |= M_INT4;		//ʹ�ܸ����ж�
			PieCtrlRegs.PIEIER4.bit.INTx1 = 1;		//ʹ��PIE�����жϣ���4���1����
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
