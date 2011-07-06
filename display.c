#include <p30f3014.h>
#include "main.h"


extern time_s seconds;
extern time_s *timeptr;
unsigned int displaystate;  //Variable for the statemachine
int disp_state = 1;
extern long tzones[40];
extern int dst;
extern int tz_dst;
extern int tz;
int test = 1;
void clockrefreshstart(void){
	displaystate = 0;
	ADPCFG = 0xFFFF;
	TRISF = 0;
    TRISB = 0;
    TRISA = 0;
  T2CON = 0;           //Clear the settings of Timer1
  TMR2 = 0;            //Clear Timer1
  PR2 = (float)(0.000833/(1.0/(FCY)));
  IEC0bits.T2IE = 1;   //Ene Timer1's interrupt
  T2CONbits.TON = 1;  //Turn on Timer1
}



void Kselect(num){
  switch(num){
    case 0:
      PORTA = 0xFFFF;
      break;
    case 1:
      PORTB = PORTB|0b0000000000000001;
      break;    
    case 2:
      PORTB = PORTB|0b0000000000000010;
      break;
    case 3:
      PORTB = PORTB|0b0000000000000100;
      break;
    case 4:
      PORTB = PORTB|0b0000000000001000;
      break;
    case 5:
      PORTB = PORTB|0b0000000000010000;
      break;
    case 6:
      PORTB = PORTB|0b0000000000100000;
      break;
    case 7:
      PORTB = PORTB|0b0000000001000000;
      break;
    case 8:
      PORTB = PORTB|0b0000000010000000;
      break;
    case 9:
      PORTB = PORTB|0b0000000100000000;
      break;
  }
}

//unsigned long clockrefresh;
//unsigned long tmp;
void __attribute__((no_auto_psv))__attribute__((__interrupt__)) _T2Interrupt(void){
		/* This Function Refreshs the display */
		IFS0bits.T2IF = 0;
		if(disp_state == 0){
			time_s day = seconds%86400;
			display(day/3600,(day%3600)/60,day%60);
		}
		if(disp_state == 1){
			if(dst){
				time_s day = (seconds+tzones[tz_dst])%86400;
				display(day/3600,(day%3600)/60,day%60);
			}
			else{
				time_s day = (seconds+tzones[tz])%86400;
				display(day/3600,(day%3600)/60,day%60);
			}
		}
		if(disp_state == 2){
			/*struct tm *temptime;
			temptime = gmtime(&seconds);
			display(((*temptime).tm_year)%100,((*temptime).tm_mon)+1,(*temptime).tm_mday);*/
		}	
}

void display(int hours, int minutes, int second){
		PORTA = 0x0000;
		PORTB = 0b0001111000000000;
		PORTF = PORTF|0x3;
		//PORTF = temp;
		Nop();
		switch(displaystate){
			case HourMSDstate: //Do the first hour Digit
				if (test==1){
					Kselect(1); //Set the Cathode
				}
				else{
					Kselect(hours/10); //Set the Cathode
				}
				PORTF = PORTF&0b1111111111111101;
				Nop();
				displaystate++; //Next time this statement executes do the next state
				break;
			case HourLSDstate:
				if (test){
					Kselect(2); //Set the Cathode
				}
				else{
					Kselect(hours%10);
				}
				PORTF = PORTF&0b1111111111111110;
				Nop();
				displaystate++;
				break;
			case MinuteMSDstate:
				if (test){
					Kselect(3); //Set the Cathode
				}
				else{
					Kselect(minutes/10);
				}
				PORTB = PORTB&0b1110111111111111;
				Nop();
				displaystate++;
				break;
			case MinuteLSDstate:
				if (test){
					Kselect(4); //Set the Cathode
				}
				else{
					Kselect(minutes%10);
				}
				PORTB = PORTB&0b1111011111111111;
				Nop();
				displaystate++;
				break;
			case SecondMSDstate:
				if (test){
					Kselect(5); //Set the Cathode
				}
				else{
					Kselect(seconds/10);
				}
				PORTB=PORTB&0b1111101111111111;
				Nop();
				displaystate++;
				break;
			case SecondLSDstate:
				if (test){
					Kselect(6); //Set the Cathode
				}
				else{
					Kselect(seconds%10);
				}
				PORTB=PORTB&0b1111110111111111;
				Nop();
				displaystate = 0;
				break;
		}	
}
