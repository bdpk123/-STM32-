#include "oled.h"
#include "font.h"

//OLED初始化
void OLED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	                                            //使能AFIO时钟(使能功能复用IO)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);  //使能PB,PC,PD端口时钟
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);                                         //JTAG-DP失能+SW-DP使能
	

#if OLED_MODE==0
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                                            //IO口速度为50MHz
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_Write(GPIOB,0xFFFF);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                                            //IO口速度为50MHz
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9);

	
#elif OLED_MODE==1||OLED_MODE==2
	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_11|GPIO_Pin_13;//设置PC1,PC4,PC5,PC11引脚
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	                                            //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                                            //IO口速度为50MHz
//	GPIO_Init(GPIOC,&GPIO_InitStructure); 	                                                        //初始化GPIOC
//	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2; 	                                                    //设置PD2引脚
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	                                            //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                                            //IO口速度为50MHz
//	GPIO_Init(GPIOD,&GPIO_InitStructure); 	                                                        //初始化GPIOD
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5;//设置引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	                                            //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	                                            //IO口速度为50MHz
	GPIO_Init(GPIOA,&GPIO_InitStructure); 	                                                        //初始化GPIOC
	
		
//	OLED_RES=0;
//	delay_ms(200);
//	OLED_RES=1;
//	delay_ms(200);
	
