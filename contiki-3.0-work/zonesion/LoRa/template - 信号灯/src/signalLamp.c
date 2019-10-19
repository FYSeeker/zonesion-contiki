#include "signalLamp.h"
#include "usart.h"

char lampOffBuf[5] = {0xFB,0x00,0xFF,0x02,0x01};                // 关闭所有信号灯
uint8_t intervalTime = 0;                                       // 闪烁间隔时间

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
    uart2_putc(buf[i]);
}

/*********************************************************************************************
* 名称：set_intervalTime()
* 功能：设置闪烁间隔时间
* 参数：time-时间间隔
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void set_intervalTime(uint8_t time)
{
  if(time >= 255)
    time = 255;
  intervalTime = time;
}

/*********************************************************************************************
* 名称：signalLamp_init()
* 功能：信号灯初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void signalLamp_init(void)
{
  uart2_init(9600);
  uart2_set_input_call(uart2_rec_handle);
}

/*********************************************************************************************
* 名称：up_signalLamp()
* 功能：信号灯状态更新
* 参数：cmd-信号灯状态
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void up_signalLamp(uint8_t cmd)
{
  char lampCmdBuf[5] = {0xFB,0x00,0xFF,0x01,0x01};
  static uint8_t time = 0;
  static uint8_t offFlag = 0;
  static uint8_t lastStatus = 0;
  if(intervalTime > 0)
  {
    time++;
    if(time >= intervalTime*2)
    {
      if(offFlag)
      {
        lampCmdBuf[2] = cmd;
        lampCmdBuf[4] = ((lampCmdBuf[1] + lampCmdBuf[2] + lampCmdBuf[3]) & 0xFF);
        uart2_send_data(lampCmdBuf, sizeof(lampCmdBuf));
      }
      else
        uart2_send_data(lampOffBuf, sizeof(lampOffBuf));
      offFlag = !offFlag;
      time = 0;
    }
  }
  else
  {
    if(lastStatus != cmd)
    {
      if(offFlag)
      {
        lampCmdBuf[2] = cmd;
        lampCmdBuf[4] = ((lampCmdBuf[1] + lampCmdBuf[2] + lampCmdBuf[3]) & 0xFF);
        uart2_send_data(lampCmdBuf, sizeof(lampCmdBuf));
        lastStatus = cmd;
      }
      else
        uart2_send_data(lampOffBuf, sizeof(lampOffBuf));
      offFlag = !offFlag;
    }
    time = 0;
  }
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
  return 1;
}




