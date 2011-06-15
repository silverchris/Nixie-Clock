/*#include <p30f3014.h>
#include <adc12.h>

void enable_ADC(void){
	ADCON1bits.ADON = 0;
	int Channel = ADC_CH0_POS_SAMPLEA_AN9 & ADC_CH0_NEG_SAMPLEA_VREFN;
	SetChanADC12(Channel);
	ConfigIntADC12(ADC_INT_DISABLE);
	int PinConfig  = ENABLE_AN9_ANA;
	int Scanselect = SCAN_NONE;
	int Adcon3_reg = ADC_SAMPLE_TIME_10 & ADC_CONV_CLK_SYSTEM & ADC_CONV_CLK_32Tcy;
	int Adcon2_reg = ADC_VREF_AVDD_AVSS & ADC_SCAN_OFF & ADC_ALT_BUF_OFF & 
		ADC_ALT_INPUT_OFF &  ADC_SAMPLES_PER_INT_16;
	int Adcon1_reg = ADC_MODULE_ON & ADC_IDLE_CONTINUE & ADC_FORMAT_INTG &
		ADC_CLK_AUTO & ADC_AUTO_SAMPLING_ON;
	OpenADC12(Adcon1_reg, Adcon2_reg, Adcon3_reg,PinConfig, Scanselect);
}

int get_adc_value(void){
	int adc_value = ReadADC12(0);

	return adc_value;
}*/
