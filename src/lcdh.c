#include "stm32f10x.h"
#include "lcdh.h"
#include "conf.h"
#include <stdarg.h>
#include <stdio.h>
#include "shiftout.h"

//				001111
#define DATA_MASK		0x3C

#define RS_MASK			0x01
#define	E_MASK			0x02


#define CMD_CLEAR      0x01
#define CMD_HOME       0x02
#define CMD_ENTRY_MODE 0x04
#define CMD_DISPLAY    0x08
#define CMD_SHIFT      0x10
#define CMD_FUNCTION   0x20
#define CMD_CGRAM_ADR  0x40
#define CMD_DGRAM_ADR  0x80

#define LCD_COLS 20

#define LCD_LINE_1_ADDRESS 0x00
#define LCD_LINE_2_ADDRESS 0x40 
#define LCD_LINE_3_ADDRESS 0x14 
#define LCD_LINE_4_ADDRESS 0x54


/*#define LCD_LINE_1_ADDRESS 0x00 
#define LCD_LINE_2_ADDRESS 0x20 
#define LCD_LINE_3_ADDRESS 0x40 
#define LCD_LINE_4_ADDRESS 0x60 
*/


#define REG_INST 0x00 // write instruction
#define REG_DATA 0x10 // write data into ram
#define REG_READ 0x01 // read busy flag/address counter

#define RS 8
#define E 7
#define REG_INST 0x00 // write instruction
#define REG_DATA 0x10 // write data into ram
#define REG_READ 0x01 // read busy flag/address counter

#define lcd_type 2   // 0=5x7, 1=5x10, 2=2 lines(or more) 

uint8_t const LCD_INIT_STRING[4] = 
{ 
 0x20 | (lcd_type << 2),  // Set mode: 4-bit, 2+ lines, 5x8 dots 
 0xc,                     // Display on 
 1,                       // Clear display 
 6                        // Increment cursor 
 }; 

extern void ts_itoa(char **buf, unsigned int d, int base);
extern int n_itoa( unsigned int d, int base);
//extern void Delay_ms(__IO uint32_t nTime);
//extern void Delay_us(__IO uint32_t nTime);

void lcd_Send(lcd_TypeDef *dev,uint8_t func,uint8_t data);
void lcd_pulseEnable(lcd_TypeDef *dev);
void lcd_send4bits(lcd_TypeDef *dev,uint8_t data);
void lcd_entry_mode(lcd_TypeDef *dev,uint8_t entry);

void lcd_function(lcd_TypeDef *dev,uint8_t func){


	if(!func)
		shiftout(dev->data&=~RS_MASK);
	else shiftout(dev->data|=RS_MASK);

	//if(!func) GPIO_SetBits(dev.GPIO_COM,dev.RS_pin);
	//else GPIO_ResetBits(dev.GPIO_COM,dev.RS_pin);
	}
void lcd_send4bits(lcd_TypeDef *dev,uint8_t data){

	data=data<<2;
	shiftout(dev->data=((dev->data & ~DATA_MASK) | (data & 0x0F)));
	}
void E_togle(lcd_TypeDef *dev){
	shiftout(dev->data|=E_MASK);
	//GPIO_SetBits(dev.GPIO_COM,dev.E_pin);
	//Delay_us(2);
	shiftout(dev->data&=~E_MASK);
	//GPIO_ResetBits(dev.GPIO_COM,dev.E_pin);

	//GPIO_ResetBits(dev.GPIO_COM,dev.E_pin);
	//Delay_us(2);
	//GPIO_SetBits(dev.GPIO_COM,dev.E_pin);
	}
	
void lcd_Send(lcd_TypeDef *dev,uint8_t func,uint8_t data){
	uint8_t temp;

	//GPIO_ResetBits(dev.GPIO_COM,dev.E_pin);
	//GPIO_SetBits(dev.GPIO_COM,dev.E_pin);

	lcd_function(dev,func);
	//if(func==REG_INST) GPIO_ResetBits(dev.GPIO_COM,dev.RS_pin);
	//else GPIO_SetBits(dev.GPIO_COM,dev.RS_pin);
	
	//E_togle(dev);
	Delay_us(40);

	temp=data>>4;
	lcd_send4bits(dev,temp);
	
	//Delay_ms(15);
	E_togle(dev);

	temp=data;
	lcd_send4bits(dev,temp);

	E_togle(dev);

	Delay_us(40);
	
	}
