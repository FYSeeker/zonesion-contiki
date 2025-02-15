#include "Air.h"
#include "usart.h"

char readAirBuf[5] = {0x11,0x02,0x01,0x00,0xEC};                // 获取当前传感器的值
char co2CorrectBuf[6] = {0x11,0x03,0x03,0x01,0x90,0x58};        // CO2零点校准
char pm25OpenBuf[6] = {0x11,0x03,0x0C,0x02,0x1E,0xC0};          // 开启粉尘测量
char pm25StopBuf[6] = {0x11,0x03,0x0C,0x01,0x1E,0xC1};          // 停止粉尘测量
uint16_t co2 = 0;                                               // 二氧化碳
uint8_t voc = 0;                                                // VOC等级
float humi = 0;                                                 // 湿度
float temp = 0;                                                 // 温度
uint16_t pm25 = 0;                                              // PM2.5
uint8_t pm25OpenFlag = 1;                                       // PM2.5检测开关

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
* 名称：get_co2()
* 功能：获取CO2数据
* 参数：无
* 返回：CO2数据
* 修改：
* 注释：
*********************************************************************************************/
unsigned int get_co2(void)
{
  return co2;
}

/*********************************************************************************************
* 名称：get_voc()
* 功能：获取VOC数据
* 参数：无
* 返回：VOC等级
* 修改：
* 注释：
*********************************************************************************************/
unsigned char get_voc(void)
{
  return voc;
}

/*********************************************************************************************
* 名称：get_humi()
* 功能：获取湿度数据
* 参数：无
* 返回：湿度数据
* 修改：
* 注释：
*********************************************************************************************/
float get_humi(void)
{
  return humi;
}

/*********************************************************************************************
* 名称：get_temp()
* 功能：获取温度数据
* 参数：无
* 返回：温度数据
* 修改：
* 注释：
*********************************************************************************************/
float get_temp(void)
{
  return temp;
}

/*********************************************************************************************
* 名称：get_pm25()
* 功能：获取PM2.5数据
* 参数：无
* 返回：PM2.5数据
* 修改：
* 注释：
*********************************************************************************************/
unsigned int get_pm25(void)
{
  return pm25;
}

/*********************************************************************************************
* 名称：get_pm25OpenFlag()
* 功能：获取PM2.5检测开关
* 参数：无
* 返回：PM2.5是否检测
* 修改：
* 注释：
*********************************************************************************************/
unsigned char get_pm25OpenFlag(void)
{
  return pm25OpenFlag;
}

/*********************************************************************************************
* 名称：air_init()
* 功能：空气质量传感器初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void air_init(void)
{
  uart2_init(9600);
  uart2_set_input_call(uart2_rec_handle);
}

/*********************************************************************************************
* 名称：read_air()
* 功能：读取传感器数据
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void read_air(void)
{
  uart2_send_data(readAirBuf, sizeof(readAirBuf));
}

/*********************************************************************************************
* 名称：co2_correct()
* 功能：CO2校准
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void co2_correct(void)
{
  uart2_send_data(co2CorrectBuf, sizeof(co2CorrectBuf));
}

/*********************************************************************************************
* 名称：open_pm25()
* 功能：开启PM2.5测量数据
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void open_pm25(void)
{
  uart2_send_data(pm25OpenBuf, sizeof(pm25OpenBuf));
}

/*********************************************************************************************
* 名称：stop_pm25()
* 功能：停止PM2.5测量数据
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void stop_pm25(void)
{
  uart2_send_data(pm25StopBuf, sizeof(pm25StopBuf));
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
  static unsigned char dataLen = 0;
  static unsigned char dataMode = 0xFF;
  if((ch == 0x16) && (flag == 0x00))
    flag = 0x01;
  else if(flag == 0x01)
  {
    dataLen = ch;
    flag = 0x02;
  }
  else if(flag == 0x02)
  {
    dataMode = ch;
    flag = 0x03;
  }
  else if(flag == 0x03)
  {
    recBuf[recLen++] = ch;
    if(recLen >= dataLen)
    {
      flag = 0x00;
      switch (dataMode)
      {
      case 0x01:
        co2 = recBuf[0]*256 + recBuf[1];
        voc = recBuf[2]*256 + recBuf[3];
        humi = (recBuf[4]*256 + recBuf[5])*0.1;
        temp = (recBuf[6]*256 + recBuf[7]-500)*0.1;
        pm25 = recBuf[8]*256 + recBuf[9];
        break;
      case 0x03:
        break;
      case 0x0C:
        if(recBuf[0] == 0x01)
          pm25OpenFlag = 0;
        else
          pm25OpenFlag = 1;
        break;
      default:
        recLen = 0;
        break;
      }
      recLen = 0;
    }
  }
  return 1;
}






