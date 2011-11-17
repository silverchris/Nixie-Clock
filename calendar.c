#include "main.h"
//time_s seconds= 383875200;//349833600;//362100000;

const int daysinmonths[] =    {0,31,28,31,30,31,30,31,31,30,31,30,31};
const int daysinmonthsleep[] = {0,31,29,31,30,31,30,31,31,30,31,30,31};
const char tostring[60][3] = {"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31",
	"32","33","34","35","36","37","38","39","40","41","42","43","45","46","47","48","49","50","51","52","53","54","55","56","57","58","59","60"};
const char daysoftheweek[8][5] = {"sun","mon","tue","wed","thu","fri","sat"};
int day(time_s second){
	return (second/86400)+1;
}

int year(int days){
	int year = days/365;
	return year;
}

int month(int days, int leap){
	if (leap == 1){
		if (days <=31) return 1;//Jan
		else if (days <=60) return 2;//Feb
		else if (days <=91) return 3;//March
		else if (days <=121) return 4;//April
		else if (days <=152) return 5;//May
		else if (days < 182) return 6;//June
		else if (days < 213) return 7;//July
		else if (days < 244) return 8;//Aug
		else if (days < 274) return 9;//Sept
		else if (days < 305) return 10;//Oct
		else if (days < 335) return 11;//Nov
		else if (days < 366) return 12;//Dec
	}
	else{
		if (days <=31) return 1;//Jan
		else if (days <=59) return 2;//Feb
		else if (days <=90) return 3;//March
		else if (days <=120) return 4;//April
		else if (days <=151) return 5;//May
		else if (days <= 181) return 6;//June
		else if (days <= 212) return 7;//July
		else if (days <= 243) return 8;//Aug
		else if (days <= 273) return 9;//Sept
		else if (days <= 304) return 10;//Oct
		else if (days <= 334) return 11;//Nov
		else if (days <= 365) return 12;//Dec
	}
	return 0;
}

int dayOfmonth(int days, int month, int leap){
	int i;
	int subtract_days = 0;
	if (leap){
		for (i=0;i<month;i++){
			subtract_days = subtract_days+daysinmonthsleep[i];
		}
	}
	else{
		for (i=0;i<month;i++){
			subtract_days = subtract_days+daysinmonths[i];
		}
	}
	return days-subtract_days;
}

int dayOfweek(int y, int m, int d)
   {
       static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
       y -= m < 3;
       return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
   }
   
int isLeapYear(int year){
	  year=year+2000;
      int isLeap = 0;
      if (year%4==0) isLeap = 1;
      if (year%100==0) isLeap = 0;
      if (year%400==0) isLeap = 1;
      return isLeap;
}
int dayOfyear(int days,int year){
	int i;
	int leapdays = 0;
	for(i=0;i<year;i++){
		leapdays = leapdays+isLeapYear(i);
	}
	int day = (days-leapdays)-(365*year);
	return day;
}

time_s mktime(int year,int month, int day,int hour,int minute, int second){
	time_s time = 0;
	int leap = 0;
	if(!(year==0)){
		leap = isLeapYear(year);
		year = year-2000;
		int i;
		for(i=0;i<year;i++){
			time = time+(unsigned long)31536000;
			time = time+((unsigned long)86400*isLeapYear(i));
		}
		//time = time+31536000;
	}
	if(!(month==0)){
		int i;
		if (leap){
			for (i=1;i<month;i++){
				time=time+((unsigned long)86400*daysinmonthsleep[i]);
			}
		}
		else{
			for (i=1;i<month;i++){
				time=time+((unsigned long)86400*daysinmonths[i]);
			}
		}
	}
	if(!(day==0)){
		day = day-1;
		time=time+(86400*day);
	}
	if(!(hour==0)){
		time=time+((unsigned long)3600*hour);
	}
	if(!(minute==0)){
		time=time+((unsigned long)60*minute);
	}
	time=time+((unsigned long)1*second);
	return time;
}

void datetime(char *buffer, time_s time){
	int days = day(time);
	int years = year(days);
	int leap = isLeapYear(years);
	int dayofyear = dayOfyear(days,years);
	int months = month(dayofyear,leap);
	int dayofmonth = dayOfmonth(dayofyear,months,leap);
	int dayofweek = dayOfweek(2000+years,months,dayofmonth);
	time_s dayseconds = time%86400;
	*buffer = daysoftheweek[dayofweek][0];
	buffer++;
	*buffer = daysoftheweek[dayofweek][1];
	buffer++;
	*buffer = daysoftheweek[dayofweek][2];
	buffer++;
	*buffer = ' ';
	buffer++;
	*buffer = '2';
	buffer++;
	*buffer = '0';
	buffer++;
	*buffer = tostring[years][0];
	buffer++;
	*buffer = tostring[years][1];	
	buffer++;
	*buffer = '/';
	buffer++;
	*buffer = tostring[months][0];
	buffer++;
	*buffer = tostring[months][1];
	buffer++;
	*buffer = '/';
	buffer++;
	*buffer = tostring[dayofmonth][0];
	buffer++;
	*buffer = tostring[dayofmonth][1];
	buffer++;
	*buffer = ' ';
	buffer++;
	int hours = dayseconds/3600;
	*buffer = tostring[hours][0];
	buffer++;
	*buffer = tostring[hours][1];
	buffer++;
	*buffer = ':';
	buffer++;
	int minutes = (dayseconds%3600)/60;
	*buffer = tostring[minutes][0];
	buffer++;
	*buffer = tostring[minutes][1];
	buffer++;
	*buffer = ':';
	buffer++;	
	int secs = dayseconds%60;
	*buffer = tostring[secs][0];
	buffer++;	
	*buffer = tostring[secs][1];
	buffer++;	
	*buffer ='\0';
}
