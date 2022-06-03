

#include <detpic32.h>
#include "delay.c"

void send2displays(unsigned char val)
{

}



int main()
{
	TRISB = (TRISB & 0x80FF); //rb14 a rb8 como output
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0; //bit 5 e 6 como output para enable de displays

	LATDbits.LATD5 = 1;// apresentar nos dois displays
	LATDbits.LATD6 = 1;

	while(1)
	{
		send2displays(0x15);

		delay
	

	}




	return 0;
}


