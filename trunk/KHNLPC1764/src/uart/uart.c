//*****************************************************************************
//   +--+       
//   | ++----+   
//   +-++    |  
//     |     |  
//   +-+--+  |   
//   | +--+--+  
//   +----+    Copyright (c) 2009 Code Red Technologies Ltd. 
//
// UART example project for RDB1768 development board
//
// Software License Agreement
// 
// The software is owned by Code Red Technologies and/or its suppliers, and is 
// protected under applicable copyright laws.  All rights are reserved.  Any 
// use in violation of the foregoing restrictions may subject the user to criminal 
// sanctions under applicable laws, as well as to civil liability for the breach 
// of the terms and conditions of this license.
// 
// THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
// OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
// USE OF THIS SOFTWARE FOR COMMERCIAL DEVELOPMENT AND/OR EDUCATION IS SUBJECT
// TO A CURRENT END USER LICENSE AGREEMENT (COMMERCIAL OR EDUCATIONAL) WITH
// CODE RED TECHNOLOGIES LTD. 
//
//*****************************************************************************

#include "LPC17xx.h"
#include <string.h>

// PCUART0
#define PCUART0_POWERON (1 << 3)
#define PCUART1_POWERON (1 << 4)
#define PCUART2_POWERON (1 << 24)
#define PCLK_UART0 6
#define PCLK_UART0_MASK (3 << 6)
#define PCLK_UART1 (0 << 8)// PCLK_periph = 00: CCLK/4
#define PCLK_UART2 (0 << 16)// PCLK_periph = 00: CCLK/4
#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01


#define IIR_CTI		0x06
#define IIR_THRE	0x01


#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80



// ***********************
// Function to set up UART2 - MAX232
void UART2_Init(int baudrate) {
	int pclk;
	unsigned long int Fdiv;

	// PCLK_UART0 is being set to 1/4 of SystemCoreClock
	pclk = SystemCoreClock / 4;

	// Turn on power to UART2
	LPC_SC ->PCONP |= PCUART2_POWERON;
	// Turn on UART0 peripheral clock
	LPC_SC ->PCLKSEL1 |= PCLK_UART2;		// PCLK_periph = CCLK/4
//
//	// Set PINSEL0 so that P0.10 = TXD0, P0.11 = RXD0
	LPC_PINCON ->PINSEL0 |= ((1 << 20) | (1 << 22));
	LPC_UART2 ->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
	//Fdiv = (pclk / 16) / baudrate;	// Set baud rate
	//Fdiv = (pclk / 16) / 115200;	// Set baud rate
	Fdiv = (pclk / 16) / 9600;
	LPC_UART2 ->DLM = Fdiv / 256;
	LPC_UART2 ->DLL = Fdiv % 256;
	LPC_UART2 ->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
	LPC_UART2 ->FCR = 0x07;		// Enable and reset TX and RX FIFO

	NVIC_EnableIRQ(UART2_IRQn);
	LPC_UART2 ->IER = IER_RBR | IER_THRE | IER_RLS; /* Enable UART0 interrupt */
}

//Init UART 0 - SIM900
void UART0_Init() {
	int pclk;
	unsigned long int Fdiv;

	// PCLK_UART0 is being set to 1/4 of SystemCoreClock
	pclk = SystemCoreClock / 4;

	// Turn on power to UART0
	LPC_SC ->PCONP |= PCUART0_POWERON;

	// Turn on UART0 peripheral clock
	LPC_SC ->PCLKSEL0 &= ~(PCLK_UART0_MASK);
	LPC_SC ->PCLKSEL0 |= (0 << PCLK_UART0);		// PCLK_periph = CCLK/4

	// Set PINSEL0 so that P0.2 = TXD0, P0.3 = RXD0
	LPC_PINCON ->PINSEL0 &= ~0xf0;
	LPC_PINCON ->PINSEL0 |= ((1 << 4) | (1 << 6));

	LPC_UART0 ->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
	Fdiv = (pclk / 16) / 9600;	// Set baud rate
	LPC_UART0 ->DLM = Fdiv / 256;
	LPC_UART0 ->DLL = Fdiv % 256;
	LPC_UART0 ->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
	LPC_UART0 ->FCR = 0x07;		// Enable and reset TX and RX FIFO
	NVIC_EnableIRQ(UART0_IRQn);
	LPC_UART0 ->IER = IER_RBR | IER_THRE | IER_RLS; /* Enable UART0 interrupt */
}
//Init UART 1 GPS
void UART1_Init() {
	int pclk;
	unsigned long int Fdiv;

	// PCLK_UART0 is being set to 1/4 of SystemCoreClock
	pclk = SystemCoreClock / 4;

	// Turn on power to UART2
	LPC_SC ->PCONP |= PCUART1_POWERON;
	// Turn on UART1 peripheral clock
	LPC_SC ->PCLKSEL0 |= PCLK_UART1;		// PCLK_periph = CCLK/4
	//
	/* TxD1 is P0.15 and RxD1 is P0.16 */LPC_PINCON ->PINSEL0 |= (1 << 30);
	LPC_PINCON ->PINSEL1 |= (1 << 0);
	LPC_UART1 ->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
	Fdiv = (pclk / 16) / 9600;	// Set baud rate
	LPC_UART1 ->DLM = Fdiv / 256;
	LPC_UART1 ->DLL = Fdiv % 256;
	LPC_UART1 ->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
	LPC_UART1 ->FCR = 0x07;		// Enable and reset TX and RX FIFO
	NVIC_EnableIRQ(UART1_IRQn);
	LPC_UART1 ->IER = IER_RBR | IER_THRE | IER_RLS; /* Enable UART0 interrupt */
}
void UART2_Sendchar(char c) {
	while ((LPC_UART2 ->LSR & LSR_THRE) == 0)
		;	// Block until tx empty
	LPC_UART2 ->THR = c;
}





// ***********************
// Function to send character over UART

// ***********************
// Function to get character from UART
//char UART2_Getchar() {
//	char c;
//	while ((LPC_UART2 ->LSR & LSR_RDR) == 0)
//		;  // Nothing received so just block
//	c = LPC_UART2 ->RBR; // Read Receiver buffer register
//	return c;
//}


// ***********************
// Function to prints the string out over the UART
void UART2_PrintString(char *pcString) {
	int i = 0;
	// loop through until reach string's zero terminator
	while (pcString[i] != 0) {
		UART2_Sendchar(pcString[i]); // print each character
		i++;
	}
}
void UART0_Sendchar(char c) {
	while ((LPC_UART0->LSR & LSR_THRE) == 0);// Block until tx empty
	LPC_UART0 ->THR = c;
}
void UART0_PrintString(char *pcString) {
	int i = 0;
	// loop through until reach string's zero terminator
	while (pcString[i] != 0) {
		UART0_Sendchar(pcString[i]); // print each character
		i++;
	}
}
