/********************************************************************
* 文件名： 	按键扫描程序
* 描述:  	运行程序，按下按键，观测变量Key的值（该值从从1~9,无键按下时为0)。
*       行线编号：1-3，未按下时编号为0
*       列线编号：1-3，未按下时编号为0
* 调用函数: ReadKey();

* 使用资源：
* 行线KX1~KX3，接入F28335的GPI053~GPI051
* 行线KY1~KY3，接入F28335的GPI050~GPI048

**********************************************************************/
/********************************************************************
程序说明：按键扫描原理：1、3列对应的IO设置为输出,3行对应的IO设置为输入。
				2、若无按键按下，3行输入IO，均为高电平（因为有外部上拉电阻）
				3、若按键按下，对应行IO被拉低（假设为第X行），检测为低电平。按键所在行（X行），被鉴别出来。
				4、此时，依次改变3列的输出为高，当遇到按键所在列时，第X行电平重新变为高。按键所在列。被鉴别出来。
********************************************************************/

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

/**************************************宏定义************************************************/
#define SET_KY4			GpioDataRegs.GPASET.bit.GPIO0 = 1								//Y4拉高
#define SET_KY3			GpioDataRegs.GPBSET.bit.GPIO48 = 1    						//Y3拉高
#define SET_KY2			GpioDataRegs.GPBSET.bit.GPIO49 = 1							//Y2拉高
#define SET_KY1			GpioDataRegs.GPBSET.bit.GPIO50 = 1							//Y1拉高

#define RST_KY4			GpioDataRegs.GPACLEAR.bit.GPIO0 = 1						//Y4拉低
#define RST_KY3			GpioDataRegs.GPBCLEAR.bit.GPIO48 = 1 						//Y3拉低
#define RST_KY2			GpioDataRegs.GPBCLEAR.bit.GPIO49 = 1 						//Y2拉低
#define RST_KY1			GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1						//Y1拉低

#define KX4_STATUS	 	GpioDataRegs.GPBDAT.bit.GPIO50							//X4状态
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

/**************************************变量定义************************************************/
Uint16 Keys[3][3] = {1,2,3,4,5,6,7,8,9};//10,11,12,13,14,15,16};//数据表，与16个按键对应
Uint16 Key ;												//实时按键信息变量
Uint16 KX_On = 0;   //行线编号：未按下时编号为0,有按下分别为1-3，
Uint16 KY_On = 0;   //列 线编号：未按下时编号为0,有按下分别为1-3，
Uint16 KX_Status[4]={0};     // KX_Status[3]--->KX3状态， KX_Status[0]未使用

Uint16 num = 0;
Uint16 index=0;

//
///*****************************************************************************************************/
///*********************************************函数声明************************************************/
//void Init_LedGpio(void);// 初始化输出到LED灯的引脚为输出引脚
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



/*********************************************函数定义**************************************************/
// 初始化输出到LED灯的引脚为输出引脚
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

/*******************************按键IO初始化***********************************/
void Init_KeyGpio(void)
{

	EALLOW;

	//////////////////////////////以下四个IO口设置为输出，作为列扫描////////////////////////////

	//KY4: 对应PCB端子 KEY_MATRIX：IO23
//	GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;   									// Enable pullup on GPIO11
//    GpioDataRegs.GPASET.bit.GPIO0 = 1;   									// Load output latch
//    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;  									// GPIO11 = GPIO
//    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;   									// GPIO11 = output

	//KY3: 对应PCB端子 KEY_MATRIX：IO24
	GpioCtrlRegs.GPBPUD.bit.GPIO48 = 0;   									// Enable pullup on GPIO11。使能上拉电阻
    GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;  									// GPIO11 = GPIO。设置为GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO48 = 1;   										// GPIO11 = output。设置为输出方向
    GpioDataRegs.GPBSET.bit.GPIO48 = 1;   									// Load output latch。输出高电位

	//KY2: 对应PCB端子 KEY_MATRIX：IO25
	GpioCtrlRegs.GPBPUD.bit.GPIO49 = 0;   									// Enable pullup on GPIO11
    GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;  									// GPIO11 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO49 = 1;   									// GPIO11 = output
    GpioDataRegs.GPBSET.bit.GPIO49 = 1;   									// Load output latch

	//KY1: 对应PCB端子 KEY_MATRIX：IO26
    GpioCtrlRegs.GPBPUD.bit.GPIO50 = 0;   									// Enable pullup on GPIO11
    GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;  									// GPIO11 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;   									// GPIO11 = output
    GpioDataRegs.GPBSET.bit.GPIO50 = 1;   									// Load output latch


	//////////////////////////////以下四个IO口设置为输入，作为行扫描////////////////////////////

	//KX4: 对应PCB端子 KEY_MATRIX：IO27
  //  GpioCtrlRegs.GPBPUD.bit.GPIO50 = 0;   									// Enable pullup on GPIO11
  //  GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;  									// 设置为一般IO口
  //  GpioCtrlRegs.GPBDIR.bit.GPIO50  = 0;   									// IO口方向为输入

	//KX3: 对应PCB端子 KEY_MATRIX：IO28
    GpioCtrlRegs.GPBPUD.bit.GPIO51 = 0;   									// Enable pullup on GPIO51
    GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;  									// 设置为一般IO口
    GpioCtrlRegs.GPBDIR.bit.GPIO51  = 0;   									// IO口方向为输入

 	//KX2: 对应PCB端子 KEY_MATRIX：IO29
    GpioCtrlRegs.GPBPUD.bit.GPIO52 = 0;   									// Enable pullup on GPIO52
    GpioCtrlRegs.GPBMUX2.bit.GPIO52 = 0;  									// 设置为一般IO口
    GpioCtrlRegs.GPBDIR.bit.GPIO52  = 0;   									// IO口方向为输入

	//KX1: 对应PCB端子 KEY_MATRIX：IO30
    GpioCtrlRegs.GPBPUD.bit.GPIO53 = 0;   									// Enable pullup on GPIO53
    GpioCtrlRegs.GPBMUX2.bit.GPIO53  = 0;  									// 设置为一般IO口
    GpioCtrlRegs.GPBDIR.bit.GPIO53   = 0;   									// IO口方向为输入

    EDIS;
 	RstAllKY();
}


