
#include <detpic32.h>

void putc(char byte)
{
	while(U2STAbits.UTXBF == 1);
	U2TXREG = byte;
}

int main(void)
{
	// Configure UART2: 115200, N, 8, 1
	// Configure UART2 interrupts, with RX interrupts enabled
	//and TX interrupts disabled:
	//	enable U2RXIE, disable U2TXIE (register IEC1)
	//	set UART2 priority level (register IPC8)
	//	clear Interrupt Flag bit U2RXIF (register IFS1)
	//define RX interrupt mode (URXISEL bits)
	// Enable global Interrupts
	
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
	
	TRISC = (TRISC & 0xBFFF);
	EnableInterrupts();
	while(1);
	return 0;
}

void _int_(32) isr_uart2(void)
{
	if(IFS1bits.U2RXIF == 1)
	{
		char ch = U2RXREG;
		putc(ch);
		if(ch == 'T')
		{
			LATCbits.LATC14 = 1;
		}
		if(ch == 't')
			LATCbits.LATC14 = 0;
		IFS1bits.U2RXIF = 0;
	}
}
