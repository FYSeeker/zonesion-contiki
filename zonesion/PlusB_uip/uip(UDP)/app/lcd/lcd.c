#include "lcd.h"


void lcd_Update(uint8_t refresh)
{
    char pbuf[40] = {0};
    uint16_t y = 0;
    uint16_t PenColor = LCD_COLOR_WHITE;
    uint16_t BackColor = LCD_COLOR_BLACK;
    
    if(refresh&0x80)
    {
        LCD_DrawRectangle(5,10,315,45,LCD_COLOR_WHITE);
        LCD_DrawRectangle(5,60,315,235,LCD_COLOR_WHITE);
        
        sprintf(pbuf," Ƕ��ʽ΢����ϵͳ ");
        LCDShowFont16(160-(strlen(pbuf)*4),y+2,pbuf,strlen(pbuf)*8,LCD_COLOR_WHITE,BackColor);
        
        y+=20;
        sprintf(pbuf," Contiki OS UDPͨ�� ");
        LCDShowFont16(160-(strlen(pbuf)*4),y+2,pbuf,strlen(pbuf)*8,LCD_COLOR_RED,BackColor);
        
        y+=30;
        sprintf(pbuf," ��Ϣ ");
        LCDShowFont16(160-(strlen(pbuf)*4),y+2,pbuf,strlen(pbuf)*8,LCD_COLOR_WHITE,BackColor);
    }
    
    y=70;
    sprintf(pbuf,"����IP��ַ:   %u.%u.%u.%u",
            HostIpAddr.u8[0],HostIpAddr.u8[1],HostIpAddr.u8[2],HostIpAddr.u8[3]);
    LCDShowFont16(8,y+2,pbuf,300,PenColor,BackColor);
    
    y+=20;
    sprintf(pbuf,"����IP��ַ:   %u.%u.%u.%u",
            GatewayIpAddr.u8[0],GatewayIpAddr.u8[1],GatewayIpAddr.u8[2],GatewayIpAddr.u8[3]);
    LCDShowFont16(8,y+2,pbuf,300,PenColor,BackColor);
    
    y+=20;
    sprintf(pbuf,"��������:     %u.%u.%u.%u",
            NetworkMask.u8[0],NetworkMask.u8[1],NetworkMask.u8[2],NetworkMask.u8[3]);
    LCDShowFont16(8,y+2,pbuf,300,PenColor,BackColor);
    
    y+=30;
    sprintf(pbuf,"Զ��IP��ַ:   %u.%u.%u.%u",
            udpCB.RemoteIP.u8[0],udpCB.RemoteIP.u8[1],udpCB.RemoteIP.u8[2],udpCB.RemoteIP.u8[3]);
    LCDShowFont16(8,y+2,pbuf,300,PenColor,BackColor);
    
    y+=20;
    sprintf(pbuf,"Զ��/���ض˿�:%u/%u",udpCB.RemotePort,udpCB.LocalPort);
    LCDShowFont16(8,y+2,pbuf,300,PenColor,BackColor);
    
    y+=30;
    PenColor = LCD_COLOR_BLUE;
    uint8_t bufSize = UDP_BUF_SIZE;
    sprintf(pbuf,"��������(ֻ��ʾǰ%u���ֽ�):",bufSize);
    LCDShowFont16(8,y+2,pbuf,300,PenColor,BackColor);
    
    y+=20;
    PenColor = LCD_COLOR_PURPLE;
    if(UdpDataBuf[0] != NULL)
        LCDShowFont16(8,y+2,UdpDataBuf,301,PenColor,BackColor);
    else
        LCDShowFont16(8,y+2," ",301,PenColor,BackColor);
}


PROCESS(lcd, "lcd");

PROCESS_THREAD(lcd, ev, data)
{
    static struct etimer lcd_timer;
    
    PROCESS_BEGIN();
    
    ILI93xxInit();
    lcd_Update(0x80);
    process_post(&lcd, PROCESS_EVENT_TIMER, NULL);
    
    while (1)
    {
        PROCESS_WAIT_EVENT(); 
        if(ev == PROCESS_EVENT_TIMER)
        {
            etimer_set(&lcd_timer,100);
            lcd_Update(0x00);
        }
    }
    
    PROCESS_END();
}