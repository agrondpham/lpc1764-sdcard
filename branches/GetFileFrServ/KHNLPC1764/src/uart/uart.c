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
////////////////////start

#include "LPC17xx.h"
#include "../common.h"
// PCUART0
#define PCUART0_POWERON (1 << 3)
#define PCUART1_POWERON (1 << 4)
#define PCUART2_POWERON (1 << 24)
#define PCUART3_POWERON (1 << 25)

#define PCLK_UART0 6
#define PCLK_UART1 8
#define PCLK_UART2 16
#define PCLK_UART3 18
#define PCLK_UART0_MASK (3 << 6)
#define PCLK_UART1_MASK (3 << 8)
#define PCLK_UART2_MASK (3 << 16)
#define PCLK_UART3_MASK (3 << 18)

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

///////////end
///start unit uart

/// uart 0
void UART0_Init(int baudrate) {
	int pclk;
	unsigned long int Fdiv;

	// PCLK_UART0 is being set to 1/4 of SystemCoreClock
	pclk = SystemCoreClock / 4;

	// Turn on power to UART0
	LPC_SC->PCONP |= PCUART0_POWERON;

	// Turn on UART0 peripheral clock
	LPC_SC->PCLKSEL0 &= ~(PCLK_UART0_MASK);
	LPC_SC->PCLKSEL0 |= (0 << PCLK_UART0);		// PCLK_periph = CCLK/4

	// Set PINSEL0 so that P0.2 = TXD0, P0.3 = RXD0
	LPC_PINCON->PINSEL0 &= ~0xf0;
	LPC_PINCON->PINSEL0 |= ((1 << 4) | (1 << 6));

	LPC_UART0->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
	Fdiv = (pclk / 16) / baudrate;	// Set baud rate
	LPC_UART0->DLM = Fdiv / 256;
	LPC_UART0->DLL = Fdiv % 256;
	LPC_UART0->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
	LPC_UART0->FCR = 0x07;		// Enable and reset TX and RX FIFO

	NVIC_EnableIRQ(UART0_IRQn);
	LPC_UART0->IER = IER_RBR | IER_THRE | IER_RLS; /* Enable UART0 interrupt */
}
/// uart 1
void UART1_Init(int baudrate) {
	int pclk;
	unsigned long int Fdiv;

	// PCLK_UART0 is being set to 1/4 of SystemCoreClock
	pclk = SystemCoreClock / 4;

	// Turn on power to UART0
	LPC_SC->PCONP |= PCUART1_POWERON;

	// Turn on UART0 peripheral clock
	LPC_SC->PCLKSEL0 &= ~(PCLK_UART1_MASK);
	LPC_SC->PCLKSEL0 |= (0 << PCLK_UART1);		// PCLK_periph = CCLK/4

	// Set PINSEL0 so that P0.2 = TXD0, P0.3 = RXD0
	LPC_PINCON->PINSEL0 &= ~0xc0000000;
	LPC_PINCON->PINSEL0 |= (1 << 30);
	LPC_PINCON->PINSEL1 &= ~0xc;
	LPC_PINCON->PINSEL1 |= (1 << 0);

	LPC_UART1->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
	Fdiv = (pclk / 16) / baudrate;	// Set baud rate
	LPC_UART1->DLM = Fdiv / 256;
	LPC_UART1->DLL = Fdiv % 256;
	LPC_UART1->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
	LPC_UART1->FCR = 0x07;		// Enable and reset TX and RX FIFO
	NVIC_EnableIRQ(UART1_IRQn);
	LPC_UART1->IER = IER_RBR | IER_THRE | IER_RLS; /* Enable UART0 interrupt */
}
// uart 2
void UART2_Init(int baudrate) {
	int pclk;
	unsigned long int Fdiv;

	// PCLK_UART2 is being set to 1/4 of SystemCoreClock
	pclk = SystemCoreClock / 4;

	// Turn on power to UART2
	LPC_SC->PCONP |= PCUART2_POWERON;

	// Turn on UART2 peripheral clock
	LPC_SC->PCLKSEL1 &= ~(PCLK_UART2_MASK);
	LPC_SC->PCLKSEL1 |= (0 << PCLK_UART2);		// PCLK_periph = CCLK/4

	// Set PINSEL0 so that P0.10 = TXD2, P0.11 = RXD2
	LPC_PINCON->PINSEL0 &= ~0xf00000;
	LPC_PINCON->PINSEL0 |= ((1 << 20) | (1 << 22));

	LPC_UART2->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
	Fdiv = (pclk / 16) / baudrate;	// Set baud rate
	LPC_UART2->DLM = Fdiv / 256;
	LPC_UART2->DLL = Fdiv % 256;
	LPC_UART2->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
	LPC_UART2->FCR = 0x07;		// Enable and reset TX and RX FIFO
	NVIC_EnableIRQ(UART2_IRQn);
	LPC_UART2->IER = IER_RBR | IER_THRE | IER_RLS; /* Enable UART0 interrupt */
}
//uart 3

