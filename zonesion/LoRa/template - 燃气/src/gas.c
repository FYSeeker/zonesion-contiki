#include "gas.h"

#define GAS_GPIO                GPIOA
#define GAS_CLK                 RCC_APB2Periph_GPIOA
#define GAS_PIN                 GPIO_Pin_7
#define INPUT_PIN                    GPIO_ReadInputDataBit(GAS_GPIO,GAS_PIN)

uint8_t gasStatus = 0;

void gas_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(GAS_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = GAS_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GAS_GPIO, &GPIO_InitStructure);
}

uint8_t get_gas_status(void)
{
  if(INPUT_PIN)
    gasStatus = 1;
  else
    gasStatus = 0;
  return gasStatus;
}


