#include <p30f3014.h>
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <UART.h>

char uarttextbuffer[200];
char uarttempbuffer[200];
char *uartbuffptr;
char uartrxbuffer[60];
int running_command;

void enable_uart(void){
	uartbuffptr = &uarttextbuffer[0];
	#define UMODEvalue UART_EN & UART_IDLE_CON &\
    UART_DIS_WAKE & UART_ALTRX_ALTTX &\
    UART_DIS_ABAUD & UART_NO_PAR_8BIT &\
    UART_1STOPBIT & UART_DIS_LOOPBACK
	#define USTAvalue UART_INT_TX_BUF_EMPTY &\
	UART_TX_PIN_NORMAL &\
	UART_TX_ENABLE &\
	UART_INT_RX_CHAR &\
	UART_ADR_DETECT_DIS &\
	UART_RX_OVERRUN_CLEAR
	OpenUART1(UMODEvalue,USTAvalue,BRG);
	_U1TXIE = 1;
	_U1RXIE = 1;
}

void __attribute__((no_auto_psv))__attribute__((__interrupt__)) _U1TXInterrupt(void){
  /*Uses a Circilic buffer to sent out the data stored in the uart buffer. Makes sure that the uart buffer in 
  software should never be full(I hope)*/
  char transmit_char;
  IFS0bits.U1TXIF = 0;
  while((strlen(uarttextbuffer) > 0)&& !U1STAbits.UTXBF){
    transmit_char = *uartbuffptr;
    char * uarttempbuffer = (char *)uarttextbuffer + 1;
    strcpy(uarttextbuffer, uarttempbuffer);
	U1TXREG = transmit_char;
  }
}

void __attribute__((no_auto_psv))__attribute__((__interrupt__)) _U1RXInterrupt(void){
  IFS0bits.U1RXIF = 0;
  while((U1STA&0x1) == 1){
	    char received_char = U1RXREG;
		if(running_command == 0){
			if (received_char == 0x0D){
				uartbuffer("\n");
				running_command = 1;
				execute_command(&uartrxbuffer[0]);
				int count = strlen(uartrxbuffer);
				int i;
				for(i=0; i <= count;i++){
					uartrxbuffer[i] = '\0';
				}
				running_command = 0;
				uartbuffer("\r\n>");
			}
			else{
				uartrxbuffer[strlen(uartrxbuffer)] = received_char;
				char string[2];
				string[0] = received_char;
				string[1] = '\0';
				uartbuffer(string);
			}
		}
  }
}

void uartbuffer(char string[120]){
	//Appends a charecter or string of characters to the software uartbuffer.
	if(string){
		strcat(uarttextbuffer, string);
	}
	if(U1STAbits.TRMT == 1){
		U1TXInterrupt();
	}
}

void U1TXInterrupt(void){
	_U1TXInterrupt();
}

