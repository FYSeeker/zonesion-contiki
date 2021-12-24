#ifndef __RFID_H_
#define __RFID_H_
#include "stm32f10x.h"

void read_rfid(void);
void rfid_init(void);
uint8_t get_doorStatus(void);
uint8_t get_closeDoorTime(void); 
uint16_t get_readTime(void);
void set_maxLegalSum(uint8_t sum);
uint8_t get_maxLegalSum(void);
void set_doorStatus(uint8_t status);
int uart2_rec_handle(char ch);

#endif
