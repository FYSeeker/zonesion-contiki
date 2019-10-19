#include "button.h"

#define BUTTON_GPIO                GPIOA
#define BUTTON_CLK                 RCC_APB2Periph_GPIOA
#define BUTTON_PIN                 GPIO_Pin_7
#define INPUT_PIN                  GPIO_ReadInputDataBit(BUTTON_GPIO,BUTTON_PIN)

uint8_t buttonStatus = 0;

void button_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(BUTTON_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = BUTTON_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(BUTTON_GPIO, &GPIO_InitStructure);
}

uint8_t get_button_status(void)
{
  if(INPUT_PIN)
    buttonStatus = 0;
  else
    buttonStatus = 1;
  return buttonStatus;
}


