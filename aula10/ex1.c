
#include <detpic32.h>


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
	//configure
	U2BRG = 10; // baudrate = pbclk / (mode * (BRG +1))
	U2MODEbits.BRGH = 0; //16 divider mode
	U2MODEbits.PDSEL = 00; //8bit no parity
	U2MODEbits.STSEL = 0; // 1 stop bit


	//modulos de transmissao e rececao
	
	U2STAbits.UTXEN = 1;
	U2STAbits.URXEN = 1;

	U2MODEbits.ON = 1;



	while(1)
	{
		putstr("String de teste\n");
		delay(1000);
	}


	return 0;
}