void UART3_Init(int baudrate) {
	int pclk;
	unsigned long int Fdiv;

	// PCLK_UART2 is being set to 1/4 of SystemCoreClock
	pclk = SystemCoreClock / 4;

	// Turn on power to UART2
	LPC_SC->PCONP |= PCUART3_POWERON;

	// Turn on UART2 peripheral clock
	LPC_SC->PCLKSEL1 &= ~(PCLK_UART3_MASK);
	LPC_SC->PCLKSEL1 |= (0 << PCLK_UART3);		// PCLK_periph = CCLK/4

	// Set PINSEL0 so that P0.0 = TXD3, P0.1 = RXD3
//	LPC_PINCON->PINSEL0 &= ~0xf;
//	LPC_PINCON->PINSEL0 |= ((1 << 1) | (1 << 3));
	// Set PINSEL9 so that P4.28 = TXD3, P4.29 = RXD3
	LPC_PINCON->PINSEL9 &= ~0xf;
	//LPC_PINCON ->PINSEL9 |= ((3 << 24) | (3 << 26));
	LPC_PINCON->PINSEL9 |= ((1 << 24) | (1 << 25) | (1 << 26) | (1 << 27));

	LPC_UART3->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
	Fdiv = (pclk / 16) / baudrate;	// Set baud rate
	LPC_UART3->DLM = Fdiv / 256;
	LPC_UART3->DLL = Fdiv % 256;
	LPC_UART3->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
	LPC_UART3->FCR = 0x07;		// Enable and reset TX and RX FIFO
}
/////////end

// PCUART0
/*
 #define PCUART0_POWERON (1 << 3)
 #define PCUART1_POWERON (1 << 4)
 #define PCUART2_POWERON (1 << 24)
 #define PCUART3_POWERON (1 << 25)

 #define PCLK_UART0 6
 #define PCLK_UART0_MASK (3 << 6)
 #define PCLK_UART1 (0 << 8)// PCLK_periph = 00: CCLK/4
 #define PCLK_UART2 (0 << 16)// PCLK_periph = 00: CCLK/4
 #define PCLK_UART3 (0 << 18)// PCLK_periph = 00: CCLK/4
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
 */
