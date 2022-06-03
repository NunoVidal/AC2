#include <detpic32.h>

void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < FREQ/2000 * ms);
}




void sendDisplay(int ch)
{

	static const char cods[] ={                                                               
                    0x3F, //0
                    0x06, //1
                    0x5B, //2
                    0x4F, //3
                    0x66, //4
                    0x6D, //5
                    0x7D, //6
                    0x07, //7
                    0x7F, //8
                    0x6F //9
            };
	
	LATDbits.LATD6 = 1;
	LATB = (LATB & 0x80FF) | (cods[ch] << 8);		

}



int main()
{
	TRISEbits.TRISE1 = 0;
	TRISDbits.TRISD6 = 0;
	LATEbits.LATE1 = 0;
	TRISB = (TRISB & 0x80FF);
	TRISBbits.TRISB4 = 1; // RBx digital output disconnected
	AD1PCFGbits.PCFG4= 0; // RBx configured as analog input
	AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
	// mode an internal counter ends sampling and
	// starts conversion
	AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
	// interrupt is generated. At the same time,
	// hardware clears the ASAM bit
	AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
	AD1CON2bits.SMPI = 2-1; // Interrupt is generated after XX samples
	// (replace XX by the desired number of
	// consecutive samples)
	AD1CHSbits.CH0SA = 4; // replace x by the desired input
	// analog channel (0 to 15)
	AD1CON1bits.ON = 1; // Enable A/D converter
	// This must the last command of the A/D
	// configuration sequence



	while(1)
	{
		AD1CON1bits.ASAM = 1;
		while(IFS1bits.AD1IF == 0);
			
		int *p = &ADC1BUF0;
		int sum = 0;
		for(; p < &ADC1BUF2;p+=4)
		{
			sum += *p;
		}
		
		printInt((sum /2),16 | 3 <<16);
		putChar('\n');	
			
		int voltage = ((sum/2) * 9 + 511) / 1023;
		
			
		sendDisplay(voltage);
		
		LATEbits.LATE1 = !LATEbits.LATE1;
		IFS1bits.AD1IF = 0;
		delay(200);
	}


	return 0;
}
