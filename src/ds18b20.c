// Example of how to use the pcd8544 library, the lcd-driver used in
// Nokia 3310 and others.
//
// Tested on Sparkfun's "Graphic LCD 84x48 - Nokia 5110".
#include "stm32f10x.h"
#include "ds18b20.h"
#include "conf.h"
#include <stdio.h>

//extern void Delay_ms(__IO uint32_t nTime);
//extern void Delay_us(__IO uint32_t nTime);


void Ds18b20_Mode(ds18b20_TypeDef ds,GPIOMode_TypeDef mode);
void Ds18b20_Reset(ds18b20_TypeDef ds);
void Ds18b20_WriteByte(ds18b20_TypeDef ds, int data);
//void Ds18b20_WriteBit(ds18b20_TypeDef ds, int bit);
int Ds18b20_ReadByte(ds18b20_TypeDef ds);
//int Ds18b20_ReadBit(ds18b20_TypeDef ds);
//int Ds18b20_TouchByte(ds18b20_TypeDef ds, int data);
//void Ds18b20_Block(ds18b20_TypeDef ds, unsigned char *data, int data_len);

#define DS18B20_MODE_READ GPIO_Mode_IN_FLOATING
#define DS18B20_MODE_WRITE GPIO_Mode_Out_PP

void Ds18b20_Init(ds18b20_TypeDef *ds,GPIO_TypeDef* GPIO,uint32_t GPIO_Pin,uint8_t *addr){
	GPIO_InitTypeDef GPIO_conf;
	(*ds).GPIO=GPIO;
	(*ds).flag=0;
	
	GPIO_conf.GPIO_Pin = GPIO_Pin;
  	GPIO_conf.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_conf.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIO, &GPIO_conf);
	(*ds).GPIO_conf=GPIO_conf;
	(*ds).addr=addr;
	}
void Ds18b20_Mode(ds18b20_TypeDef ds,GPIOMode_TypeDef mode){
	ds.GPIO_conf.GPIO_Mode=mode;
	GPIO_Init(ds.GPIO, &ds.GPIO_conf);
	}

	
void Ds18b20_Reset(ds18b20_TypeDef ds){
	uint16_t timer;
	__disable_irq();
	//printf("teste1\r\n");
	Ds18b20_Mode(ds,DS18B20_MODE_WRITE);
	//ds.GPIO->BSRR=ds.GPIO_conf.GPIO_Pin;
	//printf("teste2\r\n");
	//GPIO_SetBits(ds.GPIO,ds.GPIO_conf.GPIO_Pin);
	ds.GPIO->BSRR=ds.GPIO_conf.GPIO_Pin;
	Delay_us(4);
	ds.GPIO->BRR=ds.GPIO_conf.GPIO_Pin;
	//printf("teste3\r\n");
	//GPIO_ResetBits(ds.GPIO,ds.GPIO_conf.GPIO_Pin);
	//printf("teste4\r\n");
	Delay_us(490);
	//ds.GPIO->BSRR=ds.GPIO_conf.GPIO_Pin;
	//GPIO_SetBits(ds.GPIO,ds.GPIO_conf.GPIO_Pin);
	Ds18b20_Mode(ds,DS18B20_MODE_READ);
	//Delay_us(15);
	timer=TIM3->CNT;
	while(GPIO_ReadInputDataBit(ds.GPIO,ds.GPIO_conf.GPIO_Pin) & ((TIM3->CNT - timer)<500));
	//GPIO_ReadInputDataBit(ds.GPIO,ds.GPIO_conf.GPIO_Pin);
	Ds18b20_Mode(ds,DS18B20_MODE_WRITE);
	ds.GPIO->BSRR=ds.GPIO_conf.GPIO_Pin;
	//GPIO_SetBits(ds.GPIO,ds.GPIO_conf.GPIO_Pin);
	Delay_us(120);
	__enable_irq();
	}

