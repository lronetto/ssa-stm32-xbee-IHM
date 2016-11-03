
#include "stm32f10x_conf.h"



void aux_ADC1_DMA(void);
void Delay_Init();
void TIM1_PWM(void);
void Delay_ms(__IO uint32_t nTime);
void Delay_us(__IO uint32_t nTime);

#define STDOUT_USART 3
#define STDERR_USART 3
#define STDIN_USART 3

void ADC1_Initm(void);

void usart1_init();
void usart2_init();
void usart3_init();
void usart_send(USART_TypeDef *usart,char *str);
void usart_send_n(USART_TypeDef *usart,char *str, int n);

void EXTI_config(void);



