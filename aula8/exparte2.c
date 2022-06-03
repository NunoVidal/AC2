

#include <detpic32.h>

static volatile int counter = 0;

void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < FREQ/2000 * ms);
}


int main()
{
	TRISE = TRISE & 0xFFFE;
	LATE = LATE & 0xFFFE;
	TRISD = TRISD | 0x0100;

	T2CONbits.TCKPS = 7;
	PR2 = 39062; 

	IPC2bits.T2IP = 3; 
	IEC0bits.T2IE = 1; // Enable timer T2 interrupts
	IFS0bits.T2IF = 0;

	IPC1bits.INT1IP = 3; 
	IEC0bits.INT1IE = 1; // Enable timer T2 interrupts
	IFS0bits.INT1IF = 0;
	INTCONbits.INT1EP = 0;


	EnableInterrupts();

	while(1)
	{
	}

	return 0;
}



void _int_(8) isr_t2(void)
{
	if(counter == 6)
	{
		counter = 0;
		LATEbits.LATE0 = 0;
		T2CONbits.TON = 0;	
	}else{
		counter++;
	}
	IFS0bits.T2IF = 0;
}

void _int_(7) isr_int1(void)
{
	LATEbits.LATE0 = 1;
	T2CONbits.TON = 1;	
	TMR2 = 0;
	IFS0bits.INT1IF = 0;
}
