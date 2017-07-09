/********************************************************************
* �ļ����� 	����ɨ�����
* ����:  	���г��򣬰��°������۲����Key��ֵ����ֵ�Ӵ�1~9,�޼�����ʱΪ0)��
*       ���߱�ţ�1-3��δ����ʱ���Ϊ0
*       ���߱�ţ�1-3��δ����ʱ���Ϊ0
* ���ú���: ReadKey();

* ʹ����Դ��
* ����KX1~KX3������F28335��GPI053~GPI051
* ����KY1~KY3������F28335��GPI050~GPI048

**********************************************************************/
/********************************************************************
����˵��������ɨ��ԭ��1��3�ж�Ӧ��IO����Ϊ���,3�ж�Ӧ��IO����Ϊ���롣
				2�����ް������£�3������IO����Ϊ�ߵ�ƽ����Ϊ���ⲿ�������裩
				3�����������£���Ӧ��IO�����ͣ�����Ϊ��X�У������Ϊ�͵�ƽ�����������У�X�У��������������
				4����ʱ�����θı�3�е����Ϊ�ߣ�����������������ʱ����X�е�ƽ���±�Ϊ�ߡ����������С������������
********************************************************************/

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

/**************************************�궨��************************************************/
#define SET_KY4			GpioDataRegs.GPASET.bit.GPIO0 = 1								//Y4����
#define SET_KY3			GpioDataRegs.GPBSET.bit.GPIO48 = 1    						//Y3����
#define SET_KY2			GpioDataRegs.GPBSET.bit.GPIO49 = 1							//Y2����
#define SET_KY1			GpioDataRegs.GPBSET.bit.GPIO50 = 1							//Y1����

#define RST_KY4			GpioDataRegs.GPACLEAR.bit.GPIO0 = 1						//Y4����
#define RST_KY3			GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1 						//Y3����
#define RST_KY2			GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1 						//Y2����
#define RST_KY1			GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1						//Y1����

#define KX4_STATUS	 	GpioDataRegs.GPBDAT.bit.GPIO50							//X4״̬
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

/**************************************��������************************************************/
Uint16 Keys[3][3] = {1,2,3,4,5,6,7,8,9};//10,11,12,13,14,15,16};//���ݱ���16��������Ӧ
Uint16 Key ;												//ʵʱ������Ϣ����
Uint16 KX_On = 0;   //���߱�ţ�δ����ʱ���Ϊ0,�а��·ֱ�Ϊ1-3��
Uint16 KY_On = 0;   //�� �߱�ţ�δ����ʱ���Ϊ0,�а��·ֱ�Ϊ1-3��
Uint16 KX_Status[4]={0};     // KX_Status[3]--->KX3״̬�� KX_Status[0]δʹ��

Uint16 num = 0;
Uint16 index=0;

//
///*****************************************************************************************************/
///*********************************************��������************************************************/
//void Init_LedGpio(void);// ��ʼ�������LED�Ƶ�����Ϊ�������
//void Init_KeyGpio(void);
//void delay2(Uint32 t);
//
//
void RstAllKY(void);
//void GetKX(void);
//void GetKX_On(void);
//void SetKY(Uint16 x);
//void RstKY(Uint16 x);
//void GetKey(void);



/*********************************************��������**************************************************/
// ��ʼ�������LED�Ƶ�����Ϊ�������
void Init_LedGpio(void)
{

	EALLOW;
	//LED1  D10<----GPIO0
	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;   	// Enable pullup on GPIO0
    GpioDataRegs.GPASET.bit.GPIO0 = 1;   	// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;   	// GPIO11 = output
	//LED2  D11
	GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;   	// Enable pullup on GPIO1
    GpioDataRegs.GPASET.bit.GPIO1 = 1;   	// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;   	// GPIO11 = output
	//LED3  D12
	GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;   	// Enable pullup on GPIO2
    GpioDataRegs.GPASET.bit.GPIO2 = 1;   	// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;   	// GPIO11 = output
	//LED4  D13
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;   	// Enable pullup on GPIO3
    GpioDataRegs.GPASET.bit.GPIO3 = 1;   	// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;   	// GPIO11 = output
	//LED5  D14
	GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;   	// Enable pullup on GPIO4
    GpioDataRegs.GPASET.bit.GPIO4 = 1;   	// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;   	// GPIO11 = output
	//LED6  D15
	GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;   	// Enable pullup on GPIO5
    GpioDataRegs.GPASET.bit.GPIO5 = 1;   	// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;   	// GPIO11 = output
	//LED7  D16
	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;   	// Enable pullup on GPIO12
    GpioDataRegs.GPASET.bit.GPIO12 = 1;   	// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;   	// GPIO11 = output
	//LED8  D17
	GpioCtrlRegs.GPBPUD.bit.GPIO36 = 0;   	// Enable pullup on GPIO36
    GpioDataRegs.GPBSET.bit.GPIO36 = 1;   	// Load output latch
    GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 0;  	// GPIO11 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO36  = 1;   	// GPIO11 = output

    EDIS;
}

