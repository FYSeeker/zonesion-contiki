/*********************************************************************************************
* 文件：api_lcd.c
* 作者：fuyou 2018.11.1
* 说明：lcd与传感器的修改部分
* 修改：
* 注释：
*********************************************************************************************/
#include "api_lcd.h"


extern char A0;
extern char D1;
extern void sensor_control(unsigned char cmd);

/*test file*/

extern uint8_t testFlag_key1;
extern uint8_t testFlag_key2;
extern uint8_t testFlag_key3;
extern uint8_t testFlag_key4;
extern uint8_t testFlag_uart1;
extern uint8_t testFlag_flash;
extern uint8_t testFlag_ethernet;

//显示条目-4
void lcdShowOF4(unsigned short y,char* buf1,char* buf2,char* buf3,char* buf4,
                  unsigned short wordColor,unsigned short backColor,unsigned char refresh,unsigned char mode)
{
    if(refresh&0x80)
    {
        LCD_File(0,y,319,y+19,backColor);
        LCD_DrawLine(160,y,160,y+19,LCD_COLOR_WHITE);
        LCD_DrawLine(80,y,80,y+19,LCD_COLOR_WHITE);
        LCD_DrawLine(240,y,240,y+19,LCD_COLOR_WHITE);
        if(mode)
        {
            LCD_DrawLine(0,y+19,319,y+19,LCD_COLOR_WHITE);
        }
    }
    LCDShowFont16(40-(strlen(buf1)*4),y+2,buf1,strlen(buf1)*8,wordColor,backColor);
    LCDShowFont16(120-(strlen(buf2)*4),y+2,buf2,strlen(buf2)*8,wordColor,backColor);
    LCDShowFont16(200-(strlen(buf3)*4),y+2,buf3,strlen(buf3)*8,wordColor,backColor);
    LCDShowFont16(280-(strlen(buf4)*4),y+2,buf4,strlen(buf4)*8,wordColor,backColor);
}

void test_showInfo(unsigned short y,unsigned char refresh)
{
    char pbuf1[16]={0};
    char pbuf2[16]={0};
    char pbuf3[16]={0};
    char pbuf4[16]={0};
    
    if(refresh&0x80)
        lcdShowBarX1(y,"测试结果",LCD_COLOR_WHITE,LCD_COLOR_BLACK,refresh,0);
    
    if(testFlag_key1)
        sprintf(pbuf1,"K1:正常");
    else
        sprintf(pbuf1,"K1:    ");
    if(testFlag_key2)
        sprintf(pbuf2,"K2:正常");
    else
        sprintf(pbuf2,"K2:    ");
    if(testFlag_key3)
        sprintf(pbuf3,"K3:正常");
    else
        sprintf(pbuf3,"K3:    ");
    if(testFlag_key4)
        sprintf(pbuf4,"K4:正常");
    else
        sprintf(pbuf4,"K4:    ");
    lcdShowOF4(y+=20,pbuf1,pbuf2,pbuf3,pbuf4,LCD_COLOR_RED,LCD_COLOR_YELLOW,refresh,1);
    
    if(testFlag_uart1&0x02)
        sprintf(pbuf1," RF串口错误 ");
    else if(testFlag_uart1&0x01)
        sprintf(pbuf1," RF串口正常 ");
    else
        sprintf(pbuf1,"RF串口测试中");
    
    if(testFlag_flash&0x02)
        sprintf(pbuf2," flash错误 ");
    else if(testFlag_flash&0x01)
        sprintf(pbuf2," flash正常 ");
    else
        sprintf(pbuf2,"flash测试中");
    
    if(testFlag_ethernet&0x02)
        sprintf(pbuf3," 以太网错误 ");
    else if(testFlag_ethernet&0x01)
        sprintf(pbuf3," 以太网正常 ");
    else
        sprintf(pbuf3,"以太网测试中");
    lcdShowBarX3(y+=20,pbuf1,pbuf2,pbuf3,LCD_COLOR_RED,LCD_COLOR_YELLOW,refresh,1);
}


/*********************************************************************************************
* 名称：option_1_Handle()
* 功能：菜单项1的处理函数
* 参数：status:菜单项的选定状态
* 返回：无
* 修改：
* 注释：自定义菜单项1被选定后进行的操作
*********************************************************************************************/
void option_1_Handle(optionStatus_t status)
{
    menuExitHandle();
}

/*********************************************************************************************
* 名称：option_2_Handle()
* 功能：菜单项2的处理函数
* 参数：status:菜单项的选定状态
* 返回：无
* 修改：
* 注释：自定义菜单项2被选定后进行的操作
*********************************************************************************************/
void option_2_Handle(optionStatus_t status)
{
    if(status==SELECT)
    {
        D1 |= 0x40;
    }
    else if(status==UNSELECT)
    {
        D1 &= ~(0x40);
    }
    sensor_control(D1);
}