// ***********************
// Function to set up UART2 - MAX232
///RF
//void UART3_Init(int baudrate) {
//	int pclk;
//	unsigned long int Fdiv;
//
//	// PCLK_UART0 is being set to 1/4 of SystemCoreClock
//	pclk = SystemCoreClock / 4;
//
//	// Turn on power to UART2
//	LPC_SC ->PCONP |= PCUART3_POWERON;
//	// Turn on UART0 peripheral clock
//	LPC_SC ->PCLKSEL1 |= PCLK_UART3;		// PCLK_periph = CCLK/4
////
////	// Set PINSEL0 so that P4.28 = TXD3, P4.29 = RXD3
//	//LPC_PINCON ->PINSEL9 |= ((3 << 24) | (3 << 26));
//	//	// Set PINSEL0 so that P0.0 = TXD3, P0.1 = RXD3
//	LPC_PINCON->PINSEL0 |= ((1 << 1) | (1 << 3));
//
//	LPC_UART3 ->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
//	//Fdiv = (pclk / 16) / baudrate;	// Set baud rate
//	//Fdiv = (pclk / 16) / 115200;	// Set baud rate
//	Fdiv = (pclk / 16) / 9600;
//	LPC_UART3 ->DLM = Fdiv / 256;
//	LPC_UART3 ->DLL = Fdiv % 256;
//	LPC_UART3 ->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
//	LPC_UART3 ->FCR = 0x07;		// Enable and reset TX and RX FIFO
//
//	NVIC_EnableIRQ(UART3_IRQn);
//	LPC_UART3 ->IER = IER_RBR | IER_THRE | IER_RLS; /* Enable UART0 interrupt */
//}
//END
//
//void UART2_Init(int baudrate) {
//	int pclk;
//	unsigned long int Fdiv;
//
//	// PCLK_UART0 is being set to 1/4 of SystemCoreClock
//	pclk = SystemCoreClock / 4;
//
//	// Turn on power to UART2
//	LPC_SC ->PCONP |= PCUART2_POWERON;
//	// Turn on UART0 peripheral clock
//	LPC_SC ->PCLKSEL1 |= PCLK_UART2;		// PCLK_periph = CCLK/4
////
////	// Set PINSEL0 so that P0.10 = TXD0, P0.11 = RXD0
//	LPC_PINCON ->PINSEL0 |= ((1 << 20) | (1 << 22));
//	LPC_UART2 ->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
//	//Fdiv = (pclk / 16) / baudrate;	// Set baud rate
//	//Fdiv = (pclk / 16) / 115200;	// Set baud rate
//	Fdiv = (pclk / 16) / 9600;
//	LPC_UART2 ->DLM = Fdiv / 256;
//	LPC_UART2 ->DLL = Fdiv % 256;
//	LPC_UART2 ->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
//	LPC_UART2 ->FCR = 0x07;		// Enable and reset TX and RX FIFO
//
//	NVIC_EnableIRQ(UART2_IRQn);
//	LPC_UART2 ->IER = IER_RBR | IER_THRE | IER_RLS; /* Enable UART0 interrupt */
//}
//Init UART 0 - SIM900
/*
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
//}
//
////Init UART 1 GPS
//void UART1_Init() {
//	int pclk;
//	unsigned long int Fdiv;
//
//	// PCLK_UART0 is being set to 1/4 of SystemCoreClock
//	pclk = SystemCoreClock / 4;
//
//	// Turn on power to UART2
//	LPC_SC ->PCONP |= PCUART1_POWERON;
//	// Turn on UART1 peripheral clock
//	LPC_SC ->PCLKSEL0 |= PCLK_UART1;		// PCLK_periph = CCLK/4
//	//
//	/* TxD1 is P0.15 and RxD1 is P0.16 */LPC_PINCON ->PINSEL0 |= (1 << 30);
//	LPC_PINCON ->PINSEL1 |= (1 << 0);
//	LPC_UART1 ->LCR = 0x83;		// 8 bits, no Parity, 1 Stop bit, DLAB=1
//	Fdiv = (pclk / 16) / 9600;	// Set baud rate
//	LPC_UART1 ->DLM = Fdiv / 256;
//	LPC_UART1 ->DLL = Fdiv % 256;
//	LPC_UART1 ->LCR = 0x03;		// 8 bits, no Parity, 1 Stop bit DLAB = 0
//	LPC_UART1 ->FCR = 0x07;		// Enable and reset TX and RX FIFO
//	NVIC_EnableIRQ(UART1_IRQn);
//	LPC_UART1 ->IER = IER_RBR | IER_THRE | IER_RLS; /* Enable UART0 interrupt */
//}
void UART3_Sendchar(char c) {
	while ((LPC_UART3->LSR & LSR_THRE) == 0)
		;	// Block until tx empty
	LPC_UART3->THR = c;
}
void UART3_PrintString(char *pcString) {
	int i = 0;
	// loop through until reach string's zero terminator
	while (pcString[i] != 0) {
		UART3_Sendchar(pcString[i]); // print each character
		i++;
	}
}