/*******************************����IO��ʼ��***********************************/
void Init_KeyGpio(void)
{

	EALLOW;

	//////////////////////////////�����ĸ�IO������Ϊ�������Ϊ��ɨ��////////////////////////////

	//KY4: ��ӦPCB���� KEY_MATRIX��IO23
//	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;   									// Enable pullup on GPIO11
//    GpioDataRegs.GPASET.bit.GPIO0 = 1;   									// Load output latch
//    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;  									// GPIO11 = GPIO
//    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;   									// GPIO11 = output

	//KY3: ��ӦPCB���� KEY_MATRIX��IO24
	GpioCtrlRegs.GPBPUD.bit.GPIO48 = 0;   									// Enable pullup on GPIO11��ʹ����������
    GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;  									// GPIO11 = GPIO������ΪGPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1;   										// GPIO11 = output������Ϊ�������
    GpioDataRegs.GPBSET.bit.GPIO48 = 1;   									// Load output latch������ߵ�λ

	//KY2: ��ӦPCB���� KEY_MATRIX��IO25
	GpioCtrlRegs.GPBPUD.bit.GPIO49 = 0;   									// Enable pullup on GPIO11
    GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;  									// GPIO11 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO49 = 1;   									// GPIO11 = output
    GpioDataRegs.GPBSET.bit.GPIO49 = 1;   									// Load output latch

	//KY1: ��ӦPCB���� KEY_MATRIX��IO26
    GpioCtrlRegs.GPBPUD.bit.GPIO50 = 0;   									// Enable pullup on GPIO11
    GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;  									// GPIO11 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;   									// GPIO11 = output
    GpioDataRegs.GPBSET.bit.GPIO50 = 1;   									// Load output latch


	//////////////////////////////�����ĸ�IO������Ϊ���룬��Ϊ��ɨ��////////////////////////////

	//KX4: ��ӦPCB���� KEY_MATRIX��IO27
  //  GpioCtrlRegs.GPBPUD.bit.GPIO50 = 0;   									// Enable pullup on GPIO11
  //  GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;  									// ����Ϊһ��IO��
  //  GpioCtrlRegs.GPBDIR.bit.GPIO50  = 0;   									// IO�ڷ���Ϊ����

	//KX3: ��ӦPCB���� KEY_MATRIX��IO28
    GpioCtrlRegs.GPBPUD.bit.GPIO51 = 0;   									// Enable pullup on GPIO51
    GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;  									// ����Ϊһ��IO��
    GpioCtrlRegs.GPBDIR.bit.GPIO51  = 0;   									// IO�ڷ���Ϊ����

 	//KX2: ��ӦPCB���� KEY_MATRIX��IO29
    GpioCtrlRegs.GPBPUD.bit.GPIO52 = 0;   									// Enable pullup on GPIO52
    GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 0;  									// ����Ϊһ��IO��
    GpioCtrlRegs.GPBDIR.bit.GPIO52  = 0;   									// IO�ڷ���Ϊ����

	//KX1: ��ӦPCB���� KEY_MATRIX��IO30
    GpioCtrlRegs.GPBPUD.bit.GPIO53 = 0;   									// Enable pullup on GPIO53
    GpioCtrlRegs.GPBMUX2.bit.GPIO53  = 0;  									// ����Ϊһ��IO��
    GpioCtrlRegs.GPBDIR.bit.GPIO53   = 0;   									// IO�ڷ���Ϊ����

    EDIS;
 	RstAllKY();
}


