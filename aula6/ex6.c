#include <detpic32.h>


void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < FREQ/2000 * ms);
}

void send2displays(unsigned char val)
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
		0x6F, //9
		0x77, //A
		0x7C, //b
		0x39, //c
		0x5E, //d
		0x79, //E
		0x71
	};
	static int displayFlag = 0;
	
	unsigned int digit_low = val % 10;
	unsigned int digit_high = val / 10;
	
	
	if(displayFlag == 1)
	{
		LATDbits.LATD5 = 0;// apresentar no high
		LATDbits.LATD6 = 1;
		LATB = (LATB & 0x80FF) | (cods[digit_high] << 8);
	}else{
		LATDbits.LATD6 = 0;
		LATDbits.LATD5 = 1;// apresentar low
		LATB = (LATB & 0x80FF) | (cods[digit_low] << 8);	
	}

	displayFlag = !displayFlag;
}

int main()
{

	TRISB = (TRISB & 0x80F7) | 0x0008; // rb14a rb8 e rb4 a 1
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0;
	AD1PCFGbits.PCFG4 = 0;
	AD1CON1bits.SSRC = 7;

	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16; //sample time 16TAD( TAD = 100ns )
	
	AD1CON2bits.SMPI = 3;
	AD1CHSbits.CH0SA = 4;
	AD1CON1bits.ON = 1;

	int i = 0,v = 0;
	while(1)
	{
	
		if(i == 0)
		{
			AD1CON1bits.ASAM = 1;
			while(IFS1bits.AD1IF == 0);
			int *p = &ADC1BUF0;
			int sum = 0;
			for(i = 0; i < 4;i++)
			{
				sum += p[i*4];
			}
			sum = sum / 4;
			
			v = (sum*33 +511) / 1023;
			IFS1bits.AD1IF = 0;
		}	
		
		putChar('\r');
		printInt(v, 10);	
		send2displays(v);
		delay(10);
		
		i = (i+1) % 20;
	}



	return 0;
}

