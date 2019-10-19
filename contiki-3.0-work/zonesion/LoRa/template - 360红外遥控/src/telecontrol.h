#ifndef __TELECONTROL_H_
#define __TELECONTROL_H_
#include "stm32f10x.h"

void set_currentMode(uint8_t mode);
uint8_t get_currentMode(void);
void set_currentStatus(uint8_t status);
uint8_t get_currentStatus(void);
void set_keyValue(uint8_t value);
void send_keyValue(uint8_t value);
int uart2_rec_handle(char ch);
void telecontrol_init(void);
uint8_t get_handleFlag(void);
void set_handleFlag(uint8_t value);

#endif
