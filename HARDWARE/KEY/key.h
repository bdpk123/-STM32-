#ifndef __KEY_H
#define __KEY_H

#include "sys.h"
#include "timer.h"
#include "humiture_data.h"

#define KEY1_PIN PBin(3)
#define KEY2_PIN PBin(4)
#define KEY3_PIN PCin(13)

#define KEY_NULL         0//无按键按下
#define KEY1_SHORT_PRESS 1//按键1短按
#define KEY1_LONG_PRESS  2//按键1长按
#define KEY2_SHORT_PRESS 3//按键2短按
#define KEY2_LONG_PRESS  4//按键2长按
#define KEY3_SHORT_PRESS 5//按键3短按
#define KEY3_LONG_PRESS  6//按键3长按

void key_init(void);//按键初始化
u8 key_scan(void);  //按键扫描

#endif
