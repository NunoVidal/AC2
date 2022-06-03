#include <detpic32.h>

volatile int voltageMin = 6;
volatile int voltageMax = 0;
void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < FREQ/2000 * ms);
}

void putc(char byte)
{
	while(U2STAbits.UTXBF == 1);
	U2TXREG = byte;
}


void sendVoltage(int voltage)
{
	char * str = "VMxx=";
	while(*str != '\0'){
		putc(*str);
		str++;	
	}
	char firstDigit = (voltage /10) + '0';
	char lastDigit = (voltage %10) + '0';
	putc(firstDigit);
	putc(lastDigit);
}

int main()
{

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

	IPC1bits.T1IP = 2; // Interrupt priority (must be in range [1..6])
	IEC0bits.T1IE = 1; // Enable timer T2 interrupts
	IFS0bits.T1IF = 0; // Reset timer T2 interrupt flag	

	IPC3bits.T3IP = 2; // Interrupt priority (must be in range [1..6])
	IEC0bits.T3IE = 1; // Enable timer T2 interrupts
	IFS0bits.T3IF = 0; // Reset timer T2 interrupt flag	
	
	IPC6bits.AD1IP = 2; // Interrupt priority (must be in range [1..6])
	IEC1bits.AD1IE = 1; // Enable timer T2 interrupts
	IFS1bits.AD1IF = 0; // Reset timer T2 interrupt flag	

	// Configure UART2: 115200, N, 8, 1
	U2BRG = 10; // baudrate = pbclk / (mode * (BRG +1))
	U2MODEbits.BRGH = 0; //16 divider mode
	U2MODEbits.PDSEL = 00; //8bit no parity
	U2MODEbits.STSEL = 0; // 1 stop bit

	//modulos de transmissao e rececao
	
	U2STAbits.UTXEN = 1;
	U2STAbits.URXEN = 1;

	U2MODEbits.ON = 1;
	
	IEC1bits.U2RXIE = 1;
	IEC1bits.U2TXIE = 0; //enable rx disable tx interrupts
	IPC8bits.U2IP = 5;
	IFS1bits.U2RXIF = 0;
	U2STAbits.URXISEL = 00;
	
	EnableInterrupts();	
	while(1);

	return 0;
}

void _int_(4) isr_T1(void)
{
	AD1CON1bits.ASAM = 1;
	IFS0bits.T1IF = 0;
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

	int voltage = (sum*33 +511) / 1023;
	if(voltage < voltageMin)
		voltageMin = voltage;
	if(voltage > voltageMax)
		voltageMax = voltage;
	
	IFS1bits.AD1IF = 0;
}



void _int_(32) isr_uart2(void)
{
	if(IFS1bits.U2RXIF == 1)
	{
		char c = U2RXREG;
		if(c == 'M')
			sendVoltage(voltageMax);
		if(c == 'm')
			sendVoltage(voltageMin);

		IFS1bits.U2RXIF = 0;
	}
}
