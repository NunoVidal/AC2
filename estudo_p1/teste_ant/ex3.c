

#include <detpic32.h>


void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < FREQ/2000 * ms);
}


void sendDigit(int flagLS)
{
	unsigned char a = 0x77;
	unsigned char b = 0x7c;


	if(flagLS)
	{
		LATDbits.LATD5 = 1;
		LATDbits.LATD6 = 0;
		LATB = (LATB & 0x80FF) | (a << 8);
	}else{

		LATDbits.LATD5 = 0;
		LATDbits.LATD6 = 1;
		LATB = (LATB & 0x80FF) | (b << 8);
	}
}


int main()
{

	TRISB = (TRISB & 0x80EF) | 0x0010;  //8 a 14 display; 4 input AD 
	TRISD = (TRISD & 0xFF9F);	//5 e 6 sao os transistores display
	
	AD1PCFGbits.PCFG4 = 0;
	AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
	AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
	AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
	AD1CON2bits.SMPI = 1; // Interrupt is generated after XX samples
	AD1CHSbits.CH0SA = 4; // replace x by the desired input
	AD1CON1bits.ON = 1; 

	while(1)
	{
		AD1CON1bits.ASAM = 1; 
		while( IFS1bits.AD1IF == 0 );
	

		int sum = 0;
		int *p = (int *)&ADC1BUF0;
		for(; p < (int*) &ADC1BUF2;p+=4)
		{
			sum += *p;
		}
		int voltage = ((sum / 2) *33 + 511)  / 1023;	
		
		if(voltage <= 12)
		{
			sendDigit(1);
		}	else
		{
			sendDigit(0);
		}
		putChar('\r');	
		printInt(voltage, 10 | 3 << 16);


		IFS1bits.AD1IF = 0; // precisa de por reset na interrupt
		delay(100);
	}
	return 0;
}
