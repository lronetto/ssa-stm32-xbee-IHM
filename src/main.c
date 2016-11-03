#include "stm32f10x.h"
#include "conf.h"
//#include "controle.h"
#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ds18b20.h"
#include "xbee.h"
#include "func.h"
#include "ssa.h"
#include "lcdh.h"
#include "shiftout.h"


ds18b20_TypeDef ds1,ds2;
void func_process();
void xbee_process();

GPIO_InitTypeDef GPIO_InitStructure;

Inout_T	inout;
Func_T func;
xbee_t xbee;

AnalogVar_T var;
AnalogVar_T var_aux;

uint8_t payload[100];

uint8_t	xbee_broadcast[]={0,0,0,0,0,0,0xFF,0xFF};
uint8_t xbee_coord[8];

uint8_t flag_in[]={0,0,0,0};
int i,j;
int main(){
	uint32_t start,start1,start2;
	lcd_TypeDef lcd;
	Delay_Init();
	usart1_init();
	//EXTI_config();
	//usart2_init();
	//usart3_init();
	//ADC1_Initm();
	uint8_t func_addr[8]={0x00,0x13,0xA2,0x00,0x40,0xE5,0xF2,0x44};






	//SysTick_Config(SystemCoreClock / 1000);
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIOA->BSRR=GPIO_Pin_11;
	Delay_ms(500);
	payload[0]=SSA_F_CORD_ADDR;
	printf("ola mundo\r\n");
	//xbee_SendData(&xbee,xbee_broadcast,payload,1);
	//lcd_Init(&lcd);
	//lcd_move_to(&lcd,1,1);
	//lcd_puts(&lcd,"asm,ndbc");
	//shiftout(0x22);
	start=TIM2->CNT;
	start2=TIM2->CNT;
	i=0;
	while(1){
		shiftout(i);
		Delay_ms(5);
		i++;
		if(i==255) i=0;

		//xbee_process();
		//func_process();
		}
	}
void USART2_IRQHandler(void){
	if(USART_GetFlagStatus(USART2,USART_IT_RXNE)){
	xbee_usart(&xbee,USART_ReceiveData(USART2));
	}
	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
}
void EXTI9_5_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line9) != RESET){
		flag_in[0]=1;
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
	if(EXTI_GetITStatus(EXTI_Line8) != RESET){
			flag_in[1]=1;
			EXTI_ClearITPendingBit(EXTI_Line8);
		}
	if(EXTI_GetITStatus(EXTI_Line7) != RESET){
			flag_in[2]=1;
			EXTI_ClearITPendingBit(EXTI_Line7);
		}
	if(EXTI_GetITStatus(EXTI_Line6) != RESET){
			flag_in[3]=1;
			EXTI_ClearITPendingBit(EXTI_Line6);
		}
}
void func_process(){
	for(i=0;i<4;i++){
		if(flag_in[i]){
			printf("in %d\r\n",i);
			//funcao ativa?
			if(func.aci[i].act){
				//é funcao local?
				switch(func.aci[i].type){
					case FUNC_LOCAL:
						func_GPIO_T((inout.out[func.aci[i].out]));
						payload[0]=SSA_F_OUTP;
						payload[1]=func.aci[i].out;
						xbee_SendData(&xbee,xbee_coord,payload,2);
						printf("teste\r\n");
						break;
					case FUNC_REMOTE:
						payload[0]=SSA_F_OUT;
						payload[1]=func.aci[i].out;
						xbee_SendData(&xbee,func.aci[i].dest_addr,payload,2);
						break;
					case SSA_F_ANALOG_TEMPO:
						for(i=0;i<xbee.buf[XBEE_PAYLOAD_OFFSET+1];i++){
							var.tempo[xbee.buf[XBEE_PAYLOAD_OFFSET+2+i*2]]=xbee.buf[XBEE_PAYLOAD_OFFSET+3+i*2];
							var.temposum[xbee.buf[XBEE_PAYLOAD_OFFSET+2+i*2]]=0;
							}
						break;
					}
				}
			flag_in[i]=0;
			}
		}
	}
void xbee_process(void){

	if(!xbee.start){
		xbee_reciver(&xbee);
		switch(xbee.type){
			case XBEE_STATUS:
				if(!xbee.buf[XBEE_STATUS_SUCESS]){
					printf("success\r\n");
					}
				break;
			case XBEE_RECEIVE_PACKET:
				switch(xbee.buf[XBEE_PAYLOAD_OFFSET]){
					//recebe o endereco de coordinator
					case SSA_F_CORD_ADDRP:
						printf("cord addr: ");
						for(i=0;i<8;i++){
							xbee.addr_cord[i]=xbee.buf[XBEE_PAYLOAD_OFFSET+1+i];
							printf("0x%02X ",xbee.buf[XBEE_PAYLOAD_OFFSET+1+i]);
						}
						printf("\r\n");
					break;
					//altera o estado da saida e manda o estado atual de volta
					case SSA_F_OUT:
						payload[0]=SSA_F_OUTP;
						payload[1]=xbee.buf[XBEE_PAYLOAD_OFFSET+1];
						printf("out=%d\r\n",payload[1]);
						payload[2]=func_GPIO_T((inout.out[xbee.buf[XBEE_PAYLOAD_OFFSET+1]]));
						xbee_SendData(&xbee,xbee.addr_cord,payload,3);
						break;
					case SSA_F_FUNC:
						func.aci[xbee.buf[XBEE_PAYLOAD_OFFSET+2]].act=1;
						func.aci[xbee.buf[XBEE_PAYLOAD_OFFSET+2]].type=xbee.buf[XBEE_PAYLOAD_OFFSET+1];
						func.aci[xbee.buf[XBEE_PAYLOAD_OFFSET+2]].out=xbee.buf[XBEE_PAYLOAD_OFFSET+3];
						for(i=0;i<8;i++)
							func.aci[xbee.buf[XBEE_PAYLOAD_OFFSET+2]].dest_addr[i]=xbee.buf[XBEE_PAYLOAD_OFFSET+4+i];

						break;
					}
				break;
			}
		xbee.start=1;
		}
	}
