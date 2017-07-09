/*
 * cap_fun.c
 *
 *  Created on: 2017-7-5
 *      Author: Administrator
 */

#include "DSP2833x_Device.h"    // DSP2833x 头文件
#include "DSP2833x_Examples.h"

void InitCap1Mod1(void)
{

	//初始化的意思
	ECap1Regs.CAP1 = 0;	// 存储捕捉事件发生时 TSCTR 值
	ECap1Regs.CAP2 = 0;	// 存储捕捉事件发生时 TSCTR 值
	ECap1Regs.CAP3 = 0;	// 存储捕捉事件发生时 TSCTR 值
	ECap1Regs.CAP4 = 0;	// 存储捕捉事件发生时 TSCTR 值
	ECap1Regs.TSCTR = 0;	// eCAP 时间基准计数器
	//	count1Fre =1;
	ECap1Regs.ECEINT.all = 0x0000;             // 禁止所有捕捉中断
	ECap1Regs.ECCLR.all = 0xFFFF;              // 清除所有 CAP 中断标志位
	ECap1Regs.ECCTL1.bit.CAPLDEN = 0;          // 禁止 CAP1-CAP4  寄存器装载
	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;        // 确保计数器停止

	//配置外部寄存器
	ECap1Regs.ECCTL2.bit.CONT_ONESHT = 0;      // 连续
	ECap1Regs.ECCTL2.bit.STOP_WRAP = 0;        // 在事件 1 后环绕,意思是：开启1事件环绕后，//后面遇到事件1的触发沿时，
																										//还是执行事件1。比如，事件1.2都开启了上升沿触发事件，
																										//当第二个上升沿到来时，不回去执行事件2，而是继续执行事件1.
																										//01表示事件2后环绕，依此类推
	ECap1Regs.ECCTL1.bit.CAP1POL = 0;          // 上升沿
	ECap1Regs.ECCTL1.bit.CAP2POL = 1;          // 下降沿
	ECap1Regs.ECCTL1.bit.CAP3POL = 1;          // 下降沿
	ECap1Regs.ECCTL1.bit.CAP4POL = 1;          // 下降沿
	ECap1Regs.ECCTL1.bit.CTRRST1 = 1;          // 差分模式运行
	ECap1Regs.ECCTL1.bit.CTRRST2 = 1;          // 差分模式运行
	ECap1Regs.ECCTL1.bit.CTRRST3 = 1;          // 差分模式运行
	ECap1Regs.ECCTL1.bit.CTRRST4 = 1;          // 差分模式运行
	ECap1Regs.ECCTL2.bit.SYNCI_EN = 1;         // 使能 sync 输入
	ECap1Regs.ECCTL2.bit.SYNCO_SEL = 0;        // 直通
	ECap1Regs.ECCTL1.bit.CAPLDEN = 1;          // 使能捕捉模块

	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;        // 启动计数器
	ECap1Regs.ECCTL2.bit.REARM = 1;            // 单次强制
	//	ECap1Regs.ECEINT.bit.CEVT4 = 1;            // 使能 4  事件 = 中断
	//	ECap1Regs.ECEINT.bit.CEVT3 = 1;            // 使能 3  事件 = 中断
	//	ECap1Regs.ECEINT.bit.CEVT2 = 1;            // 使能 2  事件 = 中断
	ECap1Regs.ECEINT.bit.CEVT1 = 1;            // 使能 1  事件 = 中断
	ECap1Regs.ECCTL2.bit.SWSYNC = 1;			// 同步脉冲
}