/*********************************************��ʱ����************************************************/
void delay2(Uint32 t)
{
	Uint32 i = 0;
	for (i = 0; i < t; i++);
}
/*****************************************************************************************************/

/****************************************4��ȫ������͵�ƽ************************************************/
void RstAllKY(void)
{
//	RST_KY4	;
	RST_KY3	;
	RST_KY2	;
	RST_KY1	;
}
/*****************************************************************************************************/

/****************************************��ȡ4��IO��ƽ״̬************************************************/
void GetKX(void)
{
	Uint16 x;           // �����
	Uint16 temp[4];     // temp[0]��ʹ��

	//��ȡ�������ߵ�ƽֵ���޼�����ʱKX_Status[1~3]��Ϊ1
	// Ϊ�˰���������������Ρ�
	KX_Status[1] = KX1_STATUS;
	KX_Status[2] = KX2_STATUS;
	KX_Status[3] = KX3_STATUS;
//	KX_Status[4] = KX4_STATUS;

	for (x=1;x<4;x++)
	{
		temp[x]=KX_Status[x];
	}
	delay2(200);	 // �Ե�

	//Ϊ���������ٴζ�ȡ�������ߵ�ƽֵ
	KX_Status[1] = KX1_STATUS;
	KX_Status[2] = KX2_STATUS;
	KX_Status[3] = KX3_STATUS;
//	KX_Status[4] = KX4_STATUS;

	for (x=1;x<4;x++)
	{
		if (temp[x]!=KX_Status[x]) // ���μ�ⲻͬ����δ���´�����ΪH
			KX_Status[x]=1;
	}
}
/*****************************************************************************************************/

/**************************************��ȡ������λ��**************************************/
// �жϰ��������У���¼�б��1~3�ڱ���KX_On�У����ް������£���KX_On=0��
void GetKX_On(void)
{
	Uint16 x;   // �����


	GetKX();  // �õ�����״̬

	//	�ӵ�1~3�������ж��Ƿ��м�����
	for (x=1;x<4;x++)
	{
		if(KX_Status[x] == 0)   // ��ָ�������м�����
		{
			KX_On = x;          // ��¼�м����µ����߱�ţ������һ��λ��
		}
	}
}
/*****************************************************************************************************/

/*******************************ָ��������ߵ�ƽ******************************************/
void Set_KY(Uint16 y)
{
	if(y==1)
	{
		SET_KY1;
	}
	if(y==2)
	{
		SET_KY2;
	}
	if(y==3)
	{
		SET_KY3;
	}
	//if(y==4){SET_KY4;}
}
/*****************************************************************************************************/

/*******************************ָ��������͵�ƽ**********************************************/
void Rst_KY(Uint16 y)
{
	if(y==1)
	{
		RST_KY1;
	}
	if(y==2)
	{
		RST_KY2;
	}
	if(y==3)
	{
		RST_KY3;
	}
	//if(y==4){RST_KY4;}
}
/*****************************************************************************************************/

/*******************************��ȡ������λ��**********************************************/
// ���а������µ��б��KY_On�Ͱ������Key
void GetKY_On(void)
{
	Uint16 y;       // �����

	if (KX_On==0)  // û�а�������
	{
		KY_On=0;
	}
	else
	{
	    for (y=1;y<4;y++) //���м��
	    {
			Set_KY(y);     // ����xָ������ΪH
			delay2(2000);	   // ��һ�ȣ���ָ��������H������

			GetKX();      // ����������ֵ��˵����ֻ��ָ����KX_On�м��ɣ����ﲻ������д������
			if(KX_Status[KX_On])  // ���ָ��KX_On�е�ƽΪH
			{
				KY_On = y;        // �õ������
			}
			Rst_KY(y);            // �ָ�L��ƽ

			if (KY_On!=0)
				break;

		}
	}
}
/*****************************************************************************************************/


/*****************************************************************************************************/
Uint16 GetKey(void)
{
	Key=0;
	KX_On=0;
	KY_On=0;

	RstAllKY();   // ������������ΪL
	GetKX_On();   // �õ��������߱�ţ�����KX_On��

	if (KX_On!=0)    // �м�����
	{
		GetKY_On();   // �õ��������߱�ţ�����KY_On��
		Key = Keys[KX_On-1][KY_On-1];   // ��ѯ����ֵ
	}
return Key;
}




//===========================================================================
// No more.
//===========================================================================

