#include <p30f3014.h>
#include "main.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
_FOSC(CSW_FSCM_OFF & XT_PLL16);
_FWDT(WDT_OFF);                 //Turn off the Watch-Dog Timer.
_FBORPOR(MCLR_EN & PWRT_OFF);   //Enable MCLR reset pin and turn off the
                                //power-up timers.
_FGS(CODE_PROT_OFF);            //Disable Code Protection

extern char uartrxbuffer[120];
extern char uarttextbuffer[120];
extern time_s seconds;
extern time_s *timeptr;
int main(void){
	eeprom_first_run();
    enable_gps();
    enable_uart();
    enable_PWM();
	clockstart();
	clockrefreshstart();
	uartbuffer("Nixie Clock Version: ");
	uartbuffer(VERSION);
 	uartbuffer("\r\n>");
	while(1){
	Nop();
	}
}



