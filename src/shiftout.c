
#include "stm32f10x.h"



#define	CLOCK_ON	GPIOA->ODR|=GPIO_Pin_12
#define	LATCH_ON	GPIOA->ODR|=GPIO_Pin_11
#define DATA_ON		GPIOA->ODR|=GPIO_Pin_10

#define	CLOCK_OFF	GPIOA->ODR&=~GPIO_Pin_12
#define	LATCH_OFF	GPIOA->ODR&=~GPIO_Pin_11
#define DATA_OFF	GPIOA->ODR&=~GPIO_Pin_10

void shiftout(uint8_t out){
	int i;
	LATCH_OFF;
	for(i=7;i>=0;i--){
		CLOCK_ON;
		Delay_us(10);
		if(out & i<<1) DATA_ON;
		else DATA_OFF;
		Delay_us(10);
		CLOCK_OFF;
		Delay_us(10);
	}
	LATCH_ON;
}
