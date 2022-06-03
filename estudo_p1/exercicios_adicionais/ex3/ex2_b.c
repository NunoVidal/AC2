
#include <detpic32.h>

void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < FREQ/2000 * ms);
}

int main(void)
{

	TRISE = (TRISE & 0xFFF0);


	while(1)
	{
		unsigned char ch = getChar();
		int a[4];
		int i;

		switch(ch)
		{
			case '0':
				LATE = (LATE & 0xFFF0) | 1;	
				break;
			case '1':
				LATE = (LATE & 0xFFF0) | 2;	
				break;
			case '2':
				LATE = (LATE & 0xFFF0) | 4;	
				break;
			case '3':
				LATE = (LATE & 0xFFF0) | 8;	
				break;
			case '4':
				LATE = (LATE & 0xFFF0) | 0xF; 
				delay(1000);
				LATE = (LATE & 0xFFF0); 
				break;
		}
	}


	return 0;
}
