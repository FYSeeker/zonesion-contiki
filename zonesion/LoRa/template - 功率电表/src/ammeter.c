#include "ammeter.h"
#include "usart.h"

char readAmmeterBuf[8] = {0x01,0x03,0x00,0x48,0x00,0x05,0x05,0xDF};//获取电表数据
float electricQuantity = 0;                                     // 用电量
float voltage = 0;                                              // 电压
float electricity = 0;                                          // 电流
float power = 0;                                                // 功率

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
* 名称：air_init()
* 功能：空气质量传感器初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void ammeter_init(void)
{
  uart2_init(9600);
  uart2_set_input_call(uart2_rec_handle);
}

void read_ammeter(void)
{
  uart2_send_data(readAmmeterBuf, sizeof(readAmmeterBuf));
}

float get_electricQuantity(void)
{
  return electricQuantity;
}

float get_voltage(void)
{
  return voltage;
}

float get_electricity(void)
{
  return electricity;
}

float get_power(void)
{
  return power;
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
  static uint8_t flag = 0;
  static char recBuf[BUFSIZE];
  if((ch == 0x01) && (flag == 0)){
    flag = 1;
  }
  else if((ch == 0x03) && (flag == 1)){
    flag = 2;
  }
  else if((ch == 0x0A) && (flag == 2)){
    flag = 3;
  }
  if((flag == 3)){
    recBuf[recLen++] = ch;
    if(recLen >= 12)
    {
      flag = 0;
      electricQuantity = ((recBuf[9]*256+recBuf[8])*65536+(recBuf[7]*256+recBuf[6]))/3200.0f;
      voltage = (recBuf[1]*256+recBuf[0])/100.0f;
      electricity = (recBuf[3]*256+recBuf[2])/1000.0f;
      power = (recBuf[5]*256+recBuf[4])/10.0f;
      recLen = 0;
    }
  }
  return 1;
}