void InitCap1Gpio_diy(void)
{
	EALLOW;
	/* Enable internal pull-up for the selected pins */
	// Pull-ups can be enabled or disabled by the user.
	// This will enable the pullups for the specified pins.
	// Comment out other unwanted lines.

	// GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;      // Enable pull-up on GPIO5 (CAP1)
	GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;     // Enable pull-up on GPIO24 (CAP1)
	// GpioCtrlRegs.GPBPUD.bit.GPIO34 = 0;     // Enable pull-up on GPIO34 (CAP1)


	// Inputs are synchronized to SYSCLKOUT by default.
	// Comment out other unwanted lines.

	// GpioCtrlRegs.GPAQSEL1.bit.GPIO5 = 0;    // Synch to SYSCLKOUT GPIO5 (CAP1)
	GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 0;   // Synch to SYSCLKOUT GPIO24 (CAP1)
	// GpioCtrlRegs.GPBQSEL1.bit.GPIO34 = 0;   // Synch to SYSCLKOUT GPIO34 (CAP1)

	/* Configure eCAP-1 pins using GPIO regs*/
	// This specifies which of the possible GPIO pins will be eCAP1 functional pins.
	// Comment out other unwanted lines.

	// GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 3;     // Configure GPIO5 as CAP1
	GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 1;    // Configure GPIO24 as CAP1
	// GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 1;    // Configure GPIO24 as CAP1

	EDIS;
}

void InitCpuTimers_diy(void)
{
	// CPU Timer 0
	// Initialize address pointers to respective timer registers:
	CpuTimer0.RegsAddr = &CpuTimer0Regs;
	// Initialize timer period to maximum:
	CpuTimer0Regs.PRD.all  = 0xFFFFFFFF;
	// Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
	CpuTimer0Regs.TPR.all  = 0;
	CpuTimer0Regs.TPRH.all = 0;
	// Make sure timer is stopped:
	CpuTimer0Regs.TCR.bit.TSS = 1;
	// Reload all counter register with period value:
	CpuTimer0Regs.TCR.bit.TRB = 1;
	// Reset interrupt counters:
	CpuTimer0.InterruptCount = 0;


	// CpuTimer 1 and CpuTimer2 are reserved for DSP BIOS & other RTOS
	// Do not use these two timers if you ever plan on integrating
	// DSP-BIOS or another realtime OS.
	//
	// Initialize address pointers to respective timer registers:
	CpuTimer1.RegsAddr = &CpuTimer1Regs;
	CpuTimer2.RegsAddr = &CpuTimer2Regs;
	// Initialize timer period to maximum:
	CpuTimer1Regs.PRD.all  = 0xFFFFFFFF;
	CpuTimer2Regs.PRD.all  = 0xFFFFFFFF;
	// Initialize pre-scale counter to divide by 1 (SYSCLKOUT):
	CpuTimer1Regs.TPR.all  = 0;
	CpuTimer1Regs.TPRH.all = 0;
	CpuTimer2Regs.TPR.all  = 0;
	CpuTimer2Regs.TPRH.all = 0;
	// Make sure timers are stopped:
	CpuTimer1Regs.TCR.bit.TSS = 1;
	CpuTimer2Regs.TCR.bit.TSS = 1;
	// Reload all counter register with period value:
	CpuTimer1Regs.TCR.bit.TRB = 1;
	CpuTimer2Regs.TCR.bit.TRB = 1;
	// Reset interrupt counters:
	CpuTimer1.InterruptCount = 0;
	CpuTimer2.InterruptCount = 0;
}
void ConfigCpuTimer_diy(struct CPUTIMER_VARS *Timer, float Freq, float Period)
{
	Uint32 	temp;

	// Initialize timer period:
	Timer->CPUFreqInMHz = Freq;
	Timer->PeriodInUSec = Period;
	temp = (long) (Freq * Period);
	Timer->RegsAddr->PRD.all = temp;

	// Set pre-scale counter to divide by 1 (SYSCLKOUT):
	Timer->RegsAddr->TPR.all  = 0;
	Timer->RegsAddr->TPRH.all  = 0;

	// Initialize timer control register:
	Timer->RegsAddr->TCR.bit.TSS = 1;      // 1 = Stop timer, 0 = Start/Restart Timer
	Timer->RegsAddr->TCR.bit.TRB = 1;      // 1 = reload timer
	Timer->RegsAddr->TCR.bit.SOFT = 0;
	Timer->RegsAddr->TCR.bit.FREE = 0;     // Timer Free Run Disabled
	Timer->RegsAddr->TCR.bit.TIE = 1;      // 0 = Disable/ 1 = Enable Timer Interrupt

	// Reset interrupt counter:
	Timer->InterruptCount = 0;
}
