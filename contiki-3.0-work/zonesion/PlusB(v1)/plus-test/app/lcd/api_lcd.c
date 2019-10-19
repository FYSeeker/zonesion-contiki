/*********************************************************************************************
* �ļ���api_lcd.c
* ���ߣ�fuyou 2018.11.1
* ˵����lcd�봫�������޸Ĳ���
* �޸ģ�
* ע�ͣ�
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

//��ʾ��Ŀ-4
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
        lcdShowBarX1(y,"���Խ��",LCD_COLOR_WHITE,LCD_COLOR_BLACK,refresh,0);
    
    if(testFlag_key1)
        sprintf(pbuf1,"K1:����");
    else
        sprintf(pbuf1,"K1:    ");
    if(testFlag_key2)
        sprintf(pbuf2,"K2:����");
    else
        sprintf(pbuf2,"K2:    ");
    if(testFlag_key3)
        sprintf(pbuf3,"K3:����");
    else
        sprintf(pbuf3,"K3:    ");
    if(testFlag_key4)
        sprintf(pbuf4,"K4:����");
    else
        sprintf(pbuf4,"K4:    ");
    lcdShowOF4(y+=20,pbuf1,pbuf2,pbuf3,pbuf4,LCD_COLOR_RED,LCD_COLOR_YELLOW,refresh,1);
    
    if(testFlag_uart1&0x02)
        sprintf(pbuf1," RF���ڴ��� ");
    else if(testFlag_uart1&0x01)
        sprintf(pbuf1," RF�������� ");
    else
        sprintf(pbuf1,"RF���ڲ�����");
    
    if(testFlag_flash&0x02)
        sprintf(pbuf2," flash���� ");
    else if(testFlag_flash&0x01)
        sprintf(pbuf2," flash���� ");
    else
        sprintf(pbuf2,"flash������");
    
    if(testFlag_ethernet&0x02)
        sprintf(pbuf3," ��̫������ ");
    else if(testFlag_ethernet&0x01)
        sprintf(pbuf3," ��̫������ ");
    else
        sprintf(pbuf3,"��̫��������");
    lcdShowBarX3(y+=20,pbuf1,pbuf2,pbuf3,LCD_COLOR_RED,LCD_COLOR_YELLOW,refresh,1);
}


/*********************************************************************************************
* ���ƣ�option_1_Handle()
* ���ܣ��˵���1�Ĵ�����
* ������status:�˵����ѡ��״̬
* ���أ���
* �޸ģ�
* ע�ͣ��Զ���˵���1��ѡ������еĲ���
*********************************************************************************************/
void option_1_Handle(optionStatus_t status)
{
    menuExitHandle();
}

/*********************************************************************************************
* ���ƣ�option_2_Handle()
* ���ܣ��˵���2�Ĵ�����
* ������status:�˵����ѡ��״̬
* ���أ���
* �޸ģ�
* ע�ͣ��Զ���˵���2��ѡ������еĲ���
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
* ���ƣ�option_3_Handle()
* ���ܣ��˵���3�Ĵ�����
* ������status:�˵����ѡ��״̬
* ���أ���
* �޸ģ�
* ע�ͣ��Զ���˵���3��ѡ������еĲ���
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
* ���ƣ�MenuOption_init()
* ���ܣ��˵����ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ��Զ���˵�������ݺ͹���
*********************************************************************************************/
void MenuOption_init()
{
    //�˵���1�ĳ�ʼ��
    optionDefaultName_set(1," 1-����������");     //�˵���Ĭ������
    optionSelectName_set(1," 1-����������");      //�˵��ѡ���������
    optionHintInfo_set(1," ");                  //�˵��ѡ�������ʾ��Ϣ���ո����ʲô������ʾ
    optionCallFunc_set(1,option_1_Handle);      //�˵��ѡ����Ĵ�����
    
    //�˵���2�ĳ�ʼ��
    optionDefaultName_set(2," 2-�򿪼̵���һ");   //�˵���Ĭ������
    optionSelectName_set(2," 2-�رռ̵���һ");    //�˵��ѡ���������
    optionHintInfo_set(2,"�̵���һ�Ѵ�");        //�˵��ѡ�������ʾ��Ϣ
    optionCallFunc_set(2,option_2_Handle);      //�˵��ѡ����Ĵ�����
    
    //�˵���3�ĳ�ʼ��
    optionDefaultName_set(3," 3-�򿪼̵�����");
    optionSelectName_set(3," 3-�رռ̵�����");
    optionHintInfo_set(3,"�̵������Ѵ�");
    optionCallFunc_set(3,option_3_Handle);
}

/*********************************************************************************************
* ���ƣ�TableInfo_init()
* ���ܣ������Ϣ��ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ��Զ�������Ҫ��ʾ����Ϣ
*********************************************************************************************/
void TableInfo_init()
{
    tableExplain_set(1,"���߽ӿ�");
    tableExplain_set(2,"-");
    tableExplain_set(3,"�̵���һ");
    tableExplain_set(4,"�̵�����");
    tableExplain_set(5,"ʾ��������");
}

/*********************************************************************************************
* ���ƣ�lcdSensor_init()
* ���ܣ�lcd�봫�������ֳ�ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void lcdSensor_init()
{
    MenuOption_init();
    TableInfo_init();
    SensorName_set("�̵���һ","�̵�����","ʾ��������");
}

/*********************************************************************************************
* ���ƣ�syncSensorStatus()
* ���ܣ�ͬ�������ഫ����״̬
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ������ഫ�������ϲ����ú���Ҫ���д˺���ͬ����Ļ�ϵ���ʾ״̬
*********************************************************************************************/
void syncSensorStatus()
{
    if(D1&0x40)
        optionState_set(2,SELECT);      //����ѡ��2Ϊѡ��״̬
    else
        optionState_set(2,UNSELECT);    //����ѡ��2Ϊȡ��ѡ��״̬
    
    if(D1&0x80)
        optionState_set(3,SELECT);      //����ѡ��3Ϊѡ��״̬
    else
        optionState_set(3,UNSELECT);    //����ѡ��3Ϊȡ��ѡ��״̬
}

/*********************************************************************************************
* ���ƣ�lcdSensor_poll()
* ���ܣ�lcd�봫����������ѯ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ���ѯ���ڴ��������ݵĸ���
*********************************************************************************************/
void lcdSensor_poll()
{
    char pbuf1[16]={0};
    char pbuf2[16]={0};
    char pbuf3[16]={0};
    
    //��������/״̬
    if(D1&0x40)
        sprintf(pbuf1,"��");
    else
        sprintf(pbuf1,"�ر�");
    
    if(D1&0x80)
        sprintf(pbuf2,"��");
    else
        sprintf(pbuf2,"�ر�");
    
    sprintf(pbuf3,"%3u",A0);
    
    //����ҳ��1�Ĵ���������
    SensorData_set(pbuf1,pbuf2,pbuf3);
    //����ҳ��2�ı������
    tableData_set(1,"-");
    tableData_set(2,"-");
    tableData_set(3,pbuf1);
    tableData_set(4,pbuf2);
    tableData_set(5,pbuf3);
    //ͬ�������ഫ������ʾ״̬
    syncSensorStatus();
}

