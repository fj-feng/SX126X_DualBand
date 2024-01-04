
#ifndef __LCD_H
#define __LCD_H	  
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"


#define	LCDPin_RS	(1<<11)  //数据/命令选择端口	 PD11 
#define	LCDPin_RST	(1<<12 )  //硬复位	PD12
#define LCDPin_SCLK (1<<8)	//SPI时钟 PB8
#define LCDPin_SDIN (1<<9)	//SPI数据输入 PB9



#define LCD_RS(x)   GPIOD->ODR=(GPIOD->ODR&~LCDPin_RS)|(x ? LCDPin_RS:0)												    
#define LCD_RST(x)  GPIOD->ODR=(GPIOD->ODR&~LCDPin_RST)|(x ? LCDPin_RST:0)	
#define LCD_SCLK(x)   GPIOB->ODR=(GPIOB->ODR&~LCDPin_SCLK)|(x ? LCDPin_SCLK:0)
#define LCD_SDIN(x)   GPIOB->ODR=(GPIOB->ODR&~LCDPin_SDIN)|(x ? LCDPin_SDIN:0)
#define LCD_READ_SDA(x)   GPIOB->IDR=(GPIOB->IDR|LCDPin_SDIN)|(x ? LCDPin_SDIN:0)

#define IIC_SCL(x)   GPIOB->ODR=(GPIOB->ODR&~LCDPin_SCLK)|(x ? LCDPin_SCLK:0)
#define IIC_SDA(x)   GPIOB->ODR=(GPIOB->ODR&~LCDPin_SDIN)|(x ? LCDPin_SDIN:0)



///******选择哪种显示方式，在哪种方式下赋值为1******/
//#define mono_color 1
//#define gray_scale 0
//#define full_color 0




void IIC_Init(void);
void IICTWait(void);
void IC_IIC_Start(void);
void IC_IIC_Stop(void);
void Write_IIC_Data(unsigned short Data);
void W_COM(unsigned char command);
void W_DATA8(unsigned char data);


void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);  		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(unsigned char  x,unsigned char  y,unsigned char  t);
void OLED_Fill(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char dot);
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned  char size,unsigned  char mode);
void OLED_ShowNum(unsigned char x,unsigned char y, unsigned int num,unsigned char len,unsigned char size);
void OLED_ShowString(unsigned char x,unsigned char y,const unsigned char *p,unsigned char size);	 
void All_Screen(void);



/* LCD Registers */
#define R0             0x00
#define R1             0x01
#define R2             0x02
#define R3             0x03
#define R4             0x04
#define R5             0x05
#define R6             0x06
#define R7             0x07
#define R8             0x08
#define R9             0x09
#define R10            0x0A
#define R12            0x0C
#define R13            0x0D
#define R14            0x0E
#define R15            0x0F
#define R16            0x10
#define R17            0x11
#define R18            0x12
#define R19            0x13
#define R20            0x14
#define R21            0x15
#define R22            0x16
#define R23            0x17
#define R24            0x18
#define R25            0x19
#define R26            0x1A
#define R27            0x1B
#define R28            0x1C
#define R29            0x1D
#define R30            0x1E
#define R31            0x1F
#define R32            0x20
#define R33            0x21
#define R34            0x22
#define R36            0x24
#define R37            0x25
#define R40            0x28
#define R41            0x29
#define R43            0x2B
#define R45            0x2D
#define R48            0x30
#define R49            0x31
#define R50            0x32
#define R51            0x33
#define R52            0x34
#define R53            0x35
#define R54            0x36
#define R55            0x37
#define R56            0x38
#define R57            0x39
#define R59            0x3B
#define R60            0x3C
#define R61            0x3D
#define R62            0x3E
#define R63            0x3F
#define R64            0x40
#define R65            0x41
#define R66            0x42
#define R67            0x43
#define R68            0x44
#define R69            0x45
#define R70            0x46
#define R71            0x47
#define R72            0x48
#define R73            0x49
#define R74            0x4A
#define R75            0x4B
#define R76            0x4C
#define R77            0x4D
#define R78            0x4E
#define R79            0x4F
#define R80            0x50
#define R81            0x51
#define R82            0x52
#define R83            0x53
#define R96            0x60
#define R97            0x61
#define R106           0x6A
#define R118           0x76
#define R128           0x80
#define R129           0x81
#define R130           0x82
#define R131           0x83
#define R132           0x84
#define R133           0x85
#define R134           0x86
#define R135           0x87
#define R136           0x88
#define R137           0x89
#define R139           0x8B
#define R140           0x8C
#define R141           0x8D
#define R143           0x8F
#define R144           0x90
#define R145           0x91
#define R146           0x92
#define R147           0x93
#define R148           0x94
#define R149           0x95
#define R150           0x96
#define R151           0x97
#define R152           0x98
#define R153           0x99
#define R154           0x9A
#define R157           0x9D
#define R192           0xC0
#define R193           0xC1
#define R229           0xE5


/* LCD color */
#define   Black         0x0000      		/* 黑色：    0,   0,   0 */
#define   BLUE          0x001F                	/* 蓝色：    0,   0, 255 */
#define   GREEN         0x07E0                	/* 绿色：    0, 255,   0 */
#define   CYAN          0x07FF                 	/* 青色：    0, 255, 255 */
#define   RED           0xF800                	/* 红色：  255,   0,   0 */
#define   MAGENTA       0xF81F                	/* 品红：  255,   0, 255 */
#define   YELLOW        0xFFE0                	/* 黄色：  255, 255, 0   */
#define   White         0xFFFF      		/* 白色：  255, 255, 255 */
#define   NAVY          0x000F      		/* 深蓝色：  0,   0, 128 */
#define   DGREEN        0x03E0               	/* 深绿色：  0, 128,   0 */
#define   DCYAN         0x03EF                	/* 深青色：  0, 128, 128 */
#define   MAROON        0x7800                	/* 深红色：128,   0,   0 */
#define   PURPLE        0x780F                	/* 紫色：  128,   0, 128 */
#define   OLIVE         0x7BE0             	/* 橄榄绿：128, 128,   0 */
#define   LGRAY         0xC618            	/* 灰白色：192, 192, 192 */
#define   DGRAY         0x7BEF                	/* 深灰色：128, 128, 128 */

#define Line0          0
#define Line1          24
#define Line2          48
#define Line3          72
#define Line4          96
#define Line5          120
#define Line6          144
#define Line7          168
#define Line8          192
#define Line9          216

#endif /* __LCD_H */


/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