// ***********************
// Function to send character over UART

// ***********************
// Function to get character from UART
//char UART3_Getchar() {
//	char c;
//	while ((LPC_UART3 ->LSR & LSR_RDR) == 0)
//		;  // Nothing received so just block
//	c = LPC_UART3 ->RBR; // Read Receiver buffer register
//	return c;
//}

// ***********************
// Function to prints the string out over the UART

void UART2_Sendchar(char c) {
	while ((LPC_UART2->LSR & LSR_THRE) == 0)
		;	// Block until tx empty
	LPC_UART2->THR = c;
}

void UART2_PrintString(char *pcString) {
	int i = 0;
	// loop through until reach string's zero terminator
	while (pcString[i] != 0) {
		UART2_Sendchar(pcString[i]); // print each character
		i++;
	}
}
void UART0_Sendchar(char c) {
	while ((LPC_UART0->LSR & LSR_THRE) == 0)
		; // Block until tx empty
	LPC_UART0->THR = c;
}
void UART0_PrintString(char *pcString) {
	int i = 0;
	// loop through until reach string's zero terminator
	while (pcString[i] != 0) {
		UART0_Sendchar(pcString[i]); // print each character
		i++;
	}
}
uint8_t UART0_GetChar(void) {
	while (!(LPC_UART0->LSR & 0x01))
		;
	return (LPC_UART0->RBR);
}
int sendATcommandReadData(char* ATcommand, char* expected_answer) {
	uint8_t firmwareData[1024] = "";
	char dataReturnFromUart[50] = "";
	char buf;
	char buf2[5];
	int x = 0;
	int y = 0;
	int current;
	int currentData;
	int check;
	uint8_t buf_recv;
	UART0_PrintString(ATcommand);
	UART0_PrintString("\r\n");

#if _DEBUG==1
	UART2_PrintString(ATcommand);
	UART2_PrintString("\n\r");
#endif
	while (x == 0) {
		current = UART0_GetChar();
		if (current == 0x0D && UART0_GetChar() == 0x0A) {
			if (UART0_GetChar() == 0x0D && UART0_GetChar() == 0x0A) {
				while (x == 0) {
					check = UART0_GetChar();
					if (check == 0x0D) {
						x = 1;
					} else {
						buf = check;

						strncat(dataReturnFromUart, &buf, 1);
					}

				}

			}

		}
	}

#if _DEBUG==1
	UART2_PrintString(dataReturnFromUart);
	UART2_PrintString("\n\r");
#endif
	if (strstr(dataReturnFromUart, "+FTPGET:2,0") != NULL) {
		GPIOSetValue(3,25, 1);
		delay_ms(50);
		GPIOSetValue(3,25, 0);
		return 0;
	} else if (strstr(dataReturnFromUart, expected_answer) != NULL) {

		buf_recv = UART0_GetChar();
		x=0;
		while (y == 0) {

			buf_recv = UART0_GetChar();
			if (buf_recv == 0x4F) {
				y = 1;
			} else {
//				sprintf(buf2, "%d", currentData);
//				UART2_PrintString(buf2);
				firmwareData[x]=buf_recv;
				//strncat(firmwareData, &buf, 1);
				//UART2_Sendchar(buf);
				             ++x;
			}
		}
		firmwareData[--x]='\0';
		firmwareData[--x]='\0';
		if (WriteFirmwareFile(firmwareData) == 1) {
			return 1;
		}
		return 0;
	} else {
		UART2_PrintString("NO exist");
		return 0;
	}
}

