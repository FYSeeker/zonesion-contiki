#ifndef __AIR_H_
#define __AIR_H_
#include "stm32f10x.h"

void air_init(void);
unsigned char get_pm25OpenFlag(void);
unsigned int get_pm25(void);
float get_temp(void);
float get_humi(void);
unsigned char get_voc(void);
unsigned int get_co2(void);
void read_air(void);
void co2_correct(void);
void open_pm25(void);
void stop_pm25(void);
int uart2_rec_handle(char ch);

#endif
