#include "signalLamp.h"
#include "usart.h"

char lampOffBuf[5] = {0xFB,0x00,0xFF,0x02,0x01};                // �ر������źŵ�
uint8_t intervalTime = 0;                                       // ��˸���ʱ��

/*********************************************************************************************
* ���ƣ�uart2_set_input_call()
* ���ܣ����ô���2�ص�����
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void uart2_set_input_call(int (*c)(char ch))
{
  uart2_set_input(c);
}

/*********************************************************************************************
* ���ƣ�uart2_send_data()
* ���ܣ�����2��������
* ������buf-��Ҫ���͵�����/len-���ݳ���
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void uart2_send_data(char* buf, unsigned int len)
{
  for(unsigned int i=0; i<len; i++)
    uart2_putc(buf[i]);
}

/*********************************************************************************************
* ���ƣ�set_intervalTime()
* ���ܣ�������˸���ʱ��
* ������time-ʱ����
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void set_intervalTime(uint8_t time)
{
  if(time >= 255)
    time = 255;
  intervalTime = time;
}

/*********************************************************************************************
* ���ƣ�signalLamp_init()
* ���ܣ��źŵƳ�ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void signalLamp_init(void)
{
  uart2_init(9600);
  uart2_set_input_call(uart2_rec_handle);
}

/*********************************************************************************************
* ���ƣ�up_signalLamp()
* ���ܣ��źŵ�״̬����
* ������cmd-�źŵ�״̬
* ���أ���
* �޸ģ�
* ע�ͣ�
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
* ���ƣ�uart2_rec_handle()
* ���ܣ�����2�ص�����
* ������ch-���ڽ��յ�����
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int uart2_rec_handle(char ch)
{
  return 1;
}




