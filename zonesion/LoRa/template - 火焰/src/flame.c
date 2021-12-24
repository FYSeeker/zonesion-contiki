#include "flame.h"

#define FLAME_CLK                 RCC_APB2Periph_GPIOA
#define FLAME_GPIO                GPIOA
#define FLAME_PIN                 GPIO_Pin_7
#define INPUT_PIN                 GPIO_ReadInputDataBit(FLAME_GPIO,FLAME_PIN)

uint8_t flameStatus = 0;

void flame_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(FLAME_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = FLAME_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(FLAME_GPIO, &GPIO_InitStructure);
}

uint8_t get_flame_status(void)
{
  if(INPUT_PIN)
    flameStatus = 0;
  else
    flameStatus = 1;
  return flameStatus;
}


