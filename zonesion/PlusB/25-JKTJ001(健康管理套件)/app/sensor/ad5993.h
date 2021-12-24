#ifndef _AD5993_H_
#define _AD5993_H_

/*********************************************************************************************
* �ļ���ad5993.h
* ���ߣ�zonesion
* ˵����ad5993ͷ�ļ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "stm32f4xx.h"
#include "extend.h"
#include "stdio.h"

/*********************************************************************************************
* �ⲿԭ�ͺ���
*********************************************************************************************/
void AD5993_IIC_Init(void);
void AD5993_IIC_Start(void);
void AD5993_IIC_Stop(void);
u8 AD5993_IIC_Wait_Ack(void);
void AD5993_IIC_Ack(void);
void AD5993_IIC_NAck(void);
void AD5993_IIC_Send_Byte(u8 txd);
u8 AD5993_IIC_Read_Byte(unsigned char ack);
int AD5993_IIC_Read(char addr, char r, char *buf, int len);
int AD5993_IIC_Write(char addr, char r, char *buf, int len);
int AD5993_Write_Reg(char r, char v);
void AD5993_Init(void);
void AD5993_Start_F(unsigned int f);
void AD5993_Add_F(unsigned int f);
void AD5993_Add_N(unsigned int v);
int AD5993_Scan(short *buf);
void AD5993_Start_Delay_T(unsigned int v);

#endif //_AD5993_H_