/*
 * xbee.c
 *
 *  Created on: 14/08/2015
 *      Author: leandro
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "xbee.h"
#include "conf.h"
#include "stm32f10x.h"
void xbee_get_address(xbee_t *xbee);
uint16_t cnt=0;
void xbee_reciver(xbee_t *xbee){
	int tam=xbee->buf[1] | xbee->buf[2];
	int i,sum=0;
	for(i=3;i<tam+3;i++){
		sum+=xbee->buf[i];
		//printf("0x%02X ",data[i]);
		}
	sum&=0xFF;
	sum=0xFF-sum;
	//printf("checksum: 0x%02X calc:0x%02X",data[tam+3],sum);
	if(xbee->buf[tam+3]==sum){
		xbee->type=xbee->buf[3];
		xbee_get_address(xbee);
	}
	else xbee->type=-1;

}
void xbee_get_address(xbee_t *xbee){
	int i;
	for(i=0;i<8;i++)
		xbee->source_Address[i]=xbee->buf[4+i];
}
void xbee_data(xbee_t *xbee,uint8_t *data,uint8_t size){
	int i;
	for(i=17;i<(size+17);i++){
		xbee->payload[i]=data[i-17];
	}
	//printf("'%s' %d\n",data,size);
	xbee->size=size;
}
void xbee_address(xbee_t *xbee,uint8_t *address){
	int i;
	for(i=0;i<8;i++){
		xbee->Address[i]=address[i];
		//printf("0x%02x ",xbee->Address[i]);
	}
}
uint8_t xbee_checksum(xbee_t *xbee){
	int sum=0,i;
	for(i=3;i<xbee->size+3;i++)
		sum+=xbee->payload[i];
	sum&=0xFF;
	sum=0xFF-sum;
	return sum;
}
void xbee_packet(xbee_t *xbee){
	int i;
	xbee->size+=14;
	xbee->payload[0]=0x7e;
	xbee->payload[1]=0;
	xbee->payload[2]=xbee->size;
	//tipo
	xbee->payload[3]=0x10;
	//id
	xbee->payload[4]=0x10;
	//address
	for(i=5;i<(5+8);i++)
		xbee->payload[i]=xbee->Address[i-5];
	xbee->payload[13]=0xFF;
	xbee->payload[14]=0xFE;
	xbee->payload[15]=0x00;
	xbee->payload[16]=0x00;
	xbee->payload[xbee->size+3]=xbee_checksum(xbee);
	//printf("tam %d \n",xbee->size);
}
void xbee_Send(xbee_t *xbee){
	int i;
	//printf("size=%d\r\n",xbee->size+4);
	for(i=0;i<xbee->size+4;i++){
		Delay_ms(1);
		USART_SendData(USART2,xbee->payload[i]);
		//printf("%02x ",xbee->payload[i]);
		}
	//printf("\n");
}
void xbee_SendData(xbee_t *xbee,uint8_t *address,uint8_t *data,uint8_t size){
	//printf("teste1\r\n");
	xbee_data(xbee,data,size);
	//printf("teste2\r\n");
	xbee_address(xbee,address);
	//printf("teste3\r\n");
	xbee_packet(xbee);
	//printf("teste4\r\n");
	usart_send_n(USART2,xbee->payload,xbee->size+4);
	//xbee_Send(xbee);
	//printf("teste5\r\n");
}
void xbee_usart(xbee_t *xbee,uint8_t data){
	uint8_t flag_c=0,i;
	uint8_t sum;
	if(xbee->start){
		xbee->buf[cnt]=data;
		//if(flag_c){
		//	flag_c=0;
		//	flag_s=0;
		//	}
		if(xbee->buf[cnt]==0x7e){
			xbee->buf[0]=0x7e;
			cnt=0;
			}
		sum=0;
		if(cnt>7){
			for(i=3;i<cnt;i++)
				sum+=xbee->buf[i];
			sum&=0xFF;
			sum=0xFF-sum;
			if(sum==xbee->buf[cnt]){
				xbee->start=0;
				cnt=0;
				}
			}
		cnt++;
		}
}

