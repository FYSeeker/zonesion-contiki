/*********************************************************************************************
* �ļ���api_lcd.c
* ���ߣ�fuyou 2018.11.1
* ˵����api_lcd.c��ͷ�ļ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#ifndef _api_lcd_h_
#define _api_lcd_h_

#include <stdio.h>
#include <string.h>
#include "fml_lcd/fml_lcd.h"
#include "hw.h"
#include "lcd.h"

//�׼�����
#define SuiteType       "��ҵ�׼�"

void lcdSensor_init(void);

void lcdSensor_poll(void);

#endif