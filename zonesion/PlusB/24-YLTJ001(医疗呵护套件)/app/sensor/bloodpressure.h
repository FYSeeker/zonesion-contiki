#ifndef _BLOODPRESSURE_
#define _BLOODPRESSURE_

#include <contiki.h>
#include <stdio.h>
#include "string.h"
#include <stdlib.h>

#define BP_VERSION 2

extern void operate_device(unsigned char *buf,unsigned char len);//对模块进行操作
extern void process_data(unsigned char *buf,unsigned char len); //处理缓冲区的数据
extern void data_init();

extern unsigned char Bloodpressure[9];                        	//存储血压值
extern unsigned char HHH[3]; 
extern unsigned int b1;
extern unsigned int b2;
extern unsigned int b3;
extern unsigned int b4;

void blood_init(void);
extern void blood_stop(void);
extern void blood_start(void);
void blood_result(void);

uint16_t SBP_get(void);
uint16_t DBP_get(void);

#endif
