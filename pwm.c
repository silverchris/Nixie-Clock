#include <p30f3014.h>
#include <outcompare.h>
#include <timer.h>
#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int pwm_freq;
extern int pwm_duty;
void enable_PWM(void){
	int duty = ((pwm_duty/100.0)*pwm_freq)+0.5;
	#define timer_config T3_ON&T2_IDLE_CON&T3_GATE_OFF&T3_PS_1_1&T3_SOURCE_INT
	OpenTimer3(timer_config, pwm_freq);
	//EnableIntT3;
	#define OC_Config OC_PWM_FAULT_PIN_DISABLE&OC_IDLE_CON&OC_TIMER3_SRC
	OpenOC1(OC_Config,duty,duty);
}

void __attribute__((no_auto_psv))__attribute__((__interrupt__)) _T3Interrupt(void){
	IFS0bits.T3IF = 0;
}

void duty_cycle(char *out_buf, char *modeptr1, char *inputptr){
	if (strcmp(modeptr1,"get") == 0){
		//int duty = ((float)pwm_duty/pwm_freq)*100;
		int duty = ((OC1R/(float)pwm_freq)*100.0)+0.5;
		sprintf(out_buf,"Current Set Duty Cycle: %d%%\r\nCurrent Actual Duty Cycle %i%%",pwm_duty,duty);
	}
	if (strcmp(modeptr1,"set") == 0){
		int duty = atoi(inputptr);
		if((duty > 99)|(duty < 1)){
 			strcpy(out_buf,"You have tried to set a invalid duty cycle value(Must be between 1 and 99)");
		}
		else{
			pwm_duty = duty;
			write_eeprom(PWM_DUTY_ADDR, duty);
			CloseOC1();
			enable_PWM();
			duty = ((OC1R/(float)pwm_freq)*100.0)+0.5;
			sprintf(out_buf,"Duty Cycle set to: %d%%\n",duty);
		}
	}
}

void pwm_freq_set(char *out_buf, char *modeptr1, char *inputptr){
	if (strcmp(modeptr1,"get") == 0){
		int duty = ((OC1R/(float)pwm_freq)*100.0)+0.5;
		float tcy = 1.0/(FCY);
		long int frequency; 
		frequency = 1.0/((pwm_freq+1.0)*tcy);
		sprintf(out_buf,"Current PWM Frequency: %ldHz\r\nDuty Cycle is:%i%%",frequency,duty);
	}
	if (strcmp(modeptr1,"set") == 0){
		float tcy = 1.0/(FCY);
		long int frequency = atol(inputptr);
		pwm_freq = (((1.0/frequency)/tcy)-1)+.5;//calculate PRy Register Value for PWM, at .5 to cause it to round correctly 
		write_eeprom(PWM_FREQ_ADDR, pwm_freq);
		CloseOC1();
		enable_PWM();
		int duty = ((OC1R/(float)pwm_freq)*100.0)+0.5;
		frequency = 1.0/((pwm_freq+1.0)*tcy);
		sprintf(out_buf,"PWM Frequency set to: %ldHz\r\nDuty Cycle is:%i%%",frequency,duty);
	}
}

