#include <p30f3014.h>
#include "main.h"
#include <libpic30.h>
int pwm_freq;
int pwm_duty;

void eeprom_first_run(void){
	if(read_eeprom(FIRSTRUN_ADDR) == 0xFFFF){
		write_eeprom(FIRSTRUN_ADDR, 0x0);
		write_eeprom(PWM_FREQ_ADDR, 0x007c);
		write_eeprom(PWM_DUTY_ADDR, 50);
		load_eeprom_data();
	}
	else{
		load_eeprom_data();
	}
}
		
void write_eeprom(_prog_addressT address, int value){
	_erase_eedata(address, _EE_WORD);
    _wait_eedata();
	_write_eedata_word(address,value);
    _wait_eedata();
}

int read_eeprom(_prog_addressT address){
	int buf[1];
	_memcpy_p2d16(buf,address,_EE_WORD);
	Nop();
	return buf[0];
}

void load_eeprom_data(void){
	pwm_freq = read_eeprom(PWM_FREQ_ADDR);
	pwm_duty = read_eeprom(PWM_DUTY_ADDR);
}
