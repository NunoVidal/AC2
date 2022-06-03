
#include <detpic32.h>

typedef struct
{
	char mem[100]; // Storage area
	int nchar; // Number of characters to be transmitted
	int posrd; // Position of the next character to be transmitted
} t_buf;

void putc(char byte)
{
	while(U2STAbits.UTXBF == 1);
	U2TXREG = byte;
}

volatile t_buf txbuf;

void putstrInt(char *s)
{
	while(txbuf.nchar > 0);
	char *p = txbuf.mem;

	while(*s != '\0')
	{
		*p = *s;
		p++;
		s++;
		txbuf.nchar++;
	}
	txbuf.posrd = 0;
	IEC1bits.U2TXIE = 1; 
}

int main(void)
{
	// Configure UART2: 115200, N, 8, 1
	U2BRG = 10; // baudrate = pbclk / (mode * (BRG +1))
	U2MODEbits.BRGH = 0; //16 divider mode
	U2MODEbits.PDSEL = 00; //8bit no parity
	U2MODEbits.STSEL = 0; // 1 stop bit

	//modulos de transmissao e rececao
	
	U2STAbits.UTXEN = 1;
	U2STAbits.URXEN = 1;

	U2MODEbits.ON = 1;
	
	IEC1bits.U2RXIE = 0;
	IEC1bits.U2TXIE = 0; //enable rx disable tx interrupts
	IPC8bits.U2IP = 5;
	IFS1bits.U2RXIF = 0;
	U2STAbits.URXISEL = 00;
	
	TRISC = (TRISC & 0xBFFF);
	EnableInterrupts();
	txbuf.nchar = 0;
	while(1){
		putstrInt("Test string which can be as long as you like as long as it is no longer than 100 characters\n");
	};
	return 0;
}

void _int_(32) isr_uart2(void)
{
	if(IFS1bits.U2TXIF == 1)
	{
		if(txbuf.nchar > 0)
		{
			U2TXREG = txbuf.mem[txbuf.posrd];
			txbuf.posrd ++;
			txbuf.nchar --;
		}else{
			IEC1bits.U2TXIE = 0;
		}
		
		
		IFS1bits.U2TXIF = 0;
	}
}
