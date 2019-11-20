#ifndef __dac_H
#define __dac_H

#include "stm32f10x.h"
#include <stdio.h>

void DAC_configration(void);
void TIM6_Configuration(void);
void Dac1_Set_Vol(u16 vol);
u16 PIfunction(double kp,double ki);
#endif
