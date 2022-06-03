#include <detpic32.h>

void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < FREQ/2000 * ms);
}

int main()
{
	TRISE = TRISE & 0xFFC0;
	TRISBbits.TRISB2 = 1;

	int ms = 333;
	LATE = (LATE & 0xFFC0) | 0x0001;
	while(1)
	{
		if(PORTBbits.RB2 == 1)
		{
			ms = 142;
		}else{
			ms = 333;
		}

		int word = (LATE & 0x3F);
		int msb = (word >> 5) & 1;

		LATE = (LATE & 0xFFC0) | ((word << 1) | msb);

		delay(ms);

	}
	return 0;
}
