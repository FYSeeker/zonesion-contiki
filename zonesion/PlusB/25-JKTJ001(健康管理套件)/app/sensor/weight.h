/*********************************************************************************************
* �ļ���__weight_h_
* ���ߣ�
* ˵����
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#ifndef __weight_h_
#define __weight_h_

#include <stdio.h>
#include "stm32f4xx.h"
#include <stdio.h>
#include <contiki.h>
#include "string.h"

/*********************************************************************************************
* �ⲿ����ԭ��
*********************************************************************************************/
void uart4_putc(char x);
void uart4_set_input();
void uart4_init(unsigned int bpr);

void weight_io_init(void);                                  //������̼������ʼ������
void weight_update(void);                                   //������̼���������º���
float get_weight(void);
void calibration(int weight);
void Rem_impur(uint8_t choose);

#endif  