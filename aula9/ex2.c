#include <detpic32.h>


void setPWM(unsigned int dutyCycle)
{
	if(dutyCycle <0 || dutyCycle > 100)
		return; 

	OC1RS = ((PR3+1) * dutyCycle) / 100; 
}
int main()
{
	T3CONbits.TCKPS = 2; // 1:32 prescaler (i.e Fout_presc = 625 KHz)
        PR3 = 49999; // Fout = 20MHz / (32 * (62499 + 1)) = 10 Hz	
	TMR3 = 0; // Reset timer T2 count register
        T3CONbits.TON = 1; // Enable timer T2 (must be the last command of the

	IPC3bits.T3IP = 2;
	IEC0bits.T3IE = 1;
	IFS0bits.T3IF = 0;

	OC1CONbits.OCM = 6; // PWM mode on OCx; fault pin disabled
	OC1CONbits.OCTSEL =1;// Use timer T2 as the time base for PWM generation
	setPWM(20);
	OC1CONbits.ON = 1; // Enable OC1 modul	

	TRISCbits.TRISC14 = 0;

	EnableInterrupts();	
	while(1)
	{
		LATCbits.LATC14 = LATDbits.LATD0;
		printInt(LATDbits.LATD0,2);
		
	}
	return 0;
}



void _int_(12) isr_t3(void)
{
	IFS0bits.T3IF = 0;
}
