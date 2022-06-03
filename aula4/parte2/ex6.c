#include <detpic32.h>
#include "delay.c"


int main(void)
{

	TRISB = (TRISB & 0x80F0) | 0x000F; // rb14 a rb8 como output e rb3 a rb0 como input
	TRISD = (TRISD & 0xFF9F); // rd6 e rd5 como output
	LATDbits.LATD5 = 1;
	LATDbits.LATD6 = 0;
	//0..9, a..g
	int cods[] ={0x3F, //0
		0x06, //1
		0x5B, //2
		0x4F, //3
		0x66, //4
		0x6D, //5
		0x7D, //6
		0x07, //7
		0x7F, //8
		0x6F, //9
		0x77, //A
		0x7C, //b
		0x39, //c
		0x5E, //d
		0x79, //E
		0x71
	};

	while(1)
	{	
		int dipState = 0;
		
		dipState |= PORTBbits.RB3 << 3;
		dipState |= PORTBbits.RB2 << 2;
		dipState |= PORTBbits.RB1 << 1;
		dipState |= PORTBbits.RB0;

			
		LATB = (LATB & 0x80FF) | cods[dipState] << 8;

	}

	return 0;
}
