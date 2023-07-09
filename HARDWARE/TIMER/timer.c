#include "timer.h"

u32 TIMx_Count=0;//TIMx计数值

//TIMx初始化
//TIMx:TIM1~TIM4
//Period(周期):1us~6553000us,Tout(us)=((Arr+1)*(Psc+1))/Tclk(MHz)
//Seize_Priority:抢占优先级
//Son_Priority:子优先级
void TIMx_Init(TIM_TypeDef* TIMx,u32 Period,uint8_t Seize_Priority,uint8_t Son_Priority)
{
	uint16_t Arr,Psc;
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	if(Period==1)                                                         //1us
	{
		Psc=35;                                                           //TIMx时钟频率除数预分频值
		Arr=36;                                                           //自动重装载寄存器周期值
	}
	else if(Period>=2 && Period<=65535)                                   //2us~65535us
	{
		Psc=71;                                                           //TIMx时钟频率除数预分频值
		Arr=(72/(Psc+1))*Period;                                          //自动重装载寄存器周期值
	}
	else if(Period>65535 && Period<=6553000)                              //65535us~6553000us
	{
		Psc=7199;                                                         //TIMx时钟频率除数预分频值
		Period/=1000;                                                     //转换为ms
		Arr=(72000/(Psc+1))*Period;                                       //自动重装载寄存器周期值
	}
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);                   //使能AFIO时钟(使能功能复用IO)
	
	if(TIMx==TIM1)                                                        //通用定时器1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);               //使能TIM1时钟
	else if(TIMx==TIM2)                                                   //通用定时器2
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);               //使能TIM2时钟
	else if(TIMx==TIM3)                                                   //通用定时器3
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);               //使能TIM3时钟
	else if(TIMx==TIM4)                                                   //通用定时器4
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);               //使能TIM4时钟
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);               //JTAG-DP失能+SW-DP使能
	
	TIM_TimeBaseStructure.TIM_Period = (uint16_t)Arr-1;	                  //自动重装载寄存器周期值
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)Psc;                  //TIMx时钟频率除数预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;               //设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;           //TIM向上计数模式
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);
	
	if(TIMx==TIM1)
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;                //TIM1中断
	else if(TIMx==TIM2)
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;                   //TIM2中断
	else if(TIMx==TIM3)
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;                   //TIM3中断
	else if(TIMx==TIM4)
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;                   //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = Seize_Priority;//抢占优先级Seize_Priority
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = Son_Priority;         //子优先级Son_Priority
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                       //NVIC使能
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_ClearFlag(TIMx,TIM_FLAG_Update);                                  //清除更新中断
	TIM_ITConfig(TIMx,TIM_IT_Update,DISABLE);                             //失能TIMx更新中断
	TIM_Cmd(TIMx,DISABLE);                                                //失能TIMx外设
}
//打开TIMx
//TIMx:TIM1~TIM8
void ON_TIMx(TIM_TypeDef* TIMx)
{
	TIM_ITConfig(TIMx,TIM_IT_Update,ENABLE);//使能TIMx更新中断
	TIM_Cmd(TIMx,ENABLE);                   //使能TIMx外设
}
//关闭TIMx
//TIMx:TIM1~TIM8
void OFF_TIMx(TIM_TypeDef* TIMx)
{
	TIM_ITConfig(TIMx,TIM_IT_Update,DISABLE);//失能TIMx更新中断
	TIM_Cmd(TIMx,DISABLE);                   //失能TIMx外设
}
#if TIM1_UP_IRQ==1
//TIM1中断请求函数
void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM1,TIM_IT_Update);//清除TIM1中断标志
	}
}
#endif
#if TIM2_IRQ==1
//TIM2中断请求函数
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除TIM2中断标志
		
		humiture_data.time2_count++;
	}
}
#endif
#if TIM3_IRQ==1
//TIM3中断请求函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//清除TIM3中断标志
		
		humiture_data.time3_count1++;
		
		if (humiture_data.alarm_flag == 1)
		{
			humiture_data.time3_count++;
			if(humiture_data.time3_count >= 450)
			{
				humiture_data.time3_count = 0;
				
				humiture_data.alarm_count++;
				if(humiture_data.alarm_count % 2)
					buzzer_set(0);
				else
					buzzer_set(1);
			}
			if(humiture_data.alarm_count >= 2)
			{
				humiture_data.time3_count = 0;
				humiture_data.alarm_count = 0;
				humiture_data.alarm_flag = 0;
				
				/* 关闭蜂鸣器 */
				buzzer_set(1);
				
//				/* 停止定时器3 */
//				OFF_TIMx(TIM3);
			}
		}
//		else
//			buzzer_set(1);
	}
}
#endif
#if TIM4_IRQ==1
//TIM4中断请求函数
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清除TIM4中断标志
		
	}
}
#endif
