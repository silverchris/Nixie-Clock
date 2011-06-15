#include <p30f3014.h>
#include "main.h"
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


unsigned long bres;
time_t seconds; // Following the convention, this holds our seconds
time_t *timeptr = &seconds;

void clockstart(void){
  T1CON = 0;           //Clear the settings of Timer1
  TMR1 = 0;            //Clear Timer1
  PR1 = 65535;      //Set Timer1 to reset at external oscillator divded by 4 so it counts one second
  IEC0bits.T1IE = 1;   //Enable Timer1's interrupt
  T1CONbits.TON = 1;  //Turn on Timer1
}

void clockstop(void){
  IEC0bits.T1IE = 0;   //Enable Timer1's interrupt
  T1CONbits.TON = 0;  //Turn on Timer1
}
	
void __attribute__((no_auto_psv))__attribute__((__interrupt__)) _T1Interrupt(void){
  IFS0bits.T1IF = 0;
  bres += 65535;    // add 65535 ticks to bresenham total
  if(bres >= FCY){     // if reached 1 second!
    bres -= FCY;        // subtract 1 second, retain error
    seconds++;
  }
}

void time_commands(char *out_buf, char *modeptr1, char *inputptr){
//	puts(modeptr1);
	//uartbuffer("\nTime Commands\n");
	//uartbuffer(*modeptr1);
	//char temp[10];
	//strcpy(temp,modeptr1);
	if (strcmp(modeptr1,"get") == 0){
		strftime(out_buf,20,"Time is %H:%M:%S\r\n",gmtime(timeptr));
		}
	if (strcmp(modeptr1,"set") == 0){
		//convert date from YYYY/MM/DD-HH:MM:SS into unix time
		if(strlen(inputptr) != 19){
			strcpy(out_buf,"You have tried to set a invalid Date/Time\r\n");
		}
		else{
			char timestring[20];
			strcpy(timestring,inputptr);
			struct tm temptime;
			char *p;
			p = strtok(timestring,"/");
			temptime.tm_year = (int)(atol(p)-(long)1900);

			p = strtok(NULL, "/");
			temptime.tm_mon = atoi(p)-1;

			p = strtok(NULL, "-");
			temptime.tm_mday = atoi(p);

			p = strtok(NULL, ":");
			temptime.tm_hour = atoi(p);

			p = strtok(NULL, ":");
			temptime.tm_min = atoi(p);

			p = strtok(NULL, ":");
			temptime.tm_sec = atoi(p);
			seconds = mktime(&temptime);
			strftime(out_buf,40,"Time Set To %Y/%m/%d %H:%M:%S\r\n",&temptime);
		}
	}
}
