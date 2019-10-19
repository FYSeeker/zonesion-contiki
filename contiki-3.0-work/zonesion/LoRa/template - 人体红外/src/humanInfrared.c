#include "humanInfrared.h"

#define INFRARED_GPIO                GPIOA
#define INFRARED_CLK                 RCC_APB2Periph_GPIOA
#define INFRARED_PIN                 GPIO_Pin_7
#define INPUT_PIN                    GPIO_ReadInputDataBit(INFRARED_GPIO,INFRARED_PIN)

uint8_t humanInfraredStatus = 0;

void humanInfrared_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(INFRARED_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = INFRARED_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(INFRARED_GPIO, &GPIO_InitStructure);
}

uint8_t get_humanInfrared_status(void)
{
  if(INPUT_PIN)
    humanInfraredStatus = 1;
  else
    humanInfraredStatus = 0;
  return humanInfraredStatus;
}


