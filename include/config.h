

#include "stm32f10x_conf.h"
//#include "var.h"


typedef union{
	float val;
	struct{
		char b[4];
    	}byte;
}floatbyte_T;
typedef struct{
	uint32_t pin;
	uint8_t estado;
}Pin_T;
typedef struct{
	uint8_t tipo;
	uint8_t id;
	Pin_T pin[20];
	uint8_t qtd_pin;
	floatbyte_T val;
}var_T;
typedef struct{
	uint8_t qtd;
	var_T var[5];
}vart_T;


typedef union{
	uint16_t val;
	struct{
		char b1;
		char b2;
	}byte;
}uint16byte_T;

typedef struct{
	uint8_t	type[5];
	floatbyte_T ana[5];

	uint16_t tempo[5];
	uint16_t temposum[5];
	uint8_t qtd;
	uint8_t n[5];
}AnalogVar_T;

typedef struct {

	//disp_conf disp;
}EE_t;

void EE_Write(EE_t EEP);
void EE_Read(EE_t *EEP);
