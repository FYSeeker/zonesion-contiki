#ifndef __SIGNALLAMP_H_
#define __SIGNALLAMP_H_
#include "stm32f10x.h"

void signalLamp_init(void);
void set_intervalTime(uint8_t time);
int uart2_rec_handle(char ch);
void up_signalLamp(uint8_t cmd);


#endif
