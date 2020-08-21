/********************************************************************
* FILENAME: main.c
* PROJECT:  PIC795_LCD_4BIT_Demo
*      
* DEMONSTRATES BASIC LCD FUNCTIONS USING 4 BIT DATA INTERFACE
* 
* Tested with DM1602K-NSW-FBS-3.3v
* Should work with any standard 2x16 display
* Processor: 	   PIC32MX795F512L on UBW32 Board
* Compiler: 	   Microchip XC32 V1.30 IDE: V5.15
*
* PORTD RD0,RD1,RD2,RD3 connected to LCD D4, D5, D6, D7
* PORTE:
* LCD_RS: RE0
* LCD_RW: RE1
* LCD_EN: RE2
* 
* 8-21-2020 JBS Written, debugged, tested, stored on Git
********************************************************************/
#include "LCD_4BIT_PIC32.h"
#include "string.h"
#include "plib.h"
#include "Delay.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


/** CONFIGURATION **************************************************/
#pragma config FSOSCEN   = ON           // Secondary osclllator enabled
#pragma config UPLLEN   = ON            // USB PLL Enabled
#pragma config FPLLMUL  = MUL_15        // PLL Multiplier
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = HS            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select

/** I N C L U D E S **********************************************************/


#define CR 13
#define LF 10
#define BACKSPACE 8

// UART FOR PC SERIAL PORT
#define HOSTuart UART2
#define HOSTbits U2STAbits
#define HOST_VECTOR _UART_2_VECTOR

#define false FALSE
#define true TRUE
/** V A R I A B L E S ********************************************************/

#define MAX_RX_BUFFERSIZE 64
unsigned char HOSTRxBuffer[MAX_RX_BUFFERSIZE];
unsigned char HOSTRxBufferFull = false;
unsigned short HOSTRxIndex = 0;

void InitializeSystem(void);

void main(void)
{       
    int TestCounter = 0;
    unsigned char strLineOne[32];
    unsigned char strLineTwo[32];
    
#define MAXNUMBYTES 16
unsigned char Line1String[MAXNUMBYTES+1] = "Hallelujah I'm a";
unsigned char Line2String[MAXNUMBYTES+1] = "bum bum bum Yeah";
unsigned char TestChar = 'A';

    InitializeSystem();   
    
    DelayMs(200);    
    LCDInit();
    LCDGoto(0, 0);
    LCDWriteArray(Line1String);
    LCDGoto(0, 1);
    LCDWriteArray(Line2String);

    while(1)
    {    
        DelayMs(200);    
        sprintf(strLineOne, "Test %d:        ", TestCounter++);
        sprintf(strLineTwo, "Character: %c    ", TestChar++);
        if (TestChar > 'Z') TestChar = 'A';
        
        LCDGoto(0, 0);
        LCDWriteArray(strLineOne);
        LCDGoto(0, 1);
        LCDWriteArray(strLineTwo);
    }
}    
 


void InitializeSystem(void) 
{         
    SYSTEMConfigPerformance(60000000);
    OSCCONbits.SOSCEN = 1; // Enable secondary oscillator
    
    // Turn off JTAG so we get the pins back
    mJTAGPortEnable(false);
        
   
    // Set up Timer 2 for 1 millisecond
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_32, 1875);
    // set up the core timer interrupt with a priority of 5 and zero sub-priority
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_5);        
 
    
    // Set up HOST UART    
    UARTConfigure(HOSTuart, UART_ENABLE_HIGH_SPEED | UART_ENABLE_PINS_TX_RX_ONLY);
    UARTSetFifoMode(HOSTuart, UART_INTERRUPT_ON_TX_DONE | UART_INTERRUPT_ON_RX_NOT_EMPTY);
    UARTSetLineControl(HOSTuart, UART_DATA_SIZE_8_BITS | UART_PARITY_NONE | UART_STOP_BITS_1);
#define SYS_FREQ 60000000
    UARTSetDataRate(HOSTuart, SYS_FREQ, 115200);
    UARTEnable(HOSTuart, UART_ENABLE_FLAGS(UART_PERIPHERAL | UART_RX | UART_TX));

    // Configure HOST UART Interrupts
    INTEnable(INT_SOURCE_UART_TX(HOSTuart), INT_DISABLED);
    INTEnable(INT_SOURCE_UART_RX(HOSTuart), INT_ENABLED);
    INTSetVectorPriority(INT_VECTOR_UART(HOSTuart), INT_PRIORITY_LEVEL_2);
    INTSetVectorSubPriority(INT_VECTOR_UART(HOSTuart), INT_SUB_PRIORITY_LEVEL_0);

    mPORTBClearBits(BIT_0 | BIT_1 | BIT_2 | BIT_3 | BIT_4 | BIT_5 | BIT_6 | BIT_7);    
    mPORTBSetPinsDigitalOut(BIT_0 | BIT_1 | BIT_2 | BIT_3 | BIT_4 | BIT_5 | BIT_6 | BIT_7);
    
    mPORTESetPinsDigitalOut(BIT_0 | BIT_1 | BIT_2); 
    mPORTEClearBits(BIT_0 | BIT_1 | BIT_2);
    
    mPORTDSetPinsDigitalOut(BIT_0 | BIT_1 | BIT_2 | BIT_3); 
    mPORTDClearBits(BIT_0 | BIT_1 | BIT_2 | BIT_3);
    
    
    // Turn on the interrupts
    // INTEnableSystemMultiVectoredInt();

}//end UserInit

