#include "window.h"

#define WINDOW_CLK                 RCC_APB2Periph_GPIOA
#define WINDOW_GPIO                GPIOA
#define WINDOW_PIN                 GPIO_Pin_7
#define INPUT_PIN                  GPIO_ReadInputDataBit(WINDOW_GPIO,WINDOW_PIN)

uint8_t windowStatus = 0;

void window_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(WINDOW_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = WINDOW_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(WINDOW_GPIO, &GPIO_InitStructure);
}

uint8_t get_window_status(void)
{
  if(INPUT_PIN)
    windowStatus = 1;
  else
    windowStatus = 0;
  return windowStatus;
}


