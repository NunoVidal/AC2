#include <detpic32.h>


void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < FREQ/2000 * ms);
}


int main()
{
	TRISD = (TRISD & 0xF7FF);
	TRISBbits.TRISB4 = 1;
	AD1PCFGbits.PCFG4 = 0;
	AD1CON1bits.SSRC = 7;

	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16; //sample time 16TAD( TAD = 100ns )
	
	AD1CON2bits.SMPI = 0;
	AD1CHSbits.CH0SA = 4;
	AD1CON1bits.ON = 1;

	while(1)
	{
		LATD = (LATD & 0xF7FF) | 0x0800; 
		AD1CON1bits.ASAM = 1;
		while(IFS1bits.AD1IF == 0);
		
		int aux = ADC1BUF0;

		LATD = (LATD & 0xF7FF); 
		IFS1bits.AD1IF = 0;

	}



	return 0;
}

