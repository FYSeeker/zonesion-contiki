#include "test_ui.h"

void testUIupdate(uint8_t refresh)
{
    uint16_t x=8,y=2;
    uint16_t PEN_COLOR=LCD_COLOR_WHITE;
    char pbuf[50]={0};
    
    if(refresh&0x80)
    {
        LCD_FillColor(0,0,319,19,LCD_COLOR_DARK_BLUE);
        sprintf(pbuf,"Plus�ڵ���Գ���");
        LCDShowFont16(160-strlen(pbuf)*4,y,pbuf,strlen(pbuf)*8,LCD_COLOR_WHITE,LCD_COLOR_DARK_BLUE);
        sprintf(pbuf,"����˵��");
        LCDShowFont16(x,y+=20,pbuf,strlen(pbuf)*8,LCD_COLOR_BLUE,LCD_COLOR_BLACK);
        sprintf(pbuf,"LED/RGB��������˸/��ɫ��");
        LCDShowFont16(x,y+=20,pbuf,strlen(pbuf)*8,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
        sprintf(pbuf,"����K1--K4�״ΰ��»���ʾ״̬��");
        LCDShowFont16(x,y+=20,pbuf,strlen(pbuf)*8,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
        sprintf(pbuf,"K2�ٴΰ������²��ԣ�");
        LCDShowFont16(x,y+=20,pbuf,strlen(pbuf)*8,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
        sprintf(pbuf,"K3�ٴΰ��¿�/�ؼ̵���һ��");
        LCDShowFont16(x,y+=20,pbuf,strlen(pbuf)*8,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
        sprintf(pbuf,"K4�ٴΰ��¿�/�ؼ̵�������");
        LCDShowFont16(x,y+=20,pbuf,strlen(pbuf)*8,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    }
    y=122;
    sprintf(pbuf,"���Խ��");
    LCDShowFont16(x,y+=20,pbuf,strlen(pbuf)*8,LCD_COLOR_BLUE,LCD_COLOR_BLACK);
    if(testFlag_key1)
    {
        sprintf(pbuf,"K1:    ��������");
        PEN_COLOR=LCD_COLOR_GREEN;
    }
    else
    {
        sprintf(pbuf,"K1:    ���²���");
        PEN_COLOR=LCD_COLOR_WHITE;
    }
    LCDShowFont16(x,y+=20,pbuf,strlen(pbuf)*8,PEN_COLOR,LCD_COLOR_BLACK);
    if(testFlag_key2)
    {
        sprintf(pbuf,"K2:    ��������");
        PEN_COLOR=LCD_COLOR_GREEN;
    }
    else
    {
        sprintf(pbuf,"K2:    ���²���");
        PEN_COLOR=LCD_COLOR_WHITE;
    }
    LCDShowFont16(x+160,y,pbuf,strlen(pbuf)*8,PEN_COLOR,LCD_COLOR_BLACK);
    if(testFlag_key3)
    {
        sprintf(pbuf,"K3:    ��������");
        PEN_COLOR=LCD_COLOR_GREEN;
    }
    else
    {
        sprintf(pbuf,"K3:    ���²���");
        PEN_COLOR=LCD_COLOR_WHITE;
    }
    LCDShowFont16(x,y+=20,pbuf,strlen(pbuf)*8,PEN_COLOR,LCD_COLOR_BLACK);
    if(testFlag_key4)
    {
        sprintf(pbuf,"K4:    ��������");
        PEN_COLOR=LCD_COLOR_GREEN;
    }
    else
    {
        sprintf(pbuf,"K4:    ���²���");
        PEN_COLOR=LCD_COLOR_WHITE;
    }
    LCDShowFont16(x+160,y,pbuf,strlen(pbuf)*8,PEN_COLOR,LCD_COLOR_BLACK);
    
    if(testFlag_uart1&0x02)
    {
        sprintf(pbuf,"RF����:ͨ��ʧ��   ");
        PEN_COLOR=LCD_COLOR_RED;
    }
    else if(testFlag_uart1&0x01)
    {
        sprintf(pbuf,"RF����:ͨ������ ");
        PEN_COLOR=LCD_COLOR_GREEN;
    }
    else
    {
        sprintf(pbuf,"����1: ������...");
        PEN_COLOR=LCD_COLOR_WHITE;
    }
    LCDShowFont16(x,y+=20,pbuf,strlen(pbuf)*8,PEN_COLOR,LCD_COLOR_BLACK);
    
    if(testFlag_flash&0x02)
    {
        sprintf(pbuf,"flash: ��дʧ�� ");
        PEN_COLOR=LCD_COLOR_RED;
    }
    else if(testFlag_flash&0x01)
    {
        sprintf(pbuf,"flash: ��д���� ");
        PEN_COLOR=LCD_COLOR_GREEN;
    }
    else
    {
        sprintf(pbuf,"flash: ������...");
        PEN_COLOR=LCD_COLOR_WHITE;
    }
    LCDShowFont16(x+160,y,pbuf,strlen(pbuf)*8,PEN_COLOR,LCD_COLOR_BLACK);

    if(testFlag_ethernet&0x02)
    {
        sprintf(pbuf,"��̫��:ͨ��ʧ�� ");
        PEN_COLOR=LCD_COLOR_RED;
    }
    else if(testFlag_ethernet&0x01)
    {
        sprintf(pbuf,"��̫��:ͨ������ ");
        PEN_COLOR=LCD_COLOR_GREEN;
    }
    else
    {
        sprintf(pbuf,"��̫��:������...");
        PEN_COLOR=LCD_COLOR_WHITE;
    }
    LCDShowFont16(x,y+=20,pbuf,strlen(pbuf)*8,PEN_COLOR,LCD_COLOR_BLACK);
}

//test ui process
PROCESS(test_ui, "test_ui");

PROCESS_THREAD(test_ui, ev, data)
{
    PROCESS_BEGIN();
    
    static struct etimer etimer_testUI;
    
    ILI93xxInit();
    LCDClear(LCD_COLOR_BLACK);
    testUIupdate(0x80);
    process_post(&test_ui,PROCESS_EVENT_TIMER,NULL);
    
    while (1)
    {
        PROCESS_WAIT_EVENT(); 
        if(ev == PROCESS_EVENT_TIMER)
        {
            etimer_set(&etimer_testUI,100);
            testUIupdate(0x00);
        }
    }
    
    PROCESS_END();
}