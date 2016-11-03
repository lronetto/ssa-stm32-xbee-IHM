#include "stm32f10x.h"

// Function constants passed to constructor, can be OR'd together
#define FUNCTION_8BIT  0x10 // enable 8 pin mode
#define FUNCTION_4BIT  0x00 // enable 4 pin mode
#define FUNCTION_2LINE 0x08 // LCD has two lines, line two starts at addr 0x40
#define FUNCTION_1LINE 0x00 // LCD has one continues line
#define FUNCTION_5x11  0x04 // use 5x11 custom characters
#define FUNCTION_5x8   0x00 // use 5x8 custom characters

// CMD_DISPLAY parameters, can be OR'd together
#define DISPLAY_ON  0x04      // display on
#define DISPLAY_OFF 0x00      // display off
#define DISPLAY_CURSOR   0x02 // cursor on
#define DISPLAY_NOCURSOR 0x00 // cursor off
#define DISPLAY_BLINK   0x01  // cursor blink on
#define DISPLAY_NOBLINK 0x00  // curson blink off

// CMD_ENTRY_MODE parameters, can be OR'd together
#define ENTRY_CURSOR_INC 0x02 // increment cursor position
#define ENTRY_CURSOR_DEC 0x00 // decrement curson position
#define ENTRY_SHIFT      0x01 // shift entire display
#define ENTRY_NOSHIFT    0x00 // don't shift display

// CMD_SHIFT parameters, can be OR'd together
#define SHIFT_SCREEN 0x08 // shift display 
#define SHIFT_CURSOR 0x00 // shift cursor
#define SHIFT_RIGHT  0x04 // to the right
#define SHIFT_LEFT   0x00 // to the left

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

typedef struct{
	GPIO_TypeDef* GPIO_COM;
	GPIO_TypeDef* GPIO_DATA;	
	uint32_t E_pin;
	uint32_t RS_pin;
	uint32_t D7_pin;
	uint32_t D6_pin;
	uint32_t D5_pin;
	uint32_t D4_pin;
	uint32_t LED_pin;
	uint8_t line;
	uint8_t data;
	}lcd_TypeDef;


void lcd_Init(lcd_TypeDef *dev);
void lcd_display(lcd_TypeDef *dev,uint8_t display);
void lcd_shift(lcd_TypeDef *dev,uint8_t shift);
void lcd_clear(lcd_TypeDef *dev);
void lcd_clearl(lcd_TypeDef *dev,int l);
void lcd_move_to(lcd_TypeDef *dev,int col, int row);
void lcd_putc(lcd_TypeDef *dev,char c);
void lcd_puts(lcd_TypeDef *dev,char* s);
void lcd_prints(lcd_TypeDef *dev,int l,int c,char *fmt, ...);

