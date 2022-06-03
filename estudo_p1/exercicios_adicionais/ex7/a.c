#include <detpic32.h>

void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < FREQ/2000 * ms);
}


int main()
{
	TRISB = (TRISB & 0xFFF0) | 0xF;
	char * str;
	TRISBbits.TRISB4 = 1; // RBx digital output disconnected
	AD1PCFGbits.PCFG4= 0; // RBx configured as analog input
	AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
	// mode an internal counter ends sampling and
	// starts conversion
	AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
	// interrupt is generated. At the same time,
	// hardware clears the ASAM bit
	AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
	AD1CON2bits.SMPI = 1-1; // Interrupt is generated after XX samples
	// (replace XX by the desired number of
	// consecutive samples)
	AD1CHSbits.CH0SA = 4; // replace x by the desired input
	// analog channel (0 to 15)
	AD1CON1bits.ON = 1; // Enable A/D converter
	// This must the last command of the A/D
	// configuration sequence
	
	while(1)
	{

		AD1CON1bits.ASAM = 1; // Stop conversions when the 1st A/D converter
		while(IFS1bits.AD1IF == 0);

		IFS1bits.AD1IF = 0;
	
		int val = ADC1BUF0 * 4 / 1023;	
		val ++;	//esta entre 1 e 5

		int ms = 1000/ val;
		int word = (PORTB & 0x000F);

		putChar('\r');
		str = "DS4=";
		printStr(str);
		printInt(word >>3,2 | 1 <<16);
		str = ", DS3=";
		printStr(str);

		printInt((word >>2) & 1,2 | 1 <<16);
		str = ", DS2=";
		printStr(str);

		printInt((word >>1) & 1,2 | 1 <<16);
		str = ", DS1=";
		printStr(str);
		printInt(word & 1,2 | 1 <<16);
		delay(ms);
	}
	return 0;
}
