#include <p30f3014.h>
#include "main.h"
#include <time.h>


extern time_t seconds;
extern time_t *timeptr;
unsigned int displaystate;  //Variable for the statemachine
void clockrefreshstart(void){
  T2CON = 0;           //Clear the settings of Timer1
  TMR2 = 0;            //Clear Timer1
  PR2 = (float)(0.000833/(1.0/(FCY)));
  IEC0bits.T2IE = 1;   //Ene Timer1's interrupt
  T2CONbits.TON = 1;  //Turn on Timer1abl
}



void Kselect(num){
  switch(num){
    case 0:
      K0 = 1;
      break;
    case 1:
      K1 = 1;
      break;    
    case 2:
      K2 = 1;
      break;
    case 3:
      K3 = 1;
      break;
    case 4:
      K4 = 1;
      break;
    case 5:
      K5 = 1;
      break;
    case 6:
      K6 = 1;
      break;
    case 7:
      K7 = 1;
      break;
    case 8:
      K8 = 1;
      break;
    case 9:
      K9 = 1;
      break;
  }
}

//unsigned long clockrefresh;
//unsigned long tmp;
void __attribute__((no_auto_psv))__attribute__((__interrupt__)) _T2Interrupt(void){
		/* This Function Refreshs the display */
		IFS0bits.T2IF = 0;
		PORTF = 0xFFFF;
		K0 = 0;
		K1 = 0;
		K2 = 0;
		K3 = 0;
		K4 = 0;
		K5 = 0;
		K6 = 0;
		K7 = 0;
		K8 = 0;
		K9 = 0;
		long int day = ((seconds%86400));
		Nop();
		switch(displaystate){
			case HourMSDstate: //Do the first hour Digit
				Kselect((day/3600)/10); //Set the Cathode
				A1 = 0;
				Nop();
				displaystate++; //Next time this statement executes do the next state
				break;
			case HourLSDstate:
				Kselect((day/3600)%10);
				A2 = 0;
				Nop();
				displaystate++;
				break;
			case MinuteMSDstate:			
				Kselect(((day%3600)/60)/10);
				A3 = 0;
				Nop();
				displaystate++;
				break;
			case MinuteLSDstate:
				Kselect(((day%3600)/60)%10);
				A4 = 0;
				Nop();
				displaystate++;
				break;
			case SecondMSDstate:
				Kselect((day%60)/10);
				A5 = 0;
				Nop();
				displaystate++;
				break;
			case SecondLSDstate:
				Kselect((day%60)%10);
				A6 = 0;
				Nop();
				displaystate = 0;
				break;
		}	
}
