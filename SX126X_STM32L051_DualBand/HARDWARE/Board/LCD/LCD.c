#include "stm32l4xx_hal.h"
#include "lcd.h"
#include "oledfont.h"


/*******************************************************************************
* Function Name  : STM32_SSD1963_Init
* Description    : Initializes the SSD1963.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void OLED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    __HAL_RCC_GPIOB_CLK_ENABLE();	//ʹ��GPIOBʱ��
    __HAL_RCC_GPIOD_CLK_ENABLE();	//ʹ��GPIOBʱ��

    GPIO_InitStructure.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP ;   //�������
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9 | GPIO_PIN_8, GPIO_PIN_SET); 	//PB6,PB7 �����

    GPIO_InitStructure.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP ;   //�������
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11 | GPIO_PIN_12, GPIO_PIN_SET); 	//PB6,PB7 �����

    HAL_Delay(5); /* delay 50 ms */
    LCD_RST(1) ;
    LCD_RS(0);
    HAL_Delay(100);
    LCD_RST(0);//Ӳ����λ
    HAL_Delay(100);
    LCD_RST(1) ;
    HAL_Delay(10);


    /*Һ��������ʼ��*/

    W_COM(0xae);

    W_COM(0xa1);    //segment remap

    W_COM(0xda);    //common pads hardware: alternative
    W_COM(0x12);

    W_COM(0xc0);    //common output scan direction:com63~com0

    W_COM(0xa8);    //multiplex ration mode:63
    W_COM(0x3f);

    W_COM(0xd5);    //display divide ratio/osc. freq. mode
    W_COM(0x70);    //Osc. Freq:320kHz,DivideRation:1

    W_COM(0x81);    //contrast control
    W_COM(0x60);    // mode:64

    W_COM(0xd9);	   //set pre-charge period
    W_COM(0x22);	   //set period 1:1;period 2:15

    W_COM(0x20);    //Set Memory Addressing Mode
    W_COM(0x02);    //page addressing mode

    W_COM(0xdb);    //VCOM deselect level mode

    W_COM(0x3c);	   //set Vvcomh=0.83*Vcc

    W_COM(0xad);    //master configuration

    W_COM(0x8e);    //external VCC supply

    W_COM(0xa4);    //out follows RAM content

    W_COM(0xa6);    //set normal display
    W_COM(0xaf);
    OLED_Clear();
}


//ʱ����ʱ
void IICTWait(void)				  //1=200ns
{
    volatile unsigned long i, j;
    for (i = 0; i < 1; i++)
    {
        for (j = 0; j < 10; j++);  		 //2020
    }
}

//IIC ��ʼ

void IC_IIC_Start(void)
{

    LCD_SDIN(1);
    IICTWait();
    LCD_SCLK(1);
    IICTWait();
    LCD_SDIN(0);
    IICTWait();
    LCD_SCLK(0);
    IICTWait();
}


//IIC ֹͣ
void IC_IIC_Stop(void)
{
    LCD_SDIN(0);
    IICTWait();
    LCD_SCLK(1);
    IICTWait();

    LCD_SDIN(1);
    IICTWait();
}


//IICд����
void Write_IIC_Data(unsigned short Data)
{
    unsigned char i;

    for(i = 0; i < 8; i++)
    {
        if(Data & 0x80)

            LCD_SDIN(1);
        else
            LCD_SDIN(0);

        LCD_SCLK(1);
        IICTWait();
//	Delay(1);
        LCD_SCLK(0);
        IICTWait();
        Data = Data << 1;
    }
    LCD_SDIN(1);			    //�ͷ�IIC SDA����Ϊ���������մ���������Ӧ���ź�
    IICTWait();
    LCD_SCLK(1);
    IICTWait();				   //��9��ʱ������
    LCD_SCLK(0);
//		IICTWait();
//		while(  LCD_SDIN(1))
//	{
//		if((i++) > 210)
//		{
//		LCD_SCLK(0);
//				IICTWait();
//			return 0;	//��Ӧ��
//		}
//	}
//	LCD_SCLK(0);
//	IICTWait();//Ӧ��
//	return 1;
//
}

//д����
void W_COM(unsigned char command)
{
    IC_IIC_Start();
    Write_IIC_Data(0x78);                //Salve Adress
    Write_IIC_Data(0x00);                   //д����
    Write_IIC_Data(command);
    IC_IIC_Stop();
}


//д����
void W_DATA8(unsigned char data)
{
    IC_IIC_Start();
    Write_IIC_Data(0x78);                //Salve Adress
    Write_IIC_Data(0x40);                      //д����
    Write_IIC_Data(data);
    IC_IIC_Stop();
}




//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127
unsigned char OLED_GRAM[128][8];

