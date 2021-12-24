#include "switch.h"

#define SWITCH_GPIO                GPIOA
#define SWITCH_CLK                 RCC_APB2Periph_GPIOA
#define SWITCH_PIN                 GPIO_Pin_7
#define INPUT_PIN                  GPIO_ReadInputDataBit(SWITCH_GPIO,SWITCH_PIN)

uint8_t switchStatus = 0;

void switch_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(SWITCH_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = SWITCH_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(SWITCH_GPIO, &GPIO_InitStructure);
}

uint8_t get_switch_status(void)
{
  if(INPUT_PIN)
    switchStatus = 0;
  else
    switchStatus = 1;
  return switchStatus;
}


