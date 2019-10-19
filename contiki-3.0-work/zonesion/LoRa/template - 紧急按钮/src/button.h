#ifndef __BUTTON_H_
#define __BUTTON_H_
#include "stm32f10x.h"

void button_init(void);
uint8_t get_button_status(void);

#endif
