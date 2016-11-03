/*
 * func.c
 *
 *  Created on: 1 de jun de 2016
 *      Author: Leandro
 */

#include "func.h"

int func_GPIO_T(GPIO_T gpio){
	if(gpio.GPIO->ODR & gpio.pin){
		gpio.GPIO->ODR&=~gpio.pin;
		return 0;
	}
	else{
		gpio.GPIO->ODR|=gpio.pin;
		return 1;
	}
}
void func_exec(Func_T func){

}
