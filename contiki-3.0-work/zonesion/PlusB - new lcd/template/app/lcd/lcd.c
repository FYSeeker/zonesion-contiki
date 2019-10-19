#include "lcd.h"

PROCESS_NAME(sensor_process);

uint8_t SysInit_Status=0;
uint8_t lcdPageIndex=0;

void RollArray(char* buf,uint8_t len,char dat)
{
    for(uint8_t i=1;i<len;i++)
    {
        buf[i-1] = buf[i];
    }
    buf[len-1] = dat;
}

void lcdInitPage(char status,short tiem)
{
    char pbuf[80]={0};
    uint16_t x = 8;
    uint16_t y = 10;
    
    if(status==1)
    {
        LCDShowFont16(x,y+2,"系统初始化...",300,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
        LCDShowFont16(x,y+32,"无线初始化...",300,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    }
    else if(status==2)
    {
        LCDShowFont16(x,y+62,"获取无线信息...",300,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
    }
    else if(status==3)
    {
        LCDShowFont16(x,y+92,"初始化完成，加载界面...",300,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
        delay_ms(200);
        process_post(&LcdProcess, RF_InitEvent, NULL);
    }
    
    sprintf(pbuf,"    剩余加载时间:%d秒    ",tiem);
    LCDShowFont16(160-(strlen(pbuf)*4),202,pbuf,300,LCD_COLOR_WHITE,LCD_COLOR_BLACK);

    memset(pbuf, 0, sizeof(pbuf)/sizeof(pbuf[0]));
    sprintf(pbuf,"若加载失败，请复位或检查设备");
    LCDShowFont16(160-(strlen(pbuf)*4),222,pbuf,300,LCD_COLOR_WHITE,LCD_COLOR_BLACK);
}


void setBar(Bar_t* bar,uint16_t wide,uint16_t high,uint16_t color,uint16_t textColor,char* text,uint8_t style)
{
    bar->wide = wide;
    bar->high = high;
    bar->color = color;
    bar->textColor = textColor;
    bar->text = text;
    bar->style = style;
}

void drawBar(uint16_t x,uint16_t y,uint16_t redraw,Bar_t* bar)
{
    if(redraw&LCD_REDRAW_EN)
    {
        LCD_FillColor(x,y,x+bar->wide-1,y+bar->high-1,bar->color);
        if(bar->style&0x01)
        {
            LCD_DrawRectangle(x,y,x+bar->wide-1,y+bar->high-1,LCD_COLOR_WHITE);
        }
    }
    if(bar->style&0x02)//左对齐
        LCDShowFont16(x+5,y+(bar->high-1 - 16)/2,bar->text,strlen(bar->text)*8,
                      bar->textColor,bar->color);
    else
        LCDShowFont16(x+(bar->wide-1 - strlen(bar->text)*8)/2,y+(bar->high-1 - 16)/2,bar->text,
                      strlen(bar->text)*8,bar->textColor,bar->color);
}

//显示条目-1
void lcdShowBarX1(unsigned short y,char* buf,unsigned short wordColor,unsigned short backColor,
                  unsigned char refresh,unsigned char mode)
{
    Bar_t bar;
    setBar(&bar,320,20,backColor,wordColor,buf,mode);
    drawBar(0,y,refresh,&bar);
}

void ShowBar_2(unsigned short y,char* buf1,char* buf2,
               unsigned short wordColor,unsigned short backColor,unsigned char refresh,unsigned char mode)
{
    Bar_t bar;
    setBar(&bar,80,20,backColor,wordColor,buf1,mode);
    drawBar(0,y,refresh,&bar);
    
    setBar(&bar,240,20,backColor,wordColor,buf2,mode+0x02);//左对齐
    drawBar(80,y,refresh,&bar);
}

//显示条目-3
void lcdShowBarX3(unsigned short y,char* buf1,char* buf2,char* buf3,
                  unsigned short wordColor,unsigned short backColor,unsigned char refresh,unsigned char mode)
{
    unsigned short width = (unsigned short)(320.0/3.0);
    
    Bar_t bar;
    setBar(&bar,width,20,backColor,wordColor,buf1,mode);
    drawBar(0,y,refresh,&bar);
    
    setBar(&bar,width,20,backColor,wordColor,buf2,mode);
    drawBar(width,y,refresh,&bar);
    
    setBar(&bar,width+2,20,backColor,wordColor,buf3,mode);
    drawBar(width*2,y,refresh,&bar);
}

//显示条目-4
void lcdShowBarX4(unsigned short y,char* buf1,char* buf2,char* buf3,char* buf4,
                  unsigned short wordColor,unsigned short backColor,unsigned char refresh,unsigned char mode)
{
    unsigned short width = 80;
    
    Bar_t bar;
    setBar(&bar,width,20,backColor,wordColor,buf1,mode);
    drawBar(0,y,refresh,&bar);
    
    setBar(&bar,width,20,backColor,wordColor,buf2,mode);
    drawBar(width,y,refresh,&bar);
    
    setBar(&bar,width,20,backColor,wordColor,buf3,mode);
    drawBar(width*2,y,refresh,&bar);
    
    setBar(&bar,width,20,backColor,wordColor,buf4,mode);
    drawBar(width*3,y,refresh,&bar);
}

//显示温度、湿度、电压
void lcdShowHumiture(unsigned short y,unsigned short wordColor,unsigned short backColor,unsigned char refresh)
{
    static unsigned char tick=0;
    char buf1[32]={0}; 
    char buf2[32]={0};
    static char buf3[32]={0};
    
    sprintf(buf1,"温度:%4.1f℃",Htu21dTemperature_Get());
    sprintf(buf2,"湿度:%4.1f％",Htu21dHumidity_Get());
    
    tick++;
    if(tick>4)
    {
        tick = 0;
        sprintf(buf3,"电压:%4.1fV",BatteryVotage_Get());
    }
    
    lcdShowBarX3(y,buf1,buf2,buf3,wordColor,backColor,refresh,0x01);
}

void lcdPageDefault(unsigned char refresh)
{
    if(refresh&0x80)
    {
        LCDClear(LCD_COLOR_WHITE);
        lcdShowBarX1(0,"无线信息获取失败,仅显示传感器数据",LCD_COLOR_RED,LCD_COLOR_YELLOW,refresh,0);
    }
    lcdShowSensorInfo(120-30,LCD_COLOR_DARK_RED,LCD_COLOR_BLACK,LCD_COLOR_LIGHT_GRAY,refresh);
    lcdShowHumiture(220,LCD_COLOR_BLACK,LCD_COLOR_LIGHT_GRAY,refresh);
}


void lcdPageNewRF(unsigned char refresh)
{
    if(refresh&0x80)
    {
        LCDClear(LCD_COLOR_WHITE);
        lcdShowBarX1(0,"未知的无线类型",LCD_COLOR_RED,LCD_COLOR_YELLOW,refresh,0);
    }
    lcdShowSensorInfo(120-30,LCD_COLOR_DARK_RED,LCD_COLOR_BLACK,LCD_COLOR_LIGHT_GRAY,refresh);
    lcdShowHumiture(220,LCD_COLOR_BLACK,LCD_COLOR_LIGHT_GRAY,refresh);
}

void lcdPageZigbee(unsigned char refresh)
{
    u16 y=0;
    u16 wordColor = LCD_COLOR_DARK_RED;
    u16 TitleColor = LCD_COLOR_BLACK;
    u16 backColor_2 = LCD_COLOR_LIGHT_GRAY;
    char buf1[32]={0};
    char buf2[32]={0};
     
    if(refresh&0x80)
    {
        LCDClear(LCD_COLOR_WHITE);                              //清屏
        lcdShowBarX1(0,"智能增强型工业节点",LCD_COLOR_WHITE,LCD_COLOR_RED,refresh,0);
        LCDShowFont16(319-(8*3),2,"K1>",24,LCD_COLOR_GRAY,LCD_COLOR_RED);
        
        sprintf(buf1,"MAC:%s",zigbeeGat_mac());
        lcdShowBarX1(220,buf1,LCD_COLOR_WHITE,LCD_COLOR_RED,refresh,0);
    }
    y=30;
    lcdShowBarX1(y,"网络信息",LCD_COLOR_WHITE,TitleColor,refresh,0);
    y+=20;
    lcdShowBarX4(y,"网络类型","ZigBee","射频芯片","CC2530",wordColor,backColor_2,refresh,1);
    
    y+=20;
    sprintf(buf1,"%u",zigbeeGat_panid());
    sprintf(buf2,"%u",zigbeeGat_channel());
    lcdShowBarX4(y,"PANID",buf1,"通信信道",buf2,wordColor,backColor_2,refresh,1);
    
    y+=20;
    switch(zigbeeGat_type())
    {
        case 0:
            sprintf(buf1,"汇集节点");
            break;
        case 1:
            sprintf(buf1,"路由节点");
            break;
        case 2:
            sprintf(buf1,"终端节点");
            break;
        default:
            sprintf(buf1,"未知节点");
    }
    if(zigbeeGat_link())
    {
        sprintf(buf2,"已连接");
    }
    else
    {
        sprintf(buf2,"已断开");
    }
    lcdShowBarX4(y,"连接状态",buf1,"节点类型",buf2,wordColor,backColor_2,refresh,1);
    
    y+=35;
    lcdShowSensorInfo(y,wordColor,TitleColor,backColor_2,refresh);
    lcdShowHumiture(200,wordColor,backColor_2,refresh);
}

void lcdPageBle(unsigned char refresh)
{
    u16 y=0;
    u16 wordColor = LCD_COLOR_DARK_BLUE;
    u16 TitleColor = LCD_COLOR_BLACK;
    u16 backColor_2 = LCD_COLOR_LIGHT_GRAY;
    char buf1[32]={0};
     
    if(refresh&0x80)
    {
        LCDClear(LCD_COLOR_WHITE);                              //清屏
        lcdShowBarX1(0,"智能增强型工业节点",LCD_COLOR_WHITE,LCD_COLOR_BLUE,refresh,0);
        LCDShowFont16(319-(8*3),2,"K1>",24,LCD_COLOR_GRAY,LCD_COLOR_BLUE);
        
        sprintf(buf1,"MAC:%s",bleGat_mac());
        lcdShowBarX1(220,buf1,LCD_COLOR_WHITE,LCD_COLOR_BLUE,refresh,0);
    }
    y=40;
    lcdShowBarX1(y,"网络信息",LCD_COLOR_WHITE,TitleColor,refresh,0);
    y+=20;
    lcdShowBarX4(y,"网络类型","BLE","射频芯片","CC2540",wordColor,backColor_2,refresh,1);
    
    y+=20;
    if(bleGat_link()==1)
    {
        sprintf(buf1,"已连接");
    }
    else
    {
        sprintf(buf1,"已断开");
    }
    lcdShowBarX4(y,"连接状态",buf1," "," ",wordColor,backColor_2,refresh,1);
    
    y+=40;
    lcdShowSensorInfo(y,wordColor,TitleColor,backColor_2,refresh);
    lcdShowHumiture(200,wordColor,backColor_2,refresh);
}

void lcdPageWifi(unsigned char refresh)
{
    u16 y=0;
    u16 wordColor = LCD_COLOR_DARK_GREEN;
    u16 TitleColor = LCD_COLOR_BLACK;
    u16 backColor_2 = LCD_COLOR_LIGHT_GRAY;
    char buf1[32]={0};
     
    if(refresh&0x80)
    {
        LCDClear(LCD_COLOR_WHITE);                              //清屏
        lcdShowBarX1(0,"智能增强型工业节点",LCD_COLOR_WHITE,LCD_COLOR_DARK_GREEN,refresh,0);
        LCDShowFont16(319-(8*3),2,"K1>",24,LCD_COLOR_GRAY,LCD_COLOR_DARK_GREEN);
    }
    y=20;
    lcdShowBarX1(y,"网络信息",LCD_COLOR_WHITE,TitleColor,refresh,0);
    y+=20;
    lcdShowBarX4(y,"网络类型","WiFi","射频芯片","CC3200",wordColor,backColor_2,refresh,1);
    
    y+=20;
    ShowBar_2(y,"名称",wifiGat_ssid(),wordColor,backColor_2,refresh,1);
    
    y+=20;
    if(wifiGat_keyType()==1)
    {
        strcpy(buf1,wifiGat_key());
    }
    else
    {
        sprintf(buf1,"无加密\0");
    }
    ShowBar_2(y,"密码",buf1,wordColor,backColor_2,refresh,1);
    
    y+=20;
    char* pdata = wifiGat_ip();
    if(*pdata==0)
    {
        sprintf(buf1,"0.0.0.0");
    }
    else
    {
        sprintf(buf1,"%s",pdata);
    }
    ShowBar_2(y,"IP",buf1,wordColor,backColor_2,refresh,1);
    
    y+=20;
    pdata = wifiGat_sip();
    if(*pdata==0)
    {
        sprintf(buf1,"0.0.0.0");
    }
    else
    {
        sprintf(buf1,"%s",pdata);
    }
    ShowBar_2(y,"SIP",buf1,wordColor,backColor_2,refresh,1);
    y+=20;
    if(wifiGat_link())
    {
        sprintf(buf1,"已连接");
    }
    else
    {
        sprintf(buf1,"已断开");
    }
    ShowBar_2(y,"连接状态",buf1,wordColor,backColor_2,refresh,0);
        
    y+=20;
    lcdShowSensorInfo(y,wordColor,TitleColor,backColor_2,refresh);
    lcdShowHumiture(220,wordColor,backColor_2,refresh);
}

void lcdPageLora(unsigned char refresh)
{
    u16 y=0;
    u16 wordColor = LCD_COLOR_DARK_PURPLE;
    u16 TitleColor = LCD_COLOR_BLACK;
    u16 backColor_2 = LCD_COLOR_LIGHT_GRAY;
    char buf1[32]={0};
    char buf2[32]={0};
    const char bandwidthList[10][10]={"7.8KHz","10.4KHz","15.6KHz","20.8KHz","31.25KHz",
                                        "41.7KHz","62.58KHz","125KHz","250KHz","500KHz"};
     
    if(refresh&0x80)
    {
        LCDClear(LCD_COLOR_WHITE);                              //清屏
        lcdShowBarX1(0,"智能增强型工业节点",LCD_COLOR_WHITE,LCD_COLOR_PURPLE,refresh,0);
        LCDShowFont16(319-(8*3),2,"K1>",24,LCD_COLOR_GRAY,LCD_COLOR_PURPLE);
    }
    y=20;
    lcdShowBarX1(y,"网络信息",LCD_COLOR_WHITE,TitleColor,refresh,0);
    y+=20;
    lcdShowBarX4(y,"网络类型","LoRa","射频芯片","SX1278",wordColor,backColor_2,refresh,1);
    
    y+=20;
    sprintf(buf1,"%u",loraGet_netID());
    unsigned short temp = loraGet_nodeID();
    sprintf(buf2,"LoRa:%X:%X",temp/256,temp%256);
    lcdShowBarX4(y,"网络ID",buf1,"地址",buf2,wordColor,backColor_2,refresh,1);
    
    y+=20;
    sprintf(buf1,"%u",loraGet_fp());
    sprintf(buf2,"%u",loraGet_pv());
    lcdShowBarX4(y,"基频",buf1,"功率",buf2,wordColor,backColor_2,refresh,1);
    
    y+=20;
    sprintf(buf1,"%u",loraGet_sf());
    sprintf(buf2,"%u",loraGet_cr());
    lcdShowBarX4(y,"扩频因子",buf1,"编码率",buf2,wordColor,backColor_2,refresh,1);
    
    y+=20;
    sprintf(buf1,"%u",loraGet_ps());
    lcdShowBarX4(y,"前导码长",buf1,"带宽",(char*)bandwidthList[loraGet_bw()],wordColor,backColor_2,refresh,1);
    
    y+=20;
    if(loraGet_hop())
    {
        sprintf(buf1,"%s",loraGet_hopTab());
        lcdShowBarX1(y,buf1,wordColor,backColor_2,refresh,0);
    }
    else
    {
        lcdShowBarX1(y,"跳频功能已关闭",wordColor,backColor_2,refresh,0);
    }
    
    y+=20;
    lcdShowSensorInfo(y,wordColor,TitleColor,backColor_2,refresh);
    lcdShowHumiture(220,wordColor,backColor_2,refresh);
}

#define SHOW_BUF_SIZE 28

void lcdPage_NB(uint8_t refresh)
{
    static u16 wordColor = LCD_COLOR_ORANGE;
    static u16 TitleColor = LCD_COLOR_BLACK;
    static u16 backColor_2 = LCD_COLOR_LIGHT_GRAY;
    static uint8_t tick = 0,count = 0,ShowBufFlag = 0;
    static char ShowBuf[SHOW_BUF_SIZE] = {0};
    
    u16 y=0;
    char buf1[32]={0};
    
    if(!ShowBufFlag)
    {
        ShowBufFlag = 1;
        memset(ShowBuf,' ',SHOW_BUF_SIZE);
    }
     
    if(refresh&0x80)
    {
        LCDClear(LCD_COLOR_WHITE);                              //清屏
        lcdShowBarX1(0,"智能增强型工业节点",LCD_COLOR_WHITE,wordColor,refresh,0);
        LCDShowFont16(319-(8*3),2,"K1>",24,LCD_COLOR_GRAY,wordColor);
    }
    y=20;
    lcdShowBarX1(y,"网络信息",LCD_COLOR_WHITE,TitleColor,refresh,0);
    y+=20;
    lcdShowBarX4(y,"网络类型","NB-IoT","射频芯片","WH-NB71",wordColor,backColor_2,refresh,1);
    
    y+=20;
    switch(NB_GetMode())
    {
        case 1:sprintf(buf1,"COAP");
            break;
        case 2:sprintf(buf1,"UDP");
            break;
        default:sprintf(buf1,"未知");
    }
    lcdShowBarX4(y,"网络模式",buf1,"连接状态","-",wordColor,backColor_2,refresh,1);
    
    y+=20;
    ShowBar_2(y,"ID",NB_GetID(),wordColor,backColor_2,refresh,1);
    
    y+=20;
    char* pbuf = NB_GetKEY();
    uint8_t len = strlen(pbuf);
    if(len > SHOW_BUF_SIZE)
    {
        tick++;
        if(tick>1)
        {
            tick = 0;
            count++;
            if(count>len+10)
                count = 0;

            if(count<len)
            {
                RollArray(ShowBuf,SHOW_BUF_SIZE,pbuf[count]);
            }
            else
            {
                RollArray(ShowBuf,SHOW_BUF_SIZE,' ');
            }
        }
        ShowBar_2(y,"KEY",ShowBuf,wordColor,backColor_2,refresh,1);
    }
    else
        ShowBar_2(y,"KEY",pbuf,wordColor,backColor_2,refresh,1);
    
    y+=20;
    ShowBar_2(y,"SIP",NB_GetSIP(),wordColor,backColor_2,refresh,1);
    
    y+=20;
    ShowBar_2(y,"IMEI",NB_GetNodeID(),wordColor,backColor_2,refresh,1);
        
    y+=20;
    lcdShowSensorInfo(y,wordColor,TitleColor,backColor_2,refresh);
    lcdShowHumiture(220,wordColor,backColor_2,refresh);
}

/********************************************************************************************/

void lcdShowPage(unsigned char refresh)
{
    if(lcdPageIndex)
    {
        menuShowHandle(refresh);
    }
    else
    {
        switch(hwType_Get())
        {
            case 0:lcdPageDefault(refresh);
                break;
            case 1:lcdPageZigbee(refresh);
                break;
            case 2:lcdPageBle(refresh);
                break;
            case 3:lcdPageWifi(refresh);
                break;
            case 4:lcdPageLora(refresh);
                break;
            case 5:lcdPage_NB(refresh);
                break;
            default:lcdPageNewRF(refresh);
        }
    }
    lcdSensor_poll();
}

void lcdKeyHandle(unsigned char keyValue)
{
    if(lcdPageIndex)
    {
        menuKeyHandle(keyValue);
    }
    else if(keyValue==0x01)
    {
        lcdPageIndex = 1;
        lcdShowPage(0x80);
    }
}

PROCESS(LcdProcess, "LcdProcess");

PROCESS_THREAD(LcdProcess, ev, data)
{
    static struct etimer lcd_timer,lcd_timeout;
    static short timeoutCount;
    
    PROCESS_BEGIN();
    
    ILI93xxInit();
    LCDClear(LCD_COLOR_BLACK);
    lcdSensor_init();
    
    timeoutCount=10;
    RF_InitEvent = process_alloc_event();
    process_post(&LcdProcess, RF_InitEvent, NULL);
    etimer_set(&lcd_timer,100);
    etimer_set(&lcd_timeout,1000);
    
    while (1)
    {
        PROCESS_WAIT_EVENT();  
        if(ev==RF_InitEvent)
        {   
            SysInit_Status++;
            lcdInitPage(SysInit_Status,timeoutCount);
            if(SysInit_Status == 4)
            {
                SysInit_Status = 0xff;
                lcdShowPage(0x80);
            }
        }
        
        if(ev==key_event)
        {
            if(SysInit_Status == 0xff)
            {
                lcdKeyHandle(*((unsigned char*)data));
            }
        }
        
        if(ev == PROCESS_EVENT_TIMER)
        {
            if(etimer_expired(&lcd_timeout))
            {
                etimer_set(&lcd_timeout,1000);
                if(SysInit_Status<4)
                {
                    if(timeoutCount>0)
                    {
                        lcdInitPage(SysInit_Status,timeoutCount);
                        timeoutCount--;
                    }
                    else
                    {
                        SysInit_Status = 0xff;
                        lcdShowPage(0x80);
                        process_start(&sensor_process,NULL);
                    }
                }
            }
            if(etimer_expired(&lcd_timer))
            {
                etimer_set(&lcd_timer,100);
                if(SysInit_Status == 0xff)
                {
                    lcdShowPage(0x00);
                }
            }
        }
    }
    
    PROCESS_END();
}