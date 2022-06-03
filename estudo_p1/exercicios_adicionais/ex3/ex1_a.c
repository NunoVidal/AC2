


#include <detpic32.h>


int main(void)
{

	TRISEbits.TRISE0 = 0; //define as output
	TRISEbits.TRISE1 = 0;
	TRISEbits.TRISE2 = 0;
	TRISEbits.TRISE3 = 0;
	TRISEbits.TRISE4 = 0;
	TRISEbits.TRISE5 = 0;
	TRISEbits.TRISE6 = 0;
	TRISEbits.TRISE7 = 0;


	TRISBbits.TRISB0 = 1;
	TRISBbits.TRISB1 = 1; //define as input
	TRISBbits.TRISB2 = 1;
	TRISBbits.TRISB3 = 1;

	while(1)
	{
		LATEbits.LATE4 = PORTBbits.RB3;
		LATEbits.LATE5 = PORTBbits.RB2;
		LATEbits.LATE6 = PORTBbits.RB1;
		LATEbits.LATE7 = PORTBbits.RB0;
		
		LATEbits.LATE0 = !PORTBbits.RB0;
		LATEbits.LATE1 = !PORTBbits.RB1;
		LATEbits.LATE2 = !PORTBbits.RB2;
		LATEbits.LATE3 = !PORTBbits.RB3;
	}


	return 0;
}
