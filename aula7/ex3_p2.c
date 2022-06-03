
#include <detpic32.h>


volatile unsigned char voltage = 0;


void _int_(27) isr_adc(void)
{
	volatile int adc_value;
	adc_value = ADC1BUF0;
	AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0;
	LATDbits.LATD11 = 1;
}


int main(void)
{

	TRISD = (TRISD & 0xF7FF); //TRISD 11

	TRISBbits.TRISB4 = 1; // RBx digital output disconnected
	AD1PCFGbits.PCFG4 = 0; // RBx configured as analog input
	AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
				// mode an internal counter ends sampling and
	// starts conversion
	AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
		// interrupt is generated. At the same time,
		// hardware clears the ASAM bit
	AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
	AD1CON2bits.SMPI = 0; //(7+1) amostras // Interrupt is generated after XX samples
					
				// (replace XX by the desired number of
				// consecutive samples)
	AD1CHSbits.CH0SA = 4; // replace x by the desired input
				// analog channel (0 to 15)
	AD1CON1bits.ON = 1; 
	
	IPC6bits.AD1IP = 2;
	IFS1bits.AD1IF = 0;
	IEC1bits.AD1IE = 1;

	EnableInterrupts();
	
	AD1CON1bits.ASAM = 1;	
	while(1)
	{
		LATDbits.LATD11 = 0;
	};
	return 0;
}


