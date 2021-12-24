#ifndef __AMMETER_H_
#define __AMMETER_H_
#include "stm32f10x.h"

void ammeter_init(void);
void read_ammeter(void);
float get_electricQuantity(void);
float get_voltage(void);
float get_electricity(void);
float get_power(void);
int uart2_rec_handle(char ch);



#endif
