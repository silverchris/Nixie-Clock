#include <libpic30.h>
#define VERSION "0.1(Finding My Way)"
#define FOSC 6000000 /* The External oscillator/clock speed, In this example 1Mhz */
#define PLL 16 /* The PLL multiplier */
#define FCY (FOSC*PLL)/4 /* The instruction clock rate, which is the Oscillator divded by 4 */
#define TCY 1/FCY
#define BRG (((FCY/57600)/16)-1)
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

#define A1 PORTFbits.RF6 
#define A2 PORTFbits.RF3
#define A3 PORTFbits.RF2
#define A4 PORTFbits.RF5
#define A5 PORTFbits.RF4
#define A6 PORTFbits.RF1

#define HourMSDstate 0  /* States for the state machine */
#define HourLSDstate 1
#define MinuteMSDstate 2
#define MinuteLSDstate 3
#define SecondMSDstate 4
#define SecondLSDstate 5

#define FIRSTRUN_ADDR 0x7FFC00
#define PWM_FREQ_ADDR 0x7FFC02
#define PWM_DUTY_ADDR 0x7FFC04

#define UART_ALTRX_ALTTX 0xFFE7 /* TO FIX MICROCHIPS EPIC INCLUDE FUCKUP THOSE FUCKERS*/

/*Type Defs*/
typedef void (*pt2Func)(char *,char *,char *);

/*time function prototypes*/
void clockstart(void);
void clockstop(void);
void time_commands(char *out_buf, char *mode, char *inputptr);

/*Display Function Proto Types*/
void clockrefreshstart(void);

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

/*ADC function Prototypes*/
void enable_ADC(void);
int get_adc_value(void);

/*Command Function Prototypes*/
pt2Func get_command(const char * name);
void execute_command(char *command_string);
void help(char *out_buf, char *modeptr1, char *inputptr);

