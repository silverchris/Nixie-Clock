#include <libpic30.h>

typedef unsigned long int time_s;
#define VERSION "0.1(Finding My Way)"
#define FOSC 6000000 /* The External oscillator/clock speed, In this example 1Mhz */
#define PLL 16 /* The PLL multiplier */
#define FCY (FOSC*PLL)/4 /* The instruction clock rate, which is the Oscillator divded by 4 */
#define TCY 1/FCY
#define BRG (((FCY/57600)/16)-1)
#define BRG2 (((FCY/4800)/16)-1)
#define VINRATIO 7.8
#define VOUTRATIO 44

/* If you change any of the anode or cathode defines, you may need to fuck with the initalization shit*/
#define K0 PORTAbits.RA11 /* 0 Cathode */
#define K1 PORTBbits.RB0  /* 0 Cathode */
#define K2 PORTBbits.RB1  /* 0 Cathode */
#define K3 PORTBbits.RB2  /* 0 Cathode */
#define K4 PORTBbits.RB3  /* 0 Cathode */
#define K5 PORTBbits.RB4  /* 0 Cathode */
#define K6 PORTBbits.RB5  /* 0 Cathode */
#define K7 PORTBbits.RB6  /* 0 Cathode */
#define K8 PORTBbits.RB7  /* 0 Cathode */
#define K9 PORTBbits.RB8  /* 0 Cathode */

#define A1 PORTFbits.RF1 
#define A2 PORTFbits.RF0
#define A3 PORTBbits.RB12
#define A4 PORTBbits.RB11
#define A5 PORTBbits.RB10
#define A6 PORTBbits.RB9

#define HourMSDstate 0  /* States for the state machine */
#define HourLSDstate 1
#define MinuteMSDstate 2
#define MinuteLSDstate 3
#define SecondMSDstate 4
#define SecondLSDstate 5

#define FIRSTRUN_ADDR 0x7FFC00
#define PWM_FREQ_ADDR 0x7FFC02
#define PWM_DUTY_ADDR 0x7FFC04
#define TZ 0x7FFC06
#define TZ_DST 0x7FFC08
#define TZ_START_WEEKDAY 0x7FFC0A
#define TZ_START_DAY_OCCURRENCE 0x7FFC0C
#define TZ_START_MONTH 0x7FFC0E
#define TZ_START_HOUR 0x7FFC10
#define TZ_END_WEEKDAY 0x7FFC12
#define TZ_END_DAY_OCCURRENCE 0x7FFC14
#define TZ_END_MONTH 0x7FFC16
#define TZ_END_HOUR 0x7FFC18

#define GPIO1 PORTDbits.RD2
#define GPIO2 PORTDbits.RD3
#define GPIO3 PORTDbits.RD8
#define GPIO1_TRIS TRISDbits.TRISD2
#define GPIO2_TRIS TRISDbits.TRISD3
#define GPIO3_TRIS TRISDbits.TRISD8

#define UART_ALTRX_ALTTX 0xFFE7 /* TO FIX MICROCHIPS EPIC INCLUDE FUCKUP THOSE FUCKERS*/

/*Type Defs*/
typedef void (*pt2Func)(char *,char *,char *);

/*time function prototypes*/
void clockstart(void);
void clockstop(void);
void time_commands(char *out_buf, char *mode, char *inputptr);

/*Display Function Proto Types*/
void clockrefreshstart(void);
void display(int hours, int minutes, int second);

/*PWM Function Prototypes*/
void enable_PWM(void);
void pwm_freq_set(char *out_buf, char *modeptr1, char *inputptr);
void duty_cycle(char *out_buf, char *modeptr1, char *inputptr);

/*EEPROM function prototypes*/
void load_eeprom_data(void);
void write_eeprom(_prog_addressT address, int value);
int read_eeprom(_prog_addressT address);
void eeprom_first_run(void);

/*Serial Function Prototypes*/
void enable_uart(void);
void uartbuffer(char string[30]);
void U1TXInterrupt(void);
void ui(char input);

/*GPS function prototypes*/
void enable_gps(void);

/*ADC function Prototypes*/
void enable_ADC(void);
int get_adc_value(void);

/*Command Function Prototypes*/
pt2Func get_command(const char * name);
void execute_command(char *command_string);
void help(char *out_buf, char *modeptr1, char *inputptr);

/*Timezone Function prototypes*/
time_s calc_dst_date(int year,int month,int week_day,int inc, int hour);
void set_dst_start_end(void);

/*Calendar Function Prototypes*/
int day(time_s second);
int year(int days);
int month(int days, int leap);
int dayOfmonth(int days,int month, int leap);
int dayOfweek(int y, int m, int d);
int isLeapYear(int year);
int dayOfyear(int days, int year);
time_s mktime(int year,int month, int day,int hour,int minute, int second);
void datetime(char *buffer, time_s time);
