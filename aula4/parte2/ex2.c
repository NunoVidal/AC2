#include <detpic32.h>
#include "delay.c"


int main(void)
{

	TRISB = (TRISB & 0x80FF); // rb14 a rb8 como output
	TRISD = (TRISD & 0xFF9F); // rd6 e rd5 como output
	LATDbits.LATD5 = 0;
	LATDbits.LATD6 = 1;

	unsigned char segment;
	while(1)
	{	
		segment = 1;
		int i;
		for(i= 0; i < 7;i++)
		{
			LATB = (LATB & 0x80FF) | (segment << 8);			
			delay(100);
			segment = segment << 1;
		}



		LATD = LATD ^ (3 << 5);
	}

	return 0;
}
