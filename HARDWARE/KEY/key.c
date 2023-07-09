#include "key.h"

//按键初始化
void key_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//按键扫描
u8 key_scan(void)
{
	static u8 Flag = 1;
	if(Flag==1)
	{
		if(KEY1_PIN==0)
		{//按键1按下
			humiture_data.time2_count = 0;//设置定时器的初始值
			ON_TIMx(TIM2);//开启定时器进行计时
			Flag = 0;
			while((KEY1_PIN==0) && (humiture_data.time2_count<120));//等待按键释放
			if(humiture_data.time2_count>=60)//大于600ms为长按
			{
				humiture_data.time2_count = 0;
				OFF_TIMx(TIM2);//关闭定时器
				return KEY1_LONG_PRESS;//长按
			}
			else
			{
				humiture_data.time2_count = 0;
				OFF_TIMx(TIM2);
				return KEY1_SHORT_PRESS;//短按
			}
		}
		if(KEY2_PIN==0)
		{
			humiture_data.time2_count = 0;//设置定时器的初始值
			ON_TIMx(TIM2);
			Flag = 0;
			while((KEY2_PIN==0) && (humiture_data.time2_count<120));//等待按键释放
			if(humiture_data.time2_count>=60)//大于600ms为长按
			{
				humiture_data.time2_count = 0;
				OFF_TIMx(TIM2);
				return KEY2_LONG_PRESS;
			}
			else
			{
				humiture_data.time2_count = 0;
				OFF_TIMx(TIM2);
				return KEY2_SHORT_PRESS;
			}
		}
		if(KEY3_PIN==0)
		{
			humiture_data.time2_count = 0;
			ON_TIMx(TIM2);
			Flag = 0;
			while((KEY3_PIN==0) && (humiture_data.time2_count<120));//等待按键释放
			if(humiture_data.time2_count>=60)//大于600ms为长按
			{
				humiture_data.time2_count = 0;
				OFF_TIMx(TIM2);
				return KEY3_LONG_PRESS;
			}
			else
			{
				humiture_data.time2_count = 0;
				OFF_TIMx(TIM2);
				return KEY3_SHORT_PRESS;
			}
		}
	}
	if((KEY1_PIN==1) && (KEY2_PIN==1) && (KEY3_PIN==1))
	{
		humiture_data.time2_count = 0;
		OFF_TIMx(TIM2);
		Flag = 1;
	}
	return KEY_NULL;//按键无反应
}
