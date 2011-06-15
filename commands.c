#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "main.h"

#define count(ARRAY) (sizeof(ARRAY)/sizeof(*ARRAY))

struct item
{
    const char * name;
    void (*pt2Function)(char *,char *,char *);
};

static _Bool sorted;

static struct item items[] =
{
    { "time", &time_commands },
    { "pwm_duty", duty_cycle },
    { "pwm_freq", pwm_freq_set },
    { "help", &help }
};

static int compare(const void * p1, const void * p2)
{
    return strcmp(*((const char **)p1), *((const char **)p2));
}



pt2Func get_command(const char * name)
{
    if(!sorted)
    {
        qsort(items, count(items), sizeof(*items), compare);
        sorted = 1;
    }

    struct item * item = bsearch(&name, items, count(items), sizeof(*items),
        compare);
	if(item->name == 0x0000){
		return NULL;
	}
	else{
		return item ? item->pt2Function : 0;
	}
}

void execute_command(char *command_str_ptr){
	char command_str[30];
	strcpy(command_str,command_str_ptr);
	char out_buf[70];
	char *p;
	p = strtok(command_str, " ");
	pt2Func command = get_command(p);
	if(command != NULL){
	    char *a1 = strtok(NULL, " ");
	    char *a2 = strtok(NULL, " ");
	    command(&out_buf[0],a1,a2);
	    uartbuffer(out_buf);
	}
	else{
		uartbuffer("Invalid Command\r\n>");
	}
}

void help(char *out_buf, char *modeptr1, char *inputptr){
	uartbuffer("Availible Commands Are:\r\n\
help\n\
time(get|set date as YYYY/MM/DD-HH:MM:SS)\r\n\
pwm_duty(get|set duty in percent)\r\n\
pwm_freq(get|set frequency in hz)\r\n");
}

