/*
 * func.h
 *
 *  Created on: 1 de jun de 2016
 *      Author: Leandro
 */

#ifndef INCLUDE_FUNC_H_
#define INCLUDE_FUNC_H_
#include "stm32f10x.h"

#define FUNC_LOCAL	0
#define FUNC_REMOTE	1

enum{
	IN0=0,
	IN1,
	IN2,
	IN3,
	IN4
}IN;
enum{
	OUT0=0,
	OUT1,
	OUT2,
	OUT3,
	OUT4
}OUT;
//type
//0 - local
//1 - remota
typedef struct{
	GPIO_TypeDef* GPIO;
	uint32_t pin;
	uint8_t state;
}GPIO_T;
typedef struct{
	GPIO_T in[4],out[5];
}Inout_T;

typedef struct{
	uint8_t type,out;
	uint8_t dest_addr[8];
	uint8_t act;
}Func_aci_T;
typedef struct{
	Func_aci_T aci[4];
}Func_T;


int func_GPIO_T(GPIO_T gpio);


#endif /* INCLUDE_FUNC_H_ */
