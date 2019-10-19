#include "ammeter.h"
#include "usart.h"

char readAmmeterBuf[8] = {0x01,0x03,0x00,0x48,0x00,0x05,0x05,0xDF};//��ȡ�������
float electricQuantity = 0;                                     // �õ���
float voltage = 0;                                              // ��ѹ
float electricity = 0;                                          // ����
float power = 0;                                                // ����

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
* ���ƣ�air_init()
* ���ܣ�����������������ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
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
* ���ƣ�uart2_rec_handle()
* ���ܣ�����2�ص�����
* ������ch-���ڽ��յ�����
* ���أ���
* �޸ģ�
* ע�ͣ�
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



