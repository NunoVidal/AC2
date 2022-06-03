#include <detpic32.h>
#include "delay.c"


int main(void)
{
	TRISE = (TRISE & 0xFFF0); // e0 a e3 output
	
	while(1)
	{
		int counter;
		for(counter = 0x0; counter <= 0xF;counter++)
		{
			delay(250);
			LATE = (LATE & 0xFFF0) | counter;
		}
	}
	return 0;
}
