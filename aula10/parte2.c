
#include <detpic32.h>



char getc(void)
{
	while(U2STAbits.URXDA == 0);
	return U2RXREG;
}

void putc(char byte)
{
	while(U1STAbits.UTXBF == 1);
	U1TXREG = byte;
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
	U1BRG = 10; // baudrate = pbclk / (mode * (BRG +1))
	U1MODEbits.BRGH = 0; //16 divider mode
	U1MODEbits.PDSEL = 00; //8bit no parity
	U1MODEbits.STSEL = 0; // 1 stop bit


	//modulos de transmissao e rececao
	
	U1STAbits.UTXEN = 1;
	U1STAbits.URXEN = 1;

	U1MODEbits.ON = 1;



	while(1)
	{
		putc(0x52);
		delay(10);
	}


	return 0;
}
