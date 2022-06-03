
#include <detpic32.h>


volatile unsigned char voltage = 0;

void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < FREQ/2000 * ms);
}

void send2Display(unsigned char ch)
{
	static int flag = 0;
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

	unsigned int digitHigh = ch / 10;
	unsigned int digitLow = ch % 10;



	if(flag == 0)
	{
		LATDbits.LATD5 = 1;
		LATDbits.LATD6 = 0;
		LATB = (LATB & 0x80FF) | (cods[digitLow] << 8);
	}else
	{
		LATDbits.LATD5 = 0;
		LATDbits.LATD6 = 1;
		LATB = (LATB & 0x80FF) | (cods[digitHigh] << 8);
	}

	flag = !flag;
}



void _int_(27) isr_adc(void)
{
	int* p = &ADC1BUF0;
	int i = 0,sum = 0;
	for(;i < 8;i++)	
	{
		sum += p[i*4];
	}

	sum = sum / 8;

	voltage = (sum*33 + 511) / 1023;
	IFS1bits.AD1IF = 0;
}


int main(void)
{

	TRISB = (TRISB & 0x80FF); //-> do 8 ao 14 como output 
	TRISD = (TRISD & 0xF79F); //5 e 6 como output pq sao flags dos displays




	TRISBbits.TRISB4 = 1; // RBx digital output disconnected
	AD1PCFGbits.PCFG4 = 0; // RBx configured as analog input
	AD1CON1bits.SSRC = 7; // Conversion trigger selection bits: in this
				// mode an internal counter ends sampling and
	// starts conversion
	AD1CON1bits.CLRASAM = 1; // Stop conversions when the 1st A/D converter
		// interrupt is generated. At the same time,
		// hardware clears the ASAM bit
	AD1CON3bits.SAMC = 16; // Sample time is 16 TAD (TAD = 100 ns)
	AD1CON2bits.SMPI = 7; //(7+1) amostras // Interrupt is generated after XX samples
					
				// (replace XX by the desired number of
				// consecutive samples)
	AD1CHSbits.CH0SA = 4; // replace x by the desired input
				// analog channel (0 to 15)
	AD1CON1bits.ON = 1; 
	
	IPC6bits.AD1IP = 2;
	IFS1bits.AD1IF = 0;
	IEC1bits.AD1IE = 1;

	EnableInterrupts();
	
	int cnt = 0;
	while(1)
	{
		if(cnt == 0) // 5Hz
		{
			AD1CON1bits.ASAM = 1; //inicia conversao
		}

		send2Display(voltage);

		cnt = (cnt + 1) % 20;

		delay(10);
	};
	return 0;
}


