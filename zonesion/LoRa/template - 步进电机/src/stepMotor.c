#include "stepMotor.h"

#define SteppingMotor_STEP         GPIO_Pin_0
#define SteppingMotor_DIR          GPIO_Pin_1
#define SteppingMotor_EN 		   GPIO_Pin_3

#define PIN_STEP_port              GPIOB  
#define PIN_DIR_port               GPIOB
#define PIN_EN_port 	           GPIOA
/*********************************************************************************************
* ȫ�ֱ���
*********************************************************************************************/
unsigned long stepNum = 720;                                         //������
char motorRun = 0;
unsigned long pulseNum = 0;                                     //������

/*********************************************************************************************
*���ƣ�step_bit_toggle
*���ܣ����step�ź������ŷ�ת
*��������
*���أ���
*********************************************************************************************/
void step_bit_toggle(unsigned int flag)
{
  if(flag)
    GPIO_ResetBits(PIN_STEP_port, SteppingMotor_STEP);
  else
    GPIO_SetBits(PIN_STEP_port, SteppingMotor_STEP);
}

/*********************************************************************************************
*���ƣ�motor_forward
*���ܣ������ת
*��������
*���أ���
*********************************************************************************************/
void motor_forward()
{
  GPIO_ResetBits(PIN_DIR_port, SteppingMotor_DIR);              //�����ߵ͵�ƽ
  GPIO_SetBits(PIN_EN_port, SteppingMotor_EN);                  //ʹ���߸ߵ�ƽ
}

/*********************************************************************************************
*���ƣ�motor_reverse
*���ܣ������ת
*��������
*���أ���
*********************************************************************************************/
void motor_reverse()
{
  GPIO_SetBits(PIN_DIR_port,SteppingMotor_DIR);                           //�����߸ߵ�ƽ
  GPIO_SetBits(PIN_EN_port, SteppingMotor_EN);                            //ʹ���߸ߵ�ƽ
}

/*********************************************************************************************
*���ƣ�motor_stop
*���ܣ����ֹͣ
*��������
*���أ���
*********************************************************************************************/
void motor_stop()
{
  GPIO_ResetBits(PIN_EN_port, SteppingMotor_EN);                          //ʹ���ߵ͵�ƽ
  GPIO_ResetBits(PIN_DIR_port, SteppingMotor_DIR);                         //�����ߵ͵�ƽ
}

/*********************************************************************************************
*���ƣ�motor_control
*���ܣ���������ۺϿ��ƺ���
*������Cmd
*���أ���
*********************************************************************************************/
void motor_control(unsigned char Cmd)
{
  if(Cmd & 0x01)                                                //����⵽���������
  {
    if(Cmd & 0x02)                                              //����⵽ָ��Ϊ��ת
    {
      motor_reverse();                                          //ִ�е����ת����
      pulseNum = stepNum * 5 / 9 + 4;                                //��ȡ���������������
    }
    else
    {
      motor_forward();                                          //����ִ�е����ת����
      pulseNum = stepNum * 5 / 9 + 4;                                //��ȡ���������������
    }
    motorRun = 1;
    TIM_Cmd(TIM2, ENABLE);
  }
  else
  {
    motor_stop();                                               //����رղ������
    motorRun = 0;
    pulseNum = 0;                                               //����������
  }
}
/*********************************************************************************************
* ���ƣ�SteppingMotorIOInit
* ���ܣ��������IO�ڳ�ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void SteppingMotorIOInit()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	        //ʹ��PA�˿�ʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	        //ʹ��PB�˿�ʱ��
  
  GPIO_InitStructure.GPIO_Pin = SteppingMotor_DIR | SteppingMotor_STEP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = SteppingMotor_EN;
  GPIO_Init(PIN_EN_port, &GPIO_InitStructure);
  GPIO_SetBits(PIN_EN_port, SteppingMotor_EN);
}

/*********************************************************************************************
* ���ƣ�TIM2IntInit
* ���ܣ���ʱ��2�жϳ�ʼ��
* ������arr:�Զ�����װֵ��psc:��Ƶϵ��
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void TIM2IntInit(unsigned short arr,unsigned short psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;             //��ʼ����ʱ���ṹ��
  NVIC_InitTypeDef NVIC_InitStructure;                        //��ʼ��NVIC�ṹ��
  
  //�򿪶�ʱ��2ʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);        //������ʱ��ʱ��
  //��������Զ�װ�صļ���ֵ�����ڼ����Ǵ�0��ʼ�ģ�����Ϊ20000us
  TIM_TimeBaseStructure.TIM_Period = arr;
  // �������Ԥ��Ƶϵ����������Ϊ0ʱ��ʾ����Ƶ����Ҫ��1
  TIM_TimeBaseStructure.TIM_Prescaler = psc;
  // �߼�Ӧ�ñ��β��漰�������ڶ�ʱ��ʱ��(CK_INT)Ƶ���������˲���(ETR,TIx)
  // ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  //���ϼ���
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  //��ʼ����ʱ��2
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  
  // �жϲ�������
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             //����ͨ�ö�ʱ��2�жϿ�����
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //������Ӧ���ȼ�Ϊ0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          //������ռ���ȼ�Ϊ1
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //��ͨ�ö�ʱ��2�ж�
  NVIC_Init(&NVIC_InitStructure);                             //ʹ���ж�
  
  //���TIM2����жϱ�־
  TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
  //������ж�
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM2, DISABLE);
}

/*********************************************************************************************
* ���ƣ�TIM2_IRQHandler
* ���ܣ���ʱ��2�жϷ�����
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void TIM2_IRQHandler(void)
{
  static char edge=0;
  
  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)            //���ж���ӦΪ��
  {
    if(pulseNum>0)
    {
      step_bit_toggle(edge);                                    //��������ź�
      edge = ~edge;                                             //��ƽ���ط�ת
      if(!edge) pulseNum --;                                    //�ڵ͵�ƽʱ��������һ
    }
    else if(motorRun && !pulseNum)
    {
      motorRun = 0;
      TIM_Cmd(TIM2, DISABLE);
    }
  }
  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);                    //��ն�ʱ���жϱ�־λ
}

/*********************************************************************************************
* ���ƣ�SteppingMotorInit
* ���ܣ����������ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void stepMotor_init()
{
  SteppingMotorIOInit();
  TIM2IntInit(499,167);
}

