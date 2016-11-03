/*
 * xbee.h
 *
 *  Created on: 14/08/2015
 *      Author: leandro
 */

#ifndef XBEE_H_
#define XBEE_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f10x.h"

#define XBEE_RECEIVE_PACKET 	0x90
#define XBEE_TRANSMIT_REQUEST	0x10
#define XBEE_STATUS		0x8B
#define XBEE_PAYLOAD_OFFSET		0x0F
#define	XBEE_STATUS_SUCESS		0x08

typedef struct{
	uint8_t type;
	uint8_t Address[8];
	uint8_t addr_cord[8];
	uint8_t source_Address[8];
	uint8_t payload[200];
	uint8_t buf[200];
	uint8_t size,id,start;

}xbee_t;

void xbee_reciver(xbee_t *xbee);
void xbee_SendData(xbee_t *xbee,uint8_t *address,uint8_t *data,uint8_t size);
void xbee_usart(xbee_t *xbee,uint8_t data);

#endif /* XBEE_H_ */