void Ds18b20_WriteByte(ds18b20_TypeDef ds, int data){
	int i;

	__disable_irq();
	for(i=0;i<8;i++){
		ds.GPIO->BRR=ds.GPIO_conf.GPIO_Pin;
		//GPIO_ResetBits(ds.GPIO,ds.GPIO_conf.GPIO_Pin);
		if(data & 0x01){
			ds.GPIO->BSRR=ds.GPIO_conf.GPIO_Pin;
			//Delay_us(59);
			//time=60;
			//GPIO_SetBits(ds.GPIO,ds.GPIO_conf.GPIO_Pin);
			}
		else{
			ds.GPIO->BRR=ds.GPIO_conf.GPIO_Pin;
			//Delay_us(1);
			//ds.GPIO->BSRR=ds.GPIO_conf.GPIO_Pin;
			//Delay_us(58);
			}
			//GPIO_ResetBits(ds.GPIO,ds.GPIO_conf.GPIO_Pin);
		data>>=1;
		Delay_us(60);
		ds.GPIO->BSRR=ds.GPIO_conf.GPIO_Pin;
		//GPIO_SetBits(ds.GPIO,ds.GPIO_conf.GPIO_Pin);
		//Delay_us(1);
		}
	ds.GPIO->BSRR=ds.GPIO_conf.GPIO_Pin;
	//GPIO_SetBits(ds.GPIO,ds.GPIO_conf.GPIO_Pin);
	__enable_irq();
	}
		
int Ds18b20_ReadByte(ds18b20_TypeDef ds){
	uint16_t i,x=0;
	__disable_irq();
	Ds18b20_Mode(ds,DS18B20_MODE_WRITE);
	for(i=0;i<8;i++){
		ds.GPIO->BRR=ds.GPIO_conf.GPIO_Pin;
		//GPIO_ResetBits(ds.GPIO,ds.GPIO_conf.GPIO_Pin);
		x>>=1;
		ds.GPIO->BSRR=ds.GPIO_conf.GPIO_Pin;
		//GPIO_SetBits(ds.GPIO,ds.GPIO_conf.GPIO_Pin);
		Ds18b20_Mode(ds,DS18B20_MODE_READ);
		Delay_us(15);
		if(ds.GPIO->IDR & ds.GPIO_conf.GPIO_Pin)
		//if(GPIO_ReadInputDataBit(ds.GPIO,ds.GPIO_conf.GPIO_Pin))
			x|=0x80;
		Delay_us(100);
		Ds18b20_Mode(ds,DS18B20_MODE_WRITE);
		}
	__enable_irq();
	return x;
	}
		
void Ds18b20_ConvertTemperature(ds18b20_TypeDef ds)
{
int i;

Ds18b20_Reset(ds); // Perform Master Reset of OneWire Bus
Ds18b20_WriteByte(ds, 0x55);
for(i=0;i<8;i++) Ds18b20_WriteByte(ds, ds.addr[i]);
//Ds18b20_WriteByte(ds, 0xCC);	 // skip ROM
Ds18b20_WriteByte(ds, 0x44);	 // convert temperature
//Ds18b20_Reset(ds);	 // Perform Master Reset of OneWire Bus
}

void Ds18b20_ReadTemperature(ds18b20_TypeDef *ds)
{
uint16_t ls,ms;
uint16_t temp;
int sig=0;
int i;
Ds18b20_Reset(*ds);
//Ds18b20_WriteByte(*ds, 0xCC);	 // skip ROM
Ds18b20_WriteByte(*ds, 0x55);
for(i=0;i<8;i++) Ds18b20_WriteByte(*ds, (*ds).addr[i]);
Ds18b20_WriteByte(*ds, 0xBE);	 // read scratch pad

ls=Ds18b20_ReadByte(*ds);
ms=Ds18b20_ReadByte(*ds);
//printf("LS 0x%X MS 0x%X",ls,ms);
if(ms & 0xF0){
	ms&=~(ms & 0xFF);
	sig=1;
//	printf(" MS1 0x%X ",ms);
}
temp= ls | ms<<8;
if(temp!=0xFF){
	if(!sig) (*ds).temp=(float)temp/16.0;
	else  (*ds).temp=(float)temp/-16.0;
	//printf("0x%X\n",temp);
	}
//temp=temp/16;


}
void Ds18b20_ReadROM(ds18b20_TypeDef *ds){
	int i;

	Ds18b20_Reset(*ds);
	Ds18b20_WriteByte(*ds, 0x33);
	for(i=0;i<8;i++){
		//ds->addr[i]=Ds18b20_ReadByte(*ds);
		printf("0x%02X ",ds->addr[i]);
	}

}



