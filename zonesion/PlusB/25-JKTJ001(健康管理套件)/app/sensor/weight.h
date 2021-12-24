/*********************************************************************************************
* 文件：__weight_h_
* 作者：
* 说明：
* 修改：
* 注释：
*********************************************************************************************/
#ifndef __weight_h_
#define __weight_h_

#include <stdio.h>
#include "stm32f4xx.h"
#include <stdio.h>
#include <contiki.h>
#include "string.h"

/*********************************************************************************************
* 外部函数原型
*********************************************************************************************/
void uart4_putc(char x);
void uart4_set_input();
void uart4_init(unsigned int bpr);

void weight_io_init(void);                                  //二氧化碳传感器始化函数
void weight_update(void);                                   //二氧化碳传感器更新函数
float get_weight(void);
void calibration(int weight);
void Rem_impur(uint8_t choose);

#endif  