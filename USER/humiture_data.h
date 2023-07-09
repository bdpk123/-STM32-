#ifndef __humiture_data_H
#define __humiture_data_H

#include "delay.h"
#include "sys.h"

/* 温湿度数据结构体 */
typedef struct
{
	int temp_alarm_value;    //温度报警值
	int humidity_alarm_value;//湿度报警值
	u8 page;                 //页面，连个界面，主界面和湿度控制界面
	u8 show_flag;            //显示标志
	u8 alarm_flag;           //报警标志
	u8 temp_flag;            //温度标志
	u8 humidity_flag;        //湿度标志
	u8 set_flag;             //设置标志 
	u32 time2_count;         //定时器2计数
	u32 time3_count;         //定时器3计数
	u32 time3_count1;        //定时器3计数1
	u32 alarm_count;         //报警计数
}humitureDataTypeDef;

extern humitureDataTypeDef humiture_data;

#endif
