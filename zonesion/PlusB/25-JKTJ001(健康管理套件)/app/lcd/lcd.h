#ifndef _lcd_h_
#define _lcd_h_

#include <stdio.h>
#include <contiki.h>
#include "zigbee-net.h"
#include "ble-net.h"
#include "wifi-net.h"
#include "lora-net.h"
#include "ili93xx.h"
//#include "image.h"
#include "htu21d.h"
#include "BatteryVoltage.h"
#include "hw.h"
#include "key.h"
#include "api_lcd.h"
#include "lcdMenu.h"


#define LCD_REDRAW_EN 0x80      // πƒ‹÷ÿªÊ


typedef struct
{
    uint16_t wide;
    uint16_t high;
    uint16_t color;
    uint16_t textColor;
    char* text;
    uint8_t style;
}Bar_t;

extern uint8_t lcdPageIndex;

PROCESS_NAME(LcdProcess);

void lcdShowBarX1(unsigned short y,char* buf,unsigned short wordColor,unsigned short backColor,
                  unsigned char refresh,unsigned char mode);
void lcdShowBarX3(unsigned short y,char* buf1,char* buf2,char* buf3,
                  unsigned short wordColor,unsigned short backColor,unsigned char refresh,unsigned char mode);

void lcdShowPage(unsigned char refresh);

#endif