/*********************************************************************************************
* 名称：option_3_Handle()
* 功能：菜单项3的处理函数
* 参数：status:菜单项的选定状态
* 返回：无
* 修改：
* 注释：自定义菜单项3被选定后进行的操作
*********************************************************************************************/
void option_3_Handle(optionStatus_t status)
{
    if(status==SELECT)
    {
        D1 |= 0x80;
    }
    else if(status==UNSELECT)
    {
        D1 &= ~(0x80);
    }
    sensor_control(D1);
}

/*********************************************************************************************
* 名称：MenuOption_init()
* 功能：菜单项初始化
* 参数：无
* 返回：无
* 修改：
* 注释：自定义菜单项的内容和功能
*********************************************************************************************/
void MenuOption_init()
{
    //菜单项1的初始化
    optionDefaultName_set(1," 1-返回主界面");     //菜单项默认名称
    optionSelectName_set(1," 1-返回主界面");      //菜单项被选定后的名称
    optionHintInfo_set(1," ");                  //菜单项被选定后的提示信息，空格就是什么都不显示
    optionCallFunc_set(1,option_1_Handle);      //菜单项被选定后的处理函数
    
    //菜单项2的初始化
    optionDefaultName_set(2," 2-打开继电器一");   //菜单项默认名称
    optionSelectName_set(2," 2-关闭继电器一");    //菜单项被选定后的名称
    optionHintInfo_set(2,"继电器一已打开");        //菜单项被选定后的提示信息
    optionCallFunc_set(2,option_2_Handle);      //菜单项被选定后的处理函数
    
    //菜单项3的初始化
    optionDefaultName_set(3," 3-打开继电器二");
    optionSelectName_set(3," 3-关闭继电器二");
    optionHintInfo_set(3,"继电器二已打开");
    optionCallFunc_set(3,option_3_Handle);
}

/*********************************************************************************************
* 名称：TableInfo_init()
* 功能：表格信息初始化
* 参数：无
* 返回：无
* 修改：
* 注释：自定义表格需要显示的信息
*********************************************************************************************/
void TableInfo_init()
{
    tableExplain_set(1,"网线接口");
    tableExplain_set(2,"-");
    tableExplain_set(3,"继电器一");
    tableExplain_set(4,"继电器二");
    tableExplain_set(5,"示例传感器");
}

/*********************************************************************************************
* 名称：lcdSensor_init()
* 功能：lcd与传感器部分初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void lcdSensor_init()
{
    MenuOption_init();
    TableInfo_init();
    SensorName_set("继电器一","继电器二","示例传感器");
}

/*********************************************************************************************
* 名称：syncSensorStatus()
* 功能：同步控制类传感器状态
* 参数：无
* 返回：无
* 修改：
* 注释：控制类传感器被上层设置后，需要运行此函数同步屏幕上的显示状态
*********************************************************************************************/
void syncSensorStatus()
{
    if(D1&0x40)
        optionState_set(2,SELECT);      //设置选项2为选择状态
    else
        optionState_set(2,UNSELECT);    //设置选项2为取消选择状态
    
    if(D1&0x80)
        optionState_set(3,SELECT);      //设置选项3为选择状态
    else
        optionState_set(3,UNSELECT);    //设置选项3为取消选择状态
}

/*********************************************************************************************
* 名称：lcdSensor_poll()
* 功能：lcd与传感器部分轮询
* 参数：无
* 返回：无
* 修改：
* 注释：轮询用于传感器数据的更新
*********************************************************************************************/
void lcdSensor_poll()
{
    char pbuf1[16]={0};
    char pbuf2[16]={0};
    char pbuf3[16]={0};
    
    //更新数据/状态
    if(D1&0x40)
        sprintf(pbuf1,"打开");
    else
        sprintf(pbuf1,"关闭");
    
    if(D1&0x80)
        sprintf(pbuf2,"打开");
    else
        sprintf(pbuf2,"关闭");
    
    sprintf(pbuf3,"%3u",A0);
    
    //更新页面1的传感器数据
    SensorData_set(pbuf1,pbuf2,pbuf3);
    //更新页面2的表格数据
    tableData_set(1,"-");
    tableData_set(2,"-");
    tableData_set(3,pbuf1);
    tableData_set(4,pbuf2);
    tableData_set(5,pbuf3);
    //同步控制类传感器显示状态
    syncSensorStatus();
}

