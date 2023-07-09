#ifndef __timer_H
#define __timer_H

#include "buzzer.h"
#include "humiture_data.h"
#include "sys.h"

#define TIM1_UP_IRQ 0//使能(1)或失能(0)定时器1中断请求函数，发生中断时，先处理中断。
#define TIM2_IRQ    1//使能(1)或失能(0)定时器2中断请求函数
#define TIM3_IRQ    1//使能(1)或失能(0)定时器3中断请求函数
#define TIM4_IRQ    0//使能(1)或失能(0)定时器4中断请求函数

extern u32 TIMx_Count;//TIMx计数值

void TIMx_Init(TIM_TypeDef* TIMx,u32 Period,uint8_t Seize_Priority,uint8_t Son_Priority);//TIMx初始化
void ON_TIMx(TIM_TypeDef* TIMx);                                                         //打开TIMx
void OFF_TIMx(TIM_TypeDef* TIMx);                                                        //关闭TIMx

#endif
