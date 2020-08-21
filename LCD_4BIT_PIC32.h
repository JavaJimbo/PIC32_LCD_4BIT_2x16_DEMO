/********************************************************************
* FileName:        LCD_4BIT_PIC32.h
*
********************************************************************/

#include "plib.h"

#define TRUE  	1                           
#define FALSE 	0			

#define ENABLE  1                       
#define DISABLE 0			

#define SET  	1                       
#define CLEAR	0			

#define LCD_RS  PORTBbits.RB0
#define LCD_RW  PORTBbits.RB1
#define LCD_EN  PORTBbits.RB2

#define LCD_D4  PORTBbits.RB4
#define LCD_D5  PORTBbits.RB5
#define LCD_D6  PORTBbits.RB6
#define LCD_D7  PORTBbits.RB7

// LCD instructions
#define LCD_COMMAND 1
#define LCD_DATA 0
#define lcd_Clear           0b00000001          // replace all characters with ASCII 'space'
#define lcd_Home            0b00000010          // return cursor to first position on first line
#define lcd_EntryMode       0b00000110          // shift cursor from left to right on read/write
#define lcd_DisplayOff      0b00001000          // turn display off
#define lcd_DisplayOn       0b00001110          // display on, cursor on, don't blink character
#define lcd_FunctionReset   0b00110000          // reset the LCD
#define lcd_FunctionSet4bit 0b00101000          // 4-bit data, 2-line display, 5 x 7 font
#define lcd_SetCursor       0b10000000          // set cursor position

// #define	LCD_STROBE {LCD_EN=1; DelayUs(1); LCD_EN=0; DelayMs(10);}
// #define	LCD_STROBE {mPORTBSetBits(BIT_2); DelayUs(1); mPORTBClearBits(BIT_2);}
#define	LCD_STROBE {mPORTESetBits(BIT_2); DelayUs(1); mPORTEClearBits(BIT_2);}

void WaitLCD(void);
void WriteNibble(unsigned char CommandFlag, unsigned char byte);
void WriteByte(unsigned char CommandFlag, unsigned char byte);
void LCDInit(void);
void LCDClear(void);
void LCDGoto(unsigned char Pos,  unsigned char Ln);
void LCDPutChar(unsigned char Data);
void LCDPutByte(unsigned char Val);
void LCDWriteStr(char  *Str);
void LCDWriteArray (unsigned char  *arrayPtr);

