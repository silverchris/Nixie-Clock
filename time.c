#include <p30f3014.h>
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


unsigned long bres;
time_s seconds; // Following the convention, this holds our seconds
const time_s *timeptr = &seconds;
extern int dst;
extern time_s dst_start;
extern time_s dst_end;

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
  	if((seconds%3600)==0){
	  	set_dst_start_end();
	}
	if(seconds>=dst_start){
		dst = 1;
	}
	if(seconds>=dst_end){
		dst = 0;
	}
  }
}

void time_commands(char *out_buf, char *modeptr1, char *inputptr){
//	puts(modeptr1);
	//uartbuffer("\nTime Commands\n");
	//uartbuffer(*modeptr1);
	//char temp[10];
	//strcpy(temp,modeptr1);
	if (strcmp(modeptr1,"get") == 0){
			char buf[24] = "";
			char *bufptr = &buf[0];
			datetime(bufptr,seconds);
			strcat(out_buf,"Time is: ");
			strcat(out_buf,buf);
		}
	if (strcmp(modeptr1,"set") == 0){
		//convert date from YYYY/MM/DD-HH:MM:SS into our time
		if(strlen(inputptr) != 19){
			strcpy(out_buf,"You have tried to set a invalid Date/Time\r\n");
		}
		else{
			char timestring[20];
			strcpy(timestring,inputptr);
			char *p;
			p = strtok(timestring,"/");
			
			int year = atoi(p);

			p = strtok(NULL, "/");
			int month = atoi(p);

			p = strtok(NULL, "-");
			int day = atoi(p);

			p = strtok(NULL, ":");
			int hour = atoi(p);

			p = strtok(NULL, ":");
			int minute = atoi(p);

			p = strtok(NULL, ":");
			int sec = atoi(p);
			char buf[24] = "";
			char *bufptr = &buf[0];
			seconds = mktime(year,month,day,hour,minute,sec);
			datetime(bufptr,seconds);
			strcat(out_buf,"Time Set to: ");
			strcat(out_buf,buf);
			set_dst_start_end();
		}
	}
}
