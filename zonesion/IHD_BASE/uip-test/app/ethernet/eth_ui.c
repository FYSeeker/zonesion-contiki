#include "eth_ui.h"


_Menu_t ethMenu={1};

void ethMenuConfirmHandle(void)
{
    ethMenu.optionState[ethMenu.index-1] = (ethMenu.optionState[ethMenu.index-1]==SELECT)?UNSELECT:SELECT;
    if(ethMenu.optionHandle[ethMenu.index-1] != NULL)
        ethMenu.optionHandle[ethMenu.index-1](ethMenu.optionState[ethMenu.index-1]);
}

void ethMenuExitHandle()
{
    ethMenu.index=0;
}

void ethMenuKeyUpHandle()
{
    if(ethMenu.index>0)ethMenu.index--;
    if(ethMenu.index==0)
    {
        ethMenu.index=ethMenuIndexMAX;
    }
}

void ethMenuKeyDownHandle()
{
    ethMenu.index++;
    if(ethMenu.index>ethMenuIndexMAX)
    {
        ethMenu.index=1;
    }
}

void ethMenuKeyHandle(unsigned char keyStatus)
{
    switch(keyStatus)
    {
        case 0x01:
            ethMenuConfirmHandle();
            break;
        case 0x02:
            ethMenuExitHandle();
            break;
        case 0x04:
            ethMenuKeyUpHandle();
            break;
        case 0x08:
            ethMenuKeyDownHandle();
            break;
    }
}

char ethOptionCallFunc_set(unsigned char optionIndex,void (*func)(optionStatus_t))
{
    if(optionIndex>ethMenuIndexMAX) return 1;
    ethMenu.optionHandle[optionIndex-1]=func;
    return 0;
}

void ethOptinoHandle_1(optionStatus_t status)
{
    if(status==SELECT)
    {
        PROCESS_CONTEXT_BEGIN(&udp_test);
        udp_establish();
        PROCESS_CONTEXT_END();
    }
}

void ethOptinoHandle_2(optionStatus_t status)
{
    if(status==SELECT)
    {
        PROCESS_CONTEXT_BEGIN(&tcp_test);
        tcp_listen(uip_htons(1210));
        PROCESS_CONTEXT_END();
    }
    else if(status==UNSELECT)
    {
        PROCESS_CONTEXT_BEGIN(&tcp_test);
        tcp_unlisten(uip_htons(1210));
        PROCESS_CONTEXT_END();
    }
}

void ethOptinoHandle_3(optionStatus_t status)
{
    if(status==SELECT)
    {
        PROCESS_CONTEXT_BEGIN(&tcp_test);
        tcp_client_connect();
        PROCESS_CONTEXT_END();
    }
}

void ethOptinoHandle_4(optionStatus_t status)
{
    if(status==SELECT)
    {
        dhcpc_request();
    }
    else if(status==UNSELECT)
    {
    }
}

/********************************************************************************************/

void ethMenuShowOption(uint16_t x,uint16_t y,char* text,uint16_t textColor,uint16_t backColor)
{
    LCD_File(x,y+1,x+(ethMenuOpeionLen*8),y+18,backColor);
    LCDShowFont16(x+8,y+2,text,(ethMenuOpeionLen*8)-16,textColor,backColor);
}

