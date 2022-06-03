#include <detpic32.h>

volatile int voltage = 0;
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


	//------------------------ CONFIG TIMERS---
	//T1-> ADC freq
	//T3 -> refresh rate displays
	
	T1CONbits.TCKPS = 2; // 1:32 prescaler (i.e Fout_presc = 625 KHz)
	PR1 = 62499; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
	TMR1 = 0; // Reset timer T2 count register
	T1CONbits.TON = 1; // Enable timer T2 (must be the last command of the

	T3CONbits.TCKPS = 2; // 1:32 prescaler (i.e Fout_presc = 625 KHz)
	PR3 = 49999; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz
	TMR3 = 0; // Reset timer T2 count register
	T3CONbits.TON = 1; // Enable timer T2 (must be the last command of the



	IPC1bits.T1IP = 2; // Interrupt priority (must be in range [1..6])
	IEC0bits.T1IE = 1; // Enable timer T2 interrupts
	IFS0bits.T1IF = 0; // Reset timer T2 interrupt flag	

	IPC3bits.T3IP = 2; // Interrupt priority (must be in range [1..6])
	IEC0bits.T3IE = 1; // Enable timer T2 interrupts
	IFS0bits.T3IF = 0; // Reset timer T2 interrupt flag	
	
	IPC6bits.AD1IP = 2; // Interrupt priority (must be in range [1..6])
	IEC1bits.AD1IE = 1; // Enable timer T2 interrupts
	IFS1bits.AD1IF = 0; // Reset timer T2 interrupt flag	
	
	EnableInterrupts();	
	while(1);

	return 0;
}

void _int_(4) isr_T1(void)
{
	AD1CON1bits.ASAM = 1;
	IFS0bits.T1IF = 0;
}

void _int_(12) isr_T3(void)
{
	putChar('\r');
	printInt(voltage, 10);	
	send2displays(voltage);
	IFS0bits.T3IF = 0;
}
void _int_(27) isr_adc(void)
{
	int *p = &ADC1BUF0;
	int sum = 0;
	int i;
	for(i = 0; i < 4;i++)
	{
		sum += p[i*4];
	}
	sum = sum / 4;

	voltage = (sum*33 +511) / 1023;
	IFS1bits.AD1IF = 0;
}



