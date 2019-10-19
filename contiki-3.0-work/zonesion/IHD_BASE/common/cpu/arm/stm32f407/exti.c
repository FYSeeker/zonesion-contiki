#include "exti.h"

void exti9_5_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʹ��GPIOAʱ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //��Ӧ����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//ʹ��SYSCFGʱ��
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);//PA7 ���ӵ��ж���7
  
	/* ����EXTI_Line7 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line7;//LINE
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; //�½��ش��� 
  //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//ʹ��LINE
	EXTI_Init(&EXTI_InitStructure);//����
  
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�ⲿ�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);//����
}


void (*EXTI9_5_CALL[5])(void)={NULL};

uint8_t EXTI9_5_CALL_Set(uint8_t index,void (*func)(void))
{
  if(index>5) return 1;
  EXTI9_5_CALL[index-1]=func;
  return 1;
}

//�ⲿ�жϷ������
void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line5)!=RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line5);//����жϱ�־λ  
    
    if(EXTI9_5_CALL[0] != NULL)
      EXTI9_5_CALL[0]();
  }
  if(EXTI_GetITStatus(EXTI_Line6)!=RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line6);//����жϱ�־λ  
    
    if(EXTI9_5_CALL[1] != NULL)
      EXTI9_5_CALL[1]();
  }
  if(EXTI_GetITStatus(EXTI_Line7)!=RESET)
  {
    
    if(EXTI9_5_CALL[2] != NULL)
      EXTI9_5_CALL[2]();
    EXTI_ClearITPendingBit(EXTI_Line7);//����жϱ�־λ  
  }
  if(EXTI_GetITStatus(EXTI_Line8)!=RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line8);//����жϱ�־λ  
    
    if(EXTI9_5_CALL[3] != NULL)
      EXTI9_5_CALL[3]();
  }
  if(EXTI_GetITStatus(EXTI_Line9)!=RESET)
  {
    EXTI_ClearITPendingBit(EXTI_Line9);//����жϱ�־λ  
    
    if(EXTI9_5_CALL[4] != NULL)
      EXTI9_5_CALL[4]();
  }
}