/*********************************************延时函数************************************************/
void delay2(Uint32 t)
{
	Uint32 i = 0;
	for (i = 0; i < t; i++);
}
/*****************************************************************************************************/

/****************************************4列全部输出低电平************************************************/
void RstAllKY(void)
{
//	RST_KY4	;
	RST_KY3	;
	RST_KY2	;
	RST_KY1	;
}
/*****************************************************************************************************/

/****************************************读取4行IO电平状态************************************************/
void GetKX(void)
{
	Uint16 x;           // 行序号
	Uint16 temp[4];     // temp[0]不使用

	//读取所有行线电平值，无键按下时KX_Status[1~3]均为1
	// 为了按键消抖，检测两次。
	KX_Status[1] = KX1_STATUS;
	KX_Status[2] = KX2_STATUS;
	KX_Status[3] = KX3_STATUS;
//	KX_Status[4] = KX4_STATUS;

	for (x=1;x<4;x++)
	{
		temp[x]=KX_Status[x];
	}
	delay2(200);	 // 稍等

	//为了消抖，再次读取所有行线电平值
	KX_Status[1] = KX1_STATUS;
	KX_Status[2] = KX2_STATUS;
	KX_Status[3] = KX3_STATUS;
//	KX_Status[4] = KX4_STATUS;

	for (x=1;x<4;x++)
	{
		if (temp[x]!=KX_Status[x]) // 两次检测不同，按未按下处理，即为H
			KX_Status[x]=1;
	}
}
/*****************************************************************************************************/

/**************************************读取按键行位置**************************************/
// 判断按键所在行，记录行编号1~3在变量KX_On中，若无按键按下，则KX_On=0；
void GetKX_On(void)
{
	Uint16 x;   // 行序号


	GetKX();  // 得到行线状态

	//	从第1~3行依次判断是否有键按下
	for (x=1;x<4;x++)
	{
		if(KX_Status[x] == 0)   // 若指定行线有键按下
		{
			KX_On = x;          // 记录有键按下的行线编号，以最后一个位置
		}
	}
}
/*****************************************************************************************************/

/*******************************指定列输出高电平******************************************/
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

/*******************************指定列输出低电平**********************************************/
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

/*******************************读取按键列位置**********************************************/
// 读有按键按下的列编号KY_On和按键编号Key
void GetKY_On(void)
{
	Uint16 y;       // 列序号

	if (KX_On==0)  // 没有按键按下
	{
		KY_On=0;
	}
	else
	{
	    for (y=1;y<4;y++) //逐列检测
	    {
			Set_KY(y);     // 设置x指定的列为H
			delay2(2000);	   // 等一等，让指定列拉至H，消抖

			GetKX();      // 读所有行线值（说明：只需指定的KX_On行即可，这里不单独编写函数）
			if(KX_Status[KX_On])  // 如果指定KX_On行电平为H
			{
				KY_On = y;        // 得到列序号
			}
			Rst_KY(y);            // 恢复L电平

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

	RstAllKY();   // 设置所有列线为L
	GetKX_On();   // 得到按键行线编号，放在KX_On中

	if (KX_On!=0)    // 有键按下
	{
		GetKY_On();   // 得到按键列线编号，放在KY_On中
		Key = Keys[KX_On-1][KY_On-1];   // 查询按键值
	}
return Key;
}




//===========================================================================
// No more.
//===========================================================================

