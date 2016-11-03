#ifndef DS18B20_H_
#define DS18B20_H_

#include "stm32f10x.h"
typedef struct {
	GPIO_TypeDef* GPIO;
	GPIO_InitTypeDef GPIO_conf;
	int flag;
	float temp;
	uint8_t *addr;
	}ds18b20_TypeDef;



void Ds18b20_Init(ds18b20_TypeDef *ds,GPIO_TypeDef* GPIO,uint32_t GPIO_Pin,uint8_t *addr);
void Ds18b20_ConvertTemperature(ds18b20_TypeDef ds);
void Ds18b20_ReadTemperature(ds18b20_TypeDef *ds);
int Ds18b20_ReadPower(ds18b20_TypeDef ds);
void Ds18b20_ReadROM(ds18b20_TypeDef *ds);

#endif