//����OLED��ʾ
void OLED_Display_On(void)
{
    W_COM(0X8D);  //SET DCDC����
    W_COM(0X14);  //DCDC ON
    W_COM(0XAF);  //DISPLAY ON
}
//�ر�OLED��ʾ
void OLED_Display_Off(void)
{
    W_COM(0X8D);  //SET DCDC����
    W_COM(0X10);  //DCDC OFF
    W_COM(0XAE);  //DISPLAY OFF
}

//�����Դ浽LCD
void OLED_Refresh_Gram(void)
{
    unsigned char i, n;
    for(i = 0; i < 8; i++)
    {

        W_COM(0xb0 + i); //����ҳ��ַ��0~7�
        W_COM (0x10);      //������ʾλ�á��е͵�ַ
        W_COM (0x00);      //������ʾλ�á��иߵ�ַ
        for(n = 0; n < 128; n++)W_DATA8(OLED_GRAM[n][i]);
    }
}

//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
void OLED_Clear(void)
{
    unsigned char i, n;
    for(i = 0; i < 8; i++)for(n = 0; n < 128; n++)OLED_GRAM[n][i] = 0X00;
    OLED_Refresh_Gram();//������ʾ
}

//����
//x:0~127
//y:0~63
//t:1 ��� 0,���
void OLED_DrawPoint(unsigned char x, unsigned char y, unsigned char t)
{
    unsigned char pos, bx, temp = 0;
    if(x > 127 || y > 63)return; //������Χ��.
    pos = 7 - y / 8;
    bx = y % 8;
    temp = 1 << (7 - bx);
    if(t)OLED_GRAM[x][pos] |= temp;
    else OLED_GRAM[x][pos] &= ~temp;
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63
//dot:0,���;1,���
void OLED_Fill(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char dot)
{
    unsigned char x, y;
    for(x = x1; x <= x2; x++)
    {
        for(y = y1; y <= y2; y++)OLED_DrawPoint(x, y, dot);
    }
    OLED_Refresh_Gram();//������ʾ
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ
//size:ѡ������ 12/16/24
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char chr, unsigned char size, unsigned char mode)
{
    unsigned char temp, t, t1;
    unsigned char y0 = y;
    unsigned char csize = (size / 8 + ((size % 8) ? 1 : 0)) * (size / 2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
    chr = chr - ' '; //�õ�ƫ�ƺ��ֵ
    for(t = 0; t < csize; t++)
    {
        if(size == 12)temp = asc2_1206[chr][t]; 	 	//����1206����
        else if(size == 16)temp = asc2_1608[chr][t];	//����1608����
        else if(size == 24)temp = asc2_2412[chr][t];	//����2412����
        else return;								//û�е��ֿ�
        for(t1 = 0; t1 < 8; t1++)
        {
            if(temp & 0x80)OLED_DrawPoint(x, y, mode);
            else OLED_DrawPoint(x, y, !mode);
            temp <<= 1;
            y++;
            if((y - y0) == size)
            {
                y = y0;
                x++;
                break;
            }
        }
    }
}

//m^n����
unsigned int mypow(unsigned char m, unsigned char n)
{
    unsigned int result = 1;
    while(n--)result *= m;
    return result;
}

//��ʾ2������
//x,y :�������
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);
void OLED_ShowNum(unsigned char x, unsigned char y, unsigned int num, unsigned char len, unsigned char size)
{
    unsigned char t, temp;
    unsigned char enshow = 0;
    for(t = 0; t < len; t++)
    {
        temp = (num / mypow(10, len - t - 1)) % 10;
        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                OLED_ShowChar(x + (size / 2)*t, y, ' ', size, 1);
                continue;
            } else enshow = 1;

        }
        OLED_ShowChar(x + (size / 2)*t, y, temp + '0', size, 1);
    }
}
//��ʾ�ַ���
//x,y:�������
//size:�����С
//*p:�ַ�����ʼ��ַ
void OLED_ShowString(unsigned char x, unsigned char y, const unsigned char *p, unsigned char size)
{
    while((*p <= '~') && (*p >= ' ')) //�ж��ǲ��ǷǷ��ַ�!
    {
        if(x > (128 - (size / 2))) {
            x = 0;    //����
            y += size;
        }
        if(y > (64 - size)) {
            y = x = 0;
            OLED_Clear();
        }
        OLED_ShowChar(x, y, *p, size, 1);
        x += size / 2;                         //����
        p++;
    }
}

void All_Screen(void)
{
    unsigned char k, i;

    for(k = 0; k < 8; k++)
    {
        W_COM(0xb0 + k);
        W_COM(0x10);
        W_COM(0x00);
        for(i = 0; i < 128; i++)
        {
            W_DATA8(0xff);
        }
    }
}