void ethMenuShow(uint8_t refresh)
{
    uint8_t index=0;
    uint16_t x,y;
    uint16_t defaultColor = LCD_COLOR_DARK_GRAY;
    uint16_t selectColor = LCD_COLOR_DARK_BLUE;
    uint16_t indexColor1 = LCD_COLOR_GRAY;
    uint16_t indexColor2 = LCD_COLOR_BLUE;
    uint16_t penColor = LCD_COLOR_WHITE;
    uint16_t backColor = defaultColor;
    const char menuText[ethMenuIndexMAX][ethMenuOpeionLen]={"建立UDP服务","创建TCP服务器","连接TCP服务器","重启DHCP",};
    
    if(refresh)
    {
        x=8,y=0;
        backColor = (ethMenu.optionState[index++]==SELECT)?(ethMenu.index==index)?indexColor2:selectColor:(ethMenu.index==index)?indexColor1:defaultColor;
        ethMenuShowOption(x,y+=20,(char*)menuText[index-1],penColor,backColor);
        backColor = (ethMenu.optionState[index++]==SELECT)?(ethMenu.index==index)?indexColor2:selectColor:(ethMenu.index==index)?indexColor1:defaultColor;
        ethMenuShowOption(x,y+=20,(char*)menuText[index-1],penColor,backColor);
        backColor = (ethMenu.optionState[index++]==SELECT)?(ethMenu.index==index)?indexColor2:selectColor:(ethMenu.index==index)?indexColor1:defaultColor;
        ethMenuShowOption(x,y+=20,(char*)menuText[index-1],penColor,backColor);
        backColor = (ethMenu.optionState[index++]==SELECT)?(ethMenu.index==index)?indexColor2:selectColor:(ethMenu.index==index)?indexColor1:defaultColor;
        ethMenuShowOption(x,y+=20,(char*)menuText[index-1],penColor,backColor);
        
        x=168,y=0;
        backColor = (ethMenu.optionState[index++]==SELECT)?(ethMenu.index==index)?indexColor2:selectColor:(ethMenu.index==index)?indexColor1:defaultColor;
        ethMenuShowOption(x,y+=20,(char*)menuText[index-1],penColor,backColor);
        backColor = (ethMenu.optionState[index++]==SELECT)?(ethMenu.index==index)?indexColor2:selectColor:(ethMenu.index==index)?indexColor1:defaultColor;
        ethMenuShowOption(x,y+=20,(char*)menuText[index-1],penColor,backColor);
        backColor = (ethMenu.optionState[index++]==SELECT)?(ethMenu.index==index)?indexColor2:selectColor:(ethMenu.index==index)?indexColor1:defaultColor;
        ethMenuShowOption(x,y+=20,(char*)menuText[index-1],penColor,backColor);
        backColor = (ethMenu.optionState[index++]==SELECT)?(ethMenu.index==index)?indexColor2:selectColor:(ethMenu.index==index)?indexColor1:defaultColor;
        ethMenuShowOption(x,y+=20,(char*)menuText[index-1],penColor,backColor);
    }
}

void ethMenuLcd_init()
{
    
    LCDClear(LCD_COLOR_BLACK);
    LCDShowFont16(160-strlen("uip stack test")*4,2,"uip stack test",strlen("uip stack test")*8,LCD_COLOR_WHITE,LCD_COLOR_DARK_GRAY);
    
    ethOptionCallFunc_set(1,ethOptinoHandle_1);
    ethOptionCallFunc_set(2,ethOptinoHandle_2);
    ethOptionCallFunc_set(3,ethOptinoHandle_3);
    ethOptionCallFunc_set(4,ethOptinoHandle_4);
    ethMenuShow(1);
}

void ethShowAddr(uint16_t x,uint16_t y)
{
    char tempBuf[40]={0};
    
    sprintf(tempBuf,"Local IP addr:   %u.%u.%u.%u",uip_hostaddr.u8[0],uip_hostaddr.u8[1],uip_hostaddr.u8[2],uip_hostaddr.u8[3]);
    LCDShowFont16(x,y,tempBuf,300,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    
    sprintf(tempBuf,"Gateway IP addr: %u.%u.%u.%u",uip_draddr.u8[0],uip_draddr.u8[1],uip_draddr.u8[2],uip_draddr.u8[3]);
    LCDShowFont16(x,y+=20,tempBuf,300,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    
    sprintf(tempBuf,"Network Mask:    %u.%u.%u.%u",uip_netmask.u8[0],uip_netmask.u8[1],uip_netmask.u8[2],uip_netmask.u8[3]);
    LCDShowFont16(x,y+=20,tempBuf,300,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
}

void ethshowHint()
{
   
}

void ethLcdInfoUpdate()
{
    ethShowAddr(8,122);
    ethMenuShow(1);
}


PROCESS(eth_ui, "eth_ui");

PROCESS_THREAD(eth_ui, ev, data)
{
    PROCESS_BEGIN();
    
    static struct etimer eth_lcdTime;
    
    ILI93xxInit();
    ethMenuLcd_init();
    etimer_set(&eth_lcdTime, 100);
    
    while(1)
    {
        PROCESS_WAIT_EVENT();
        if(etimer_expired(&eth_lcdTime))
        {
            etimer_reset(&eth_lcdTime);
            ethLcdInfoUpdate();
        }
        if(ev == key_event)
        {
            ethMenuKeyHandle(*(uint8_t*)data);
        }
    }
    PROCESS_END();
}