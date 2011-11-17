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
extern int disp_hours;
extern int disp_minutes;
extern int disp_seconds;
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

const int Cathodes_A[10] = { 0xFFFF, 0x0000, 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};
const int Cathodes_B[10] = { 0b0000000000000000, 0b0000000000000001, 0b0000000000000010,0b0000000000000100,0b0000000000001000,0b0000000000010000,0b0000000000100000,0b0000000001000000,0b0000000010000000,0b0000000100000000};

void Kselect(num){
	PORTA = Cathodes_A[num];
	PORTB = PORTB|Cathodes_B[num];
}

void __attribute__((no_auto_psv))__attribute__((__interrupt__)) _T2Interrupt(void){
		/* This Function Refreshs the display */
		IFS0bits.T2IF = 0;
		display(disp_hours,disp_minutes,disp_seconds);
}

const int Anodes_F[6] = {0b1111111111111101,0b1111111111111110,0b1111111111111111,0b1111111111111111,0b1111111111111111,0b1111111111111111};
const int Anodes_B[6] = {0b1111111111111111,0b1111111111111111,0b1110111111111111,0b1111011111111111,0b1111101111111111,0b1111110111111111};


void display(int hour, int minute, int second){
	PORTB = 0b0001111000000000;
	PORTF = PORTF|0x3;
	Nop();
	PORTF = PORTF&Anodes_F[displaystate];
	PORTB = PORTB&Anodes_B[displaystate];
	if(test==1){
		hour = 12;
		minute = 34;
		second = 56;
	}
	switch(displaystate){
		case HourMSDstate: //Do the first hour Digit
			Kselect(hour/10); //Set the Cathode
			displaystate++; //Next time this statement executes do the next state
			break;
		case HourLSDstate:
			Kselect(hour%10);
			Nop();
			displaystate++;
			break;
		case MinuteMSDstate:
			Kselect(minute/10);
			Nop();
			displaystate++;
			break;
		case MinuteLSDstate:
			Kselect(minute%10);
			Nop();
			displaystate++;
			break;
		case SecondMSDstate:
			Kselect(second/10);
			Nop();
			displaystate++;
			break;
		case SecondLSDstate:
			Kselect(second%10);
			Nop();
			displaystate = 0;
			break;
	}
}
