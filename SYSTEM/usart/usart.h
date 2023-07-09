#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "sys.h" 

#define USART1_RX_STA 256

extern u8 USART_RX_BUF[256];
extern u16 USART_RX_STA;

extern u8 USART1_RX_BUF[1];

extern u8 USART2_RX_BUF[7];//接收缓冲,最大USART_REC_LEN个字节.

extern unsigned char reca_start;//接受开始的标志
extern unsigned char reca_num;	 //接受的个数
extern unsigned char reca_ok;   //接受完成

void uart1_init(u32 bound);
void uart2_init(u32 bound);
void uart3_init(u32 bound);

void USART1_TX(u8 *data);
void USART2_TX(u8 *data);
void USART3_TX(u8 *data);

#endif