#endif
	//写入一系列命令
	OLED_Write_Byte(0xAE,0);//关闭显示命令。
	OLED_Write_Byte(0x00,0);
	OLED_Write_Byte(0x10,0);
	OLED_Write_Byte(0x40,0);
	OLED_Write_Byte(0x81,0);//命令，设置对比度。
	OLED_Write_Byte(0xCF,0);
	OLED_Write_Byte(0xA1,0);
	OLED_Write_Byte(0xC8,0);
	OLED_Write_Byte(0xA6,0);
	OLED_Write_Byte(0xA8,0);
	OLED_Write_Byte(0x3F,0);
	OLED_Write_Byte(0xD3,0);
	OLED_Write_Byte(0x00,0);
	OLED_Write_Byte(0xD5,0);
	OLED_Write_Byte(0x80,0);
	OLED_Write_Byte(0xD9,0);
	OLED_Write_Byte(0xF1,0);
	OLED_Write_Byte(0xDA,0);
	OLED_Write_Byte(0x12,0);
	OLED_Write_Byte(0xDB,0);
	OLED_Write_Byte(0x40,0);
	OLED_Write_Byte(0x20,0);
	OLED_Write_Byte(0x02,0);
	OLED_Write_Byte(0x8D,0);
	OLED_Write_Byte(0x14,0);
	OLED_Write_Byte(0xA4,0);
	OLED_Write_Byte(0xA6,0);
	OLED_Write_Byte(0xAF,0);//开启显示命令。
	
	OLED_Clear();//OLED清屏
}
//启动I2C
void Start_I2C(void)
{
	OLED_SDA=1;
	delay_us(5);
}
//停止I2C
void Stop_I2C(void)
{
	OLED_SDA=0;
	delay_us(5);
}
//I2C检测应答
u8 I2C_Detection_ACK(void)
{
	u8 ACK_bit;
	OLED_SDA=1;
	delay_us(5);
	OLED_SCL=1;
	delay_us(1);
	ACK_bit=OLED_SDA;//应答信号,0:应答,1:非应答
	OLED_SCL=0;
	return ACK_bit;
}
//I2C写字节
//Data:数据/命令
void I2C_Write_Byte(u8 Data)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(Data&0x80)
			OLED_SDA=1;
		else
			OLED_SDA=0;
	}
    I2C_Detection_ACK();//I2C检测应答
}
#if OLED_MODE==0//并行8080模式
//向OLED写入一个字节
//Data:数据/命令
//Com:1:表示数据
//    0:表示命令
void OLED_Write_Byte(u8 Data,u8 Com)
{
	DATAOUT(Data);
 	OLED_8080_DC=Com;
	OLED_8080_CS=0;
	OLED_8080_RW=0;
	OLED_8080_RW=0;
	OLED_8080_CS=0; 
	OLED_8080_DC=1;
}
#endif
#if OLED_MODE==1//4线SPI模式
//向OLED写入一个字节
//Data:数据/命令
//Com:1:表示数据
//    0:表示命令
void OLED_Write_Byte(u8 Data,u8 Com)
{
	u8 i;
	OLED_CS=0;
	OLED_DC=Com;
	for(i=0;i<8;i++)
	{
		OLED_SCLK=0;
		if(Data&0x80)
			OLED_SDIN=1;
		else
			OLED_SDIN=0;
		OLED_SCLK=1;
		Data<<=1;
	}
	OLED_CS=1;
}
#endif
#if OLED_MODE==2//I2C模式
//向OLED写入一个字节
//Data:数据/命令
//Com:1:表示数据
//    0:表示命令
void OLED_Write_Byte(u8 Data,u8 Com)
{
	Start_I2C();             //启动I2C
	I2C_Write_Byte(0x78);    //OLED地址(从机地址)，SA0=0,RW=0 
	if(Com)//Com:1:表示数据	0:表示命令
		I2C_Write_Byte(0x40);//写入数据
}
#endif
//OLED设置坐标
//x:页地址(0~7)
//y:列地址(0~127)
void OLED_Set_Coordinate(u8 x,u8 y)
{//B0~B7设置页地址，一共8页。
	OLED_Write_Byte(0xB0+x,0);            //写入设置页地址命令（哪一页）
	OLED_Write_Byte(((0xF0&y)>>4)|0x10,0);//写入设置列地址(高四位)	
	OLED_Write_Byte((0x0F&y),0);          //写入设置列地址(低四位)
}
//OLED显示(16*16)
//x:页地址(0~7)
//y:列地址(0~127)
//z:(16*16)数组数据选择
//p:(16*16)数组数据缓存首地址
void OLED_Display_16x16(u8 x,u8 y,u8 z,const u8 (*p)[16])
{
	u8 i;
	OLED_Set_Coordinate(x,y);//OLED设置坐标
	for(i=0;i<16;i++)
		OLED_Write_Byte(p[2*z][i],1);
	OLED_Set_Coordinate(x+1,y);
	for(i=0;i<16;i++)
		OLED_Write_Byte(p[2*z+1][i],1);
}
//OLED显示(6*8)/(8*16)
//x:页地址(0~7)
//y:列地址(0~127)
//z:0:显示(6*8)
//  1:显示(8*16)
//p:数组数据缓存首地址
void OLED_Display_6x8_8x16(u8 x,u8 y,u8 z,const u8 *p)
{
	u8 i=0,j,n;
	while(p[i]!='\0')
	{
		n=p[i]-' ';
		if(z==0)
		{
			OLED_Set_Coordinate(x+1,y);
			for(j=0;j<6;j++)
				OLED_Write_Byte(Char6x8[n][j],1);
		}
		i++;
		y+=8;
	}
}
//OLED显示大小(128*64)
//x:页地址(0~7)
//y:列地址(0~127)
//p:数组数据缓存首地址
void OLED_Dispaly_Bmp_128x64(u8 x,u8 y,const u8 *p)
{
	u8 i,j;
	u32 n=0;
	for(i=x;i<8;i++)
	{
		OLED_Set_Coordinate(i,y);
		for(j=y;j<128;j++)
			OLED_Write_Byte(p[n++],1);
	}
}
//OLED清屏
void OLED_Clear()
{
	u8 i,j;
	for(i=0;i<8;i++)
	{
		OLED_Write_Byte(0xB0+i,0);  //设置页地址命令
		OLED_Write_Byte(0x00,0);    //设置列地址低四位命令
		OLED_Write_Byte(0x10,0);    //设置列地址高四位命令
		for(j=0;j<128;j++)
		OLED_Write_Byte(0x00,1);//写数据0
	}
}
