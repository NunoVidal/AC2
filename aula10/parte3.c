
#include <detpic32.h>



char getc(void)
{
	while(U2STAbits.URXDA == 0);
	return U2RXREG;
}

void putc(char byte)
{
	while(U2STAbits.UTXBF == 1);
	U2TXREG = byte;
}

void putstr(char *str)
{
	char* c = str;
	while(*c != '\0')
	{
		putc(*c);
		c++;
	}
}

void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < FREQ/2000 * ms);
}


int main()
{
	//configu\e
	//BRG = 2082 -> baudrate 600
	//BRG = 1041 -> baudrate 1200
	//BRG = 129 -> baudrate 9600
	U2BRG = 10; // baudrate = pbclk / (mode * (BRG +1))
	U2MODEbits.BRGH = 0; //16 divider mode
	U2MODEbits.PDSEL = 00; //8bit no parity
	U2MODEbits.STSEL = 0; // 1 stop bit


	//modulos de transmissao e rececao
		
	U2STAbits.UTXEN = 1;
	U2STAbits.URXEN = 1;

	U2MODEbits.ON = 1;
	TRISDbits.TRISD11 = 0;
	LATDbits.LATD11 = 0;


	while(1)
	{
		while(U2STAbits.TRMT == 0);
		LATDbits.LATD11 = 1;
		putstr("123456789AB");
		LATDbits.LATD11 = 0;
	}


	return 0;
}
