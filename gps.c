#include <p30f3014.h>
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <UART.h>
#include <time.h>
#include <stdlib.h>

char gpsrxbuffer[81];
char *gpsrxptr = &gpsrxbuffer[0];
extern time_t seconds;
extern time_t *timeptr;
void enable_gps(void){
	*gpsrxptr = '\0';
	#define UMODEvalue UART_EN & UART_IDLE_CON &\
    UART_DIS_WAKE &\
    UART_DIS_ABAUD & UART_NO_PAR_8BIT &\
    UART_1STOPBIT & UART_DIS_LOOPBACK
	#define USTAvalue UART_INT_TX_BUF_EMPTY &\
	UART_TX_PIN_NORMAL &\
	UART_TX_ENABLE &\
	UART_INT_RX_CHAR &\
	UART_ADR_DETECT_DIS &\
	UART_RX_OVERRUN_CLEAR
	OpenUART2(UMODEvalue,USTAvalue,BRG2);
	_U2RXIE = 1;
}

void __attribute__((no_auto_psv))__attribute__((__interrupt__)) _U2RXInterrupt(void){
  IFS1bits.U2RXIF = 0;
  while((U2STA&0x1) == 1){
  	    char received_char = U2RXREG;
		if (received_char == 0x0A){
			char message[strlen(gpsrxbuffer)];
			strcpy(message,gpsrxbuffer);
			gpsrxptr = &gpsrxbuffer[0];
			if (!strncmp(message,"$GPGGA",6)){
				char hour[3];
				char *timeptr = &hour[0];
				int i;
				for(i=7;i < 9; i++){
				 *timeptr = message[i];
				 timeptr++;
				}
				*timeptr = '\0';
				char minutes[3];
				timeptr = &minutes[0];
				for(i=9;i < 11; i++){
				 *timeptr = message[i];
				 timeptr++;
				}
				*timeptr = '\0';
				char secs[3];
				timeptr = &secs[0];
				for(i=11;i <13 ; i++){
				 *timeptr = message[i];
				 timeptr++;
				}
				*timeptr = '\0';
				struct tm *temptime;
				temptime = gmtime(&seconds);
				(*temptime).tm_hour = atoi(hour);
				(*temptime).tm_min = atoi(minutes);
				(*temptime).tm_sec = atoi(secs);
				seconds = mktime(temptime);
			}
		}
		else{
			*gpsrxptr = received_char;
			gpsrxptr++;
			*gpsrxptr = '\0';
		}
  }
}
