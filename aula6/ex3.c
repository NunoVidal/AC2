#include <detpic32.h>


void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < FREQ/2000 * ms);
}


int main()
{
	
	TRISBbits.TRISB4 = 1;
	AD1PCFGbits.PCFG4 = 0;
	AD1CON1bits.SSRC = 7;

	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16; //sample time 16TAD( TAD = 100ns )
	
	AD1CON2bits.SMPI = 3;
	AD1CHSbits.CH0SA = 4;
	AD1CON1bits.ON = 1;

	while(1)
	{
		AD1CON1bits.ASAM = 1;
		while(IFS1bits.AD1IF == 0);
		
	
		int *p = &ADC1BUF0;
		int i;
		putChar('\r');
		for(i = 0; i < 16;i++)
		{
			printInt(p[i*4], 10 | 4 << 16);
			putChar('\t');
		}


		IFS1bits.AD1IF = 0;

	}



	return 0;
}

