#include <p30f3014.h>
#include "main.h"
#include <stdio.h>
#include <stdlib.h>

extern time_s seconds;
int dst = 0;
int tz=15;
int tz_dst=15;
int tz_start_weekday=0;
int tz_start_day_occurrence=0;
int tz_start_month=0;
int tz_start_hour=0;
int tz_end_weekday=0;
int tz_end_day_occurrence=0;
int tz_end_month=0;
int tz_end_hour=0;
time_s dst_start = 0;
time_s dst_end = 0;

long tzones[] = {-43200,-39600,-36000,-34200, //-12:00,-11:00,-10:00,-9:30
		-32400,-28800,-25200,-21600, //-9:00,-8:00,-7:00,-6:00
		-18000,-16200,-14400,-12600, //-5:00,-4:30,-4:00,-3:30
		-10800,-7200,-3600,0, //-3:00,-2:00,-1:00,0:00
		3600,7200,10800,12600, //1:00,2:00,3:00,3:30
		14400,16200,18000,19800, //4:00,4:30,5:00,5:30
		20700,21600,23400,25200, //5:45,6:00,6:30,7:00
		28800,31500,32400,34200, //8:00,8:45,9:00,9:30
		36000,37800,39600,41400, //10:00,10:30,11:00,11:30
		43200,45900,46800,50400}; //12:00,12:45,13:00,14:00
	
time_s calc_dst_date(int year,int month,int week_day,int inc, int hour){
	int day = dayOfweek(year,month,1)+1;
	int first_day=0;
	if (day > week_day){
		int diff = week_day-day;
		first_day = (7+diff)+1;
	}
	else if (day < week_day){
		first_day = (week_day-day)+1;
	} 
	else if (day == week_day){
		first_day = 1;
	}
	return mktime(year,month,first_day+(7*(inc-1)),hour,0,0);
}

void set_dst_start_end(void){
	int days = day(seconds);
	int years = 2000+year(days);
	dst_start = calc_dst_date(years,tz_start_month,tz_start_weekday,tz_start_day_occurrence,tz_start_hour);
	dst_end = calc_dst_date(years,tz_end_month,tz_end_weekday,tz_end_day_occurrence,tz_end_hour);
}