void lcd_Init(lcd_TypeDef *dev){
	uint8_t i;
	GPIO_InitTypeDef  GPIO_InitStructure;
	dev->data=0;
	shiftout(0);

	Delay_ms(100);

	lcd_send4bits(dev,0x03);
      	Delay_ms(5);
	lcd_send4bits(dev,0x03);
      	Delay_ms(5);
	lcd_send4bits(dev,0x03);
      	Delay_ms(5);

	lcd_send4bits(dev,0x02);
      	Delay_ms(5);

	for(i=0; i < sizeof(LCD_INIT_STRING); i++) lcd_Send(dev,REG_INST,LCD_INIT_STRING[i]);
	(*dev).line=1;

	
}
void lcd_display(lcd_TypeDef *dev,uint8_t display)
{
   lcd_Send(dev,REG_INST, CMD_DISPLAY | display);
}
void lcd_shift(lcd_TypeDef *dev,uint8_t shift)
{
   lcd_Send(dev,REG_INST, CMD_SHIFT | shift);
}
void lcd_clear(lcd_TypeDef *dev)
{
	lcd_Send(dev,REG_INST, CMD_CLEAR);
	lcd_puts(dev,"            ");
}
void lcd_clearl(lcd_TypeDef *dev,int l)
{
	lcd_move_to(dev,1,l);
	lcd_puts(dev,"                    ");
	lcd_move_to(dev,1,l);
}
void lcd_home(lcd_TypeDef *dev)
{
   lcd_Send(dev,REG_INST, LCD_RETURNHOME);
}
void lcd_move_to(lcd_TypeDef *dev,int col, int row){
	uint8_t address; 

	switch(row){ 
   		case 1: 
     			address = LCD_LINE_1_ADDRESS; 
			(*dev).line=1;
     			break; 

   		case 2: 
     			address = LCD_LINE_2_ADDRESS; 
			(*dev).line=2;
    			break; 

   		case 3: 
     			address = LCD_LINE_3_ADDRESS; 
			(*dev).line=3;
     			break; 

   		case 4: 
     			address = LCD_LINE_4_ADDRESS; 
			(*dev).line=4;
     			break; 

   		default: 
     			address = LCD_LINE_1_ADDRESS; 
			(*dev).line=1;
     			break;
  		} 
	address += col-1;
	lcd_Send(dev,REG_INST,0x80 | address);
	}
void lcd_putc(lcd_TypeDef *dev,char c)
{
   //lcd_function(dev,REG_DATA);
   lcd_Send(dev,REG_DATA, c);
}

void lcd_puts(lcd_TypeDef *dev,char* s)
{
   while(*s != 0x00) {
      lcd_putc(dev,*s);
      s++;
   }
}
	
/*void lcd_prints(lcd_TypeDef dev,int l,int c,char *fmt, ...){
	va_list ap; 
	char str[100];
	char *p, *sval;
	int ival,tn=0,nz=0;
	signed int val;
	double dval;
	int pre=0,i=0;
	//str=(char*)malloc(50);
	lcd_move_to(&dev,c,l);
	va_start(ap, fmt); 
	for (p = fmt; *p; p++){
		if(*p=='%'){
			p++;
			//numero
			if(*p>='1' && *p<='9'){ pre=(int)(*p-'0');}
			else {pre=0; p--;}
			switch (*++p){
				case 'c':
                               		str[i++] = va_arg(ap, int);
                                	break;
				case 'd':
                        	case 'i':
					val = va_arg(ap, signed int);
					if (val < 0){
						val *= -1;
						str[i++] = '-';
                              			}
					//tn=ts_itoa(&str,val,10);
					if(pre!=0){
						tn=n_itoa(val,10);
						if(tn!=pre){
							if(pre>tn){
								for(i=0;i<(pre-tn);i++) str[i++]='0';
								}
							}
						}
					ts_itoa(&str,val,10);
					
					
                               		break;
				case 'f':
					dval = va_arg(ap, double);
				//printf("%f", dval);
				break;
				case 's':
				//for (sval = va_arg(ap, char *); *sval; sval++;
				//putchar(*sval);
				break;
				default:
				break;
				}
			}
		else str[i++]=*p;
	}
			
	str[i]=0;
	lcd_puts(dev,str);
	va_end(ap); 
}*/
/*void lcd_prints(lcd_TypeDef dev,int l,int c,char *fmt, ...){
	va_list ap;
	
*/
	
	