int UART0_ReadDataUpdateFirmware() {
	char dataReturnFromUart[1024] = "";
	char ddd[50];
	char buf;
	int x = 0;
	int current;
	int check;
	while (1) {
		current = UART0_GetChar();
		sprintf(ddd, "%d\n\r", current);
		UART2_PrintString(ddd);

	}
//	while (x == 0) {
//		current = UART0_GetChar();
////		sprintf(ddd,"%d\n\r",current);
////		UART2_PrintString(ddd);
//		if (UART0_GetChar() == 0x0A) {
//
//			while (x == 0) {
//				check = UART0_GetChar();
//				if (check == 0x0D) {
//
//					x = 1;
//				} else {
//					buf = check;
//							sprintf(ddd,"%d",check);
//							UART2_PrintString(ddd);
//					strncat(dataReturnFromUart, &buf, 1);
//				}
//
//			}
//
//		}
//
//	}

#if _DEBUG==1
	UART2_PrintString(dataReturnFromUart);
	UART2_PrintString("\n\r");
#endif
	return 0;
}
int UART0_CompareData(char* expected_answer) {
	char dataReturnFromUart[50] = "";
	char buf;
	int x = 0;
	int current;
	int check;
	while (x == 0) {
		current = UART0_GetChar();
		if (current == 0x0D && UART0_GetChar() == 0x0A) {
			if (UART0_GetChar() == 0x0D && UART0_GetChar() == 0x0A) {
				while (x == 0) {
					check = UART0_GetChar();
					if (check == 0x0D) {
						x = 1;
					} else {
						buf = check;

						strncat(dataReturnFromUart, &buf, 1);
					}

				}

			}

		}
	}
#if _DEBUG==1
	UART2_PrintString(dataReturnFromUart);
	UART2_PrintString("\n\r");
#endif
	if (strstr(dataReturnFromUart, expected_answer) != NULL) {
		//UART2_PrintString("exist");
		return 1;
	} else {
		//UART2_PrintString("NO exist");
		return 0;
	}
}
int sendATcommand(char* ATcommand, char* expected_answer) {
	//char expectedData[20]="OK";
	char dataReturnFromUart[50] = "";
	char buf;
	int x = 0;
	int current;
	int check;

	UART0_PrintString(ATcommand);
	UART0_PrintString("\r\n");

#if _DEBUG==1
	UART2_PrintString(ATcommand);
	UART2_PrintString("\n\r");
#endif
	while (x == 0) {
		current = UART0_GetChar();
		if (current == 0x0D && UART0_GetChar() == 0x0A) {
			if (UART0_GetChar() == 0x0D && UART0_GetChar() == 0x0A) {
				while (x == 0) {
					check = UART0_GetChar();
					if (check == 0x0D) {
						x = 1;
					} else {
						buf= check;
						strncat(dataReturnFromUart, &buf, 1);
					}

				}

			}

		}
	}
#if _DEBUG==1
	UART2_PrintString(dataReturnFromUart);
	UART2_PrintString("\n\r");
#endif
	if (strstr(dataReturnFromUart, expected_answer) != NULL) {
		//UART2_PrintString("exist");
		return 1;
	} else {
		//UART2_PrintString("NO exist");
		return 0;
	}
}

//int8_t sendATcommand(char* ATcommand, char* expected_answer,
//		unsigned int timeout) {
//
//	uint8_t x = 0, answer = 0;
//	char response[100];
//	unsigned long previous;
//
//	memset(response, '\0', 100);    // Initialize the string
//
//	delay(100);
//
////while( Serial.available() > 0) Serial.read();    // Clean the input buffer
//
//	UART0_PrintString(ATcommand);    // Send the AT command
//
//	x = 0;
//	previous = millis();
//
//// this loop waits for the answer
//	do {
//		//if(Serial.available() != 0){
//		// if there are data in the UART input buffer, reads it and checks for the asnwer
//		response[x] = 'a';            // Serial.read();
//		//Serial.print(response[x]);
//		x++;
//		// check if the desired answer  is in the response of the module
//		if (strstr(response, expected_answer) != NULL) {
//			answer = 1;
//		}
//		// }
//	}
//// Waits for the asnwer with time out
//	while ((answer == 0) && ((millis() - previous) < timeout));
//
//	return answer;
//}
