#ifndef __oled_H
#define __oled_H

#include "delay.h"
#include "sys.h"

#define OLED_MODE 2          //OLED模式,0:并行8080模式,1:4线SPI模式,2:I2C模式

#define DATAOUT(Data_Value) {GPIO_Write(GPIOB,(GPIO_ReadOutputData(GPIOB)&0xFF00)|(Data_Value&0x00FF));}

#define OLED_8080_RST PBout(14)//8080:复位(低电平有效)
#define OLED_8080_CS  PCout(9) //8080:片选信号(低电平有效)
#define OLED_8080_DC  PCout(8) //8080:数据/命令控制 控制,DC=1,传输数据,DC=0,传输指令
#define OLED_8080_RW  PCout(7) //8080:写使能信号
#define OLED_8080_RD  PCout(6) //8080:读使能信号

#define OLED_SCLK PCout(5)     //SPI:D0(SCLK)串行时钟线
#define OLED_SDIN PCout(4)     //SPI:D1(MOSI)串行数据线
#define OLED_RES  PCout(1)     //SPI:复位(低电平有效)
#define OLED_DC   PDout(2)     //SPI:数据/命令控制 控制,DC=1,传输数据,DC=0,传输指令
#define OLED_CS   PCout(11)    //SPI:片选信号(低电平有效)

#define OLED_SCL PAout(5)      //I2C:SCL串行时钟线
#define OLED_SDA PAout(4)     //I2C:SDA串行数据线

extern const unsigned char Chinese[][16];//自定义中文字库
extern const unsigned char Char6x8[][6]; //(6x8)字库
extern const unsigned char Char8x16[];   //(8x16)字库

void OLED_Init(void);                                     //OLED初始化
void Start_I2C(void);                                     //启动I2C
void Stop_I2C(void);                                      //停止I2C
u8 I2C_Detection_ACK(void);                               //I2C检测应答
void I2C_Write_Byte(u8 Data);                             //I2C写字节
void OLED_Write_Byte(u8 Data,u8 Com);                     //向OLED写入一个字节
void OLED_Set_Coordinate(u8 x,u8 y);                      //OLED设置坐标
void OLED_Display_16x16(u8 x,u8 y,u8 z,const u8 (*p)[16]);//OLED显示(16*16)
void OLED_Display_6x8_8x16(u8 x,u8 y,u8 z,const u8 *p);   //OLED显示(6*8)/(8*16)
void OLED_Dispaly_Bmp_128x64(u8 x,u8 y,const u8 *p);      //OLED显示(128*64)
void OLED_Clear(void);                                    //OLED清屏

#endif
