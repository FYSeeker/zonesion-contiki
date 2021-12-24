/*********************************************************************************************
* 文件：api_lcd.c
* 作者：fuyou 2018.11.1
* 说明：api_lcd.c的头文件
* 修改：
* 注释：
*********************************************************************************************/
#ifndef _api_lcd_h_
#define _api_lcd_h_

#include <stdio.h>
#include <string.h>
#include "ili93xx.h"
#include "hw.h"
#include "lcd.h"
#include "bloodpressure.h"

//套件类型
#define SuiteType       "医疗套件"

void lcdSensor_init(void);

void lcdSensor_poll(void);

#endif