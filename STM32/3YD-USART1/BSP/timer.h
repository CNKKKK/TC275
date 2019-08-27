#ifndef __Timer_H
#define	__Timer_H

#include "stm32f10x.h"
void TIM2_Config(void);
void TIM2_delay(u16 time);
void TIM3_Init(u16 arr,u16 psc);

#endif

