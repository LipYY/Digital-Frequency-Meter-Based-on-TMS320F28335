/*
 * cap_header.h
 *
 *  Created on: 2017-7-5
 *      Author: Administrator
 */

#ifndef CAP_HEADER_H_
#define CAP_HEADER_H_

extern void InitCap1Mod1(void);
extern void InitCap1Gpio_diy(void);
extern void ConfigCpuTimer_diy(struct CPUTIMER_VARS *Timer, float Freq, float Period);
extern void InitCpuTimers_diy(void);
#endif /* CAP_HEADER_H_ */
