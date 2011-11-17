#include <p30f3014.h>
#include "main.h"
#include <libpic30.h>

int pwm_freq;
int pwm_duty;

extern int tz;
extern int tz_dst;
extern int tz_start_weekday;
extern int tz_start_day_occurrence;
extern int tz_start_month;
extern int tz_start_hour;
extern int tz_end_weekday;
extern int tz_end_day_occurrence;
extern int tz_end_month;
extern int tz_end_hour;

void eeprom_first_run(void){
	if(read_eeprom(FIRSTRUN_ADDR) == 0xFFFF){
		write_eeprom(FIRSTRUN_ADDR, 0x0);
		write_eeprom(PWM_FREQ_ADDR, 0x007c);
		write_eeprom(PWM_DUTY_ADDR, 50);
		write_eeprom(TZ, 9);
		write_eeprom(TZ_DST, 10);
		write_eeprom(TZ_START_WEEKDAY, 0);
		write_eeprom(TZ_START_DAY_OCCURRENCE, 2);
		write_eeprom(TZ_START_MONTH, 3);
		write_eeprom(TZ_START_HOUR, 2);	
		write_eeprom(TZ_END_WEEKDAY, 0);
		write_eeprom(TZ_END_DAY_OCCURRENCE, 1);
		write_eeprom(TZ_END_MONTH, 10);
		write_eeprom(TZ_END_HOUR, 2);		
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
	tz= read_eeprom(TZ);
	tz_dst= read_eeprom(TZ_DST);
	tz_start_weekday= read_eeprom(TZ_START_WEEKDAY);
	tz_start_day_occurrence= read_eeprom(TZ_START_DAY_OCCURRENCE);
	tz_start_month= read_eeprom(TZ_START_MONTH);
	tz_start_hour= read_eeprom(TZ_START_HOUR);
	tz_end_weekday= read_eeprom(TZ_END_WEEKDAY);
	tz_end_day_occurrence= read_eeprom(TZ_END_DAY_OCCURRENCE);
	tz_end_month= read_eeprom(TZ_END_MONTH);
	tz_end_hour= read_eeprom(TZ_END_HOUR);
}
