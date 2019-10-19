/*********************************************************************************************
* 文件：DS18B20.c
* 作者：yangchh 2016.06.23
* 说明：DS18B20驱动程序
* 修改：
* 注释：
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
  
  RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE);	        //使能PB端口时钟
  
  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(DS18B20_GPIO, &GPIO_InitStructure);
}

void set_io_input(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE);	        //使能PB端口时钟
  
  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(DS18B20_GPIO, &GPIO_InitStructure);
}

//0--输入/1--输出
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
* 名称：DS18B20_Rst()
* 功能：DS18B20复位函数
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void DS18B20_Rst(void) 
{
    set_io(1);
    GPIO_ResetBits(DS18B20_GPIO, DS18B20_PIN);
    delay_us(600);                                              //拉低 600us
    GPIO_SetBits(DS18B20_GPIO, DS18B20_PIN);                    //DQ=1
    delay_us(600);                                              //拉高 600us
}

/*********************************************************************************************
* 名称：DS18B20_Check()
* 功能：检测DS18B20是否存在
* 参数：
* 返回：当检测到传感器存在，则返回零，否则返回1
* 修改：
* 注释：
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
* 名称：DS18B20_Read_Bit()
* 功能：读取BS18B20一位参数
* 参数：
* 返回：返回值为读取到的一位参数
* 修改：
* 注释：
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
* 名称：DS18B20_Read_Byte()
* 功能：读取BS18B20一字节
* 参数：
* 返回：返回值为读取到的一个字节
* 修改：
* 注释：
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
* 名称：DS18B20_Write_Byte()
* 功能：向DS18B20写参数
* 参数：要输入的一字节 dat
* 返回：
* 修改：
* 注释：
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
* 名称：DS18B20_Start()
* 功能：DS18B20温度转换综合指令
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void DS18B20_Start(void)
{
    DS18B20_Rst(); 
    DS18B20_Check();
    DS18B20_Write_Byte(0xcc);                                   // skip rom
    DS18B20_Write_Byte(0x44);                                   // convert
}

/*********************************************************************************************
* 名称：DS18B20_Init()
* 功能：初始化 DS18B20 的 IO 口 DQ 同时检测 DS 的存在
* 参数：
* 返回：返回 1:不存在，返回 0:存在
* 修改：
* 注释：
*********************************************************************************************/
unsigned char DS18B20_Init(void)
{
  DS18B20_Rst();
  return DS18B20_Check();
} 

/*********************************************************************************************
* 名称：DS18B20_Get_Temp()
* 功能：从 ds18b20 得到温度值
* 参数：
* 返回：返回值：温度值 （-550~1250）
* 修改：
* 注释：
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
    temp=0;                                                     //温度为负
  }
  else temp=1;                                                  //温度为正
  tem=TH;                                                       //获得高八位
  tem<<=8;
  tem+=TL;                                                      //获得底八位
  temper = (float)(tem * 0.0625);                               //转换
  if(temp)
    return temper;                                            //返回温度值
  else 
    return -temper;
}
