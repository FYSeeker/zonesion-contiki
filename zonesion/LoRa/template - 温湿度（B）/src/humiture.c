#include "humiture.h"
#include "usart.h"

char readHumitureBuf[8] = {0x01,0x03,0x00,0x00,0x00,0x02,0xC4,0x0B};
float temp = 0.0;
float humi = 0.0;

/*********************************************************************************************
* 名称：calc_crc()
* 功能：crc校验
* 参数：pbuf -- 输入参数，校验指针
*       recLen -- 输入参数，校验数组长度
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
static unsigned int calc_crc(unsigned char *pbuf, unsigned char recLen)
{
  unsigned int crc = 0xffff;
  unsigned char k, i;
  for (k = 0; k < recLen; k++) 
  {
    crc = crc ^ pbuf[k];
    for(i = 0; i < 8; i++) 
    {
	  unsigned char tmp;
	  tmp = crc&1;
	  crc = crc>>1;
	  crc &= 0x7fff;
	  if (tmp == 1)
        crc ^= 0xa001;
	  crc &= 0xffff;
    }
  }
  return crc;
}

/*********************************************************************************************
* 名称：get_temp()
* 功能：返回传感器温度数据
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
float get_temp(void)
{
  return temp;
}

/*********************************************************************************************
* 名称：get_humi()
* 功能：返回传感器湿度数据
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
float get_humi(void)
{
  return humi;
}

/*********************************************************************************************
* 名称：humiture_init()
* 功能：温湿度传感器初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void humiture_init(void)
{
  uart2_init(9600);
  uart2_set_input_call(uart2_rec_handle);
}

/*********************************************************************************************
* 名称：uart2_send_data()
* 功能：串口2发送数据
* 参数：buf-需要发送的数据/recLen-数据长度
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void uart2_send_data(char* buf, unsigned int recLen)
{
  for(unsigned int i=0; i<recLen; i++)
  {
    uart2_putc(buf[i]);
  }
}

/*********************************************************************************************
* 名称：read_humiture()
* 功能：读取传感器数据
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void read_humiture(void)
{
  uart2_send_data(readHumitureBuf, sizeof(readHumitureBuf));
}

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
  if (recLen > 0) 
  {
    recBuf[recLen++] = ch;
    if (recLen == 9) 
    {
      uint16_t crc;
      crc = calc_crc((unsigned char*)recBuf, 7);
      if (crc == ((recBuf[8]<<8) | recBuf[7])) 
      {
        int nTemp, nHumi;
        nHumi = (recBuf[3]<<8) | recBuf[4];
        nTemp = (recBuf[5]<<8) | recBuf[6];
        
        humi = (float)nHumi/10.0f;
        temp = (float)nTemp/10.0f;
      }
      recLen = 0;
    }
  } 
  else
  {
    if (recLen == 0 && ch == 0x01)
    {
      recBuf[recLen++] = ch;
    }
    else 
    {
      recLen = 0;
    }
  }
  return 1;
}
