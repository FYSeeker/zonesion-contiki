/*********************************************************************************************
* �ļ���DS18B20.c
* ���ߣ�yangchh 2016.06.23
* ˵����DS18B20��������
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#include <stdio.h>
#include "DS18B20.h"
#include "delay.h"

#define DS18B20_GPIO    GPIOB
#define DS18B20_CLK     RCC_APB2Periph_GPIOB
#define DS18B20_PIN     GPIO_Pin_0

#define DS18B20_INPUT   GPIO_ReadInputDataBit(DS18B20_GPIO, DS18B20_PIN)


void set_io_output(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE);	        //ʹ��PB�˿�ʱ��
  
  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(DS18B20_GPIO, &GPIO_InitStructure);
}

void set_io_input(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE);	        //ʹ��PB�˿�ʱ��
  
  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(DS18B20_GPIO, &GPIO_InitStructure);
}

//0--����/1--���
void set_io(unsigned char mode)
{
  if(mode == 1) {
    set_io_output();
  }
  else {
    set_io_input();
  }
}

/*********************************************************************************************
* ���ƣ�DS18B20_Rst()
* ���ܣ�DS18B20��λ����
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void DS18B20_Rst(void) 
{
    set_io(1);
    GPIO_ResetBits(DS18B20_GPIO, DS18B20_PIN);
    delay_us(600);                                              //���� 600us
    GPIO_SetBits(DS18B20_GPIO, DS18B20_PIN);                    //DQ=1
    delay_us(600);                                              //���� 600us
}

/*********************************************************************************************
* ���ƣ�DS18B20_Check()
* ���ܣ����DS18B20�Ƿ����
* ������
* ���أ�����⵽���������ڣ��򷵻��㣬���򷵻�1
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
unsigned char DS18B20_Check(void)
{
    unsigned char retry=0;
    set_io(0);
    while (DS18B20_INPUT&&retry<200) { retry++; delay_us(1); };
    if(retry>=200)return 1;
    else retry=0;
    while (!DS18B20_INPUT&&retry<240) {retry++; delay_us(1); };
    if(retry>=240)return 1;
    return 0;
}

/*********************************************************************************************
* ���ƣ�DS18B20_Read_Bit()
* ���ܣ���ȡBS18B20һλ����
* ������
* ���أ�����ֵΪ��ȡ����һλ����
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
unsigned char DS18B20_Read_Bit(void)
{
    unsigned char data;
    set_io(1);
    GPIO_ResetBits(DS18B20_GPIO, DS18B20_PIN);
    delay_us(2);
    GPIO_SetBits(DS18B20_GPIO, DS18B20_PIN);
    set_io(0);
    delay_us(12);
    if(DS18B20_INPUT)data=1;
    else data=0; 
    delay_us(50);
    return data;
}

/*********************************************************************************************
* ���ƣ�DS18B20_Read_Byte()
* ���ܣ���ȡBS18B20һ�ֽ�
* ������
* ���أ�����ֵΪ��ȡ����һ���ֽ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
unsigned char DS18B20_Read_Byte(void)
{
    unsigned char i,j,dat;
    dat=0;
    for (i=1;i<=8;i++)
    {
	j=DS18B20_Read_Bit();
	dat=(j<<7)|(dat>>1);
    } 
    return dat;
}

/*********************************************************************************************
* ���ƣ�DS18B20_Write_Byte()
* ���ܣ���DS18B20д����
* ������Ҫ�����һ�ֽ� dat
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void DS18B20_Write_Byte(unsigned char dat)
{
    unsigned char j;
    unsigned char testb;
    set_io(1);
    for (j=1;j<=8;j++)
    {
	testb=dat&0x01;
	dat=dat>>1;
	if (testb)
	{
	    GPIO_ResetBits(DS18B20_GPIO, DS18B20_PIN);                                   // Write 1
	    delay_us(2);
	    GPIO_SetBits(DS18B20_GPIO, DS18B20_PIN);
	    delay_us(60);
	}
	else
	{
	    GPIO_ResetBits(DS18B20_GPIO, DS18B20_PIN);                                   // Write 0
	    delay_us(60);
	    GPIO_SetBits(DS18B20_GPIO, DS18B20_PIN);
	    delay_us(2);
	}
    }
}

/*********************************************************************************************
* ���ƣ�DS18B20_Start()
* ���ܣ�DS18B20�¶�ת���ۺ�ָ��
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void DS18B20_Start(void)
{
    DS18B20_Rst(); 
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);                                   // skip rom
    DS18B20_Write_Byte(0x44);                                   // convert
}

/*********************************************************************************************
* ���ƣ�DS18B20_Init()
* ���ܣ���ʼ�� DS18B20 �� IO �� DQ ͬʱ��� DS �Ĵ���
* ������
* ���أ����� 1:�����ڣ����� 0:����
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
unsigned char DS18B20_Init(void)
{
  DS18B20_Rst();
  return DS18B20_Check();
} 

/*********************************************************************************************
* ���ƣ�DS18B20_Get_Temp()
* ���ܣ��� ds18b20 �õ��¶�ֵ
* ������
* ���أ�����ֵ���¶�ֵ ��-550~1250��
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
float DS18B20_Get_Temp(void)
{
  short tem;
  static float temper = 0;
  unsigned char temp = 0;
  unsigned char TL = 0,TH = 0;
  
  DS18B20_Start();
  DS18B20_Rst();
  DS18B20_Check();
  DS18B20_Write_Byte(0xCC);                                     // skip rom
  DS18B20_Write_Byte(0xBE);                                     // convert
  TL=DS18B20_Read_Byte();                                       // LSB
  TH=DS18B20_Read_Byte();                                       // MSB
  DS18B20_Start();                                              // ds1820 start convert
  if(TH > 7){
    TH=~TH;
    TL=~TL;
    temp=0;                                                     //�¶�Ϊ��
  }
  else temp=1;                                                  //�¶�Ϊ��
  tem=TH;                                                       //��ø߰�λ
  tem<<=8;
  tem+=TL;                                                      //��õװ�λ
  temper = (float)(tem * 0.0625);                               //ת��
  if(temp)
    return temper;                                            //�����¶�ֵ
  else 
    return -temper;
}
