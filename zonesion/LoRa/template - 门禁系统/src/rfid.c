#include <string.h>
#include "rfid.h"
#include "usart.h"
#include "delay.h"
#include "relay.h"

char readRFIDBuf[7] = {0x09,0x41,0x31,0x46,0x33,0x46,0x0D};     // 读卡指令
uint16_t readTime = 1000;                                       // 读取卡号间隔时间(ms)
uint8_t closeDoorTime = 3;                                      // 关门间隔时间(S)
uint8_t maxLegalSum = 2;                                        // 允许储存最大卡号数
uint8_t legalSum = 0;                                           // 已经保存的卡号个数
char legalID[64];                                               // 卡号存储区
uint8_t doorStatus = 0;                                         // 门状态
char ICCard[9] = {0};                                           // 卡号

/*********************************************************************************************
* 名称：uart2_set_input_call()
* 功能：设置串口2回调函数
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void uart2_set_input_call(int (*c)(char ch))
{
  uart2_set_input(c);
}

/*********************************************************************************************
* 名称：uart2_send_data()
* 功能：串口2发送数据
* 参数：buf-需要发送的数据/len-数据长度
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void uart2_send_data(char* buf, unsigned int len)
{
  for(unsigned int i=0; i<len; i++)
  {
    uart2_putc(buf[i]);
    delay_ms(4);
  }
}

/*********************************************************************************************
* 名称：air_init()
* 功能：空气质量传感器初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void rfid_init(void)
{
  uart2_init(19200);
  uart2_set_input_call(uart2_rec_handle);
}

/*********************************************************************************************
* 名称：read_rfid()
* 功能：读取传感器数据
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void read_rfid(void)
{
  uart2_send_data(readRFIDBuf, sizeof(readRFIDBuf));
}

/*********************************************************************************************
* 名称：get_doorStatus()
* 功能：获取当前门的状态
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
uint8_t get_doorStatus(void)
{
  return doorStatus;
}

/*********************************************************************************************
* 名称：set_doorStatus()
* 功能：设置当前门的状态
* 参数：当前门的状态
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void set_doorStatus(uint8_t status)
{
  doorStatus = status;
}

/*********************************************************************************************
* 名称：get_closeDoorTime()
* 功能：获取当前关门时间间隔
* 参数：无
* 返回：关门时间间隔
* 修改：
* 注释：
*********************************************************************************************/
uint8_t get_closeDoorTime(void)
{
  return closeDoorTime;
}

/*********************************************************************************************
* 名称：get_maxLegalSum()
* 功能：获取最大储存卡号数量
* 参数：无
* 返回：最大储存卡号数量
* 修改：
* 注释：
*********************************************************************************************/
uint8_t get_maxLegalSum(void)
{
  return maxLegalSum;
}

/*********************************************************************************************
* 名称：set_maxLegalSum()
* 功能：设置最大储存卡号数量
* 参数：最大储存卡号数量
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void set_maxLegalSum(uint8_t sum)
{
  if(sum >= 8)
    sum = 8;
  maxLegalSum = sum;
}

/*********************************************************************************************
* 名称：get_readTime()
* 功能：获取当前读取时间间隔
* 参数：无
* 返回：读取时间间隔
* 修改：
* 注释：
*********************************************************************************************/
uint16_t get_readTime(void)
{
  return readTime;
}
/*********************************************************************************************
* 名称：uart2_rec_handle()
* 功能：串口2回调函数
* 参数：ch-串口接收的数据
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
int uart2_rec_handle(char ch)
{
#define BUFSIZE 128
  static unsigned int recLen = 0;
  static char recBuf[BUFSIZE];
  static unsigned char flag = 0;
  if(ch == 0x0A && flag == 0)
    flag = 1;
  else if(ch == 0x41 && flag == 1)
    flag = 2;
  else if(ch == 0x31 && flag == 2)
    flag = 3;
  else if(ch == 0x46 && flag == 3)
    flag = 4;
  else if(ch == 0x30 && flag == 4)
    flag = 5;
  else if(flag == 5)
  {
    recBuf[recLen++] = ch;
    if(recLen >= 16)
    {
      for(int x=0; x<=8; x+=2)
      {
        ICCard[x] = recBuf[6-x];
        ICCard[x+1] = recBuf[7-x];
      }
      for(int y=0; y<8; y++)
      {
        recBuf[y] = ICCard[y];
      }
      ICCard[8] = '\0';
      char* result = NULL;
      result = strstr(legalID, ICCard);
      if(legalSum < maxLegalSum)
      {
        if(result == NULL)
        {
          strcpy(&legalID[legalSum*8], ICCard);
          legalSum++;
        }
      }
      if(result > 0)
      {
        doorStatus |= 0x01;
        relay_control(doorStatus);
      }
      recLen = 0;
      flag = 0;
    }
  }
  else
  {
    flag = 0;
    recLen = 0;
  }
  return 1;
}




