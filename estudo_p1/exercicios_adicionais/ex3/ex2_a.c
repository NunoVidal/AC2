
#include <detpic32.h>

void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < FREQ/2000 * ms);
}

void sendStuff(int *arr)
{

	LATEbits.LATE0 = arr[0];	
	LATEbits.LATE1 = arr[1];	
	LATEbits.LATE2 = arr[2];	
	LATEbits.LATE3 = arr[3];	
}

int main(void)
{
	TRISEbits.TRISE0 = 0;
	TRISEbits.TRISE1 = 0;
	TRISEbits.TRISE2 = 0;
	TRISEbits.TRISE3 = 0;


	while(1)
	{
		unsigned char ch = getChar();
		int a[4];
		int i;

		switch(ch)
		{
			case '0':
				
				for(i = 0; i < 4;i++) a[i] = 0;
				a[0] = 1;
				sendStuff(a);
				break;
			case '1':
				for(i = 0; i < 4;i++) a[i] = 0;
				a[1] = 1;
				sendStuff(a);
				break;
			case '2':
				for(i = 0; i < 4;i++) a[i] = 0;
				a[2] = 1;
				sendStuff(a);
				break;
			case '3':
				for(i = 0; i < 4;i++) a[i] = 0;
				a[3] = 1;
				sendStuff(a);
				break;
			case '4':
				for(i = 0; i < 4;i++) a[i] = 1;
				sendStuff(a);
				delay(1000);
				for(i = 0; i < 4;i++) a[i] = 0;
				sendStuff(a);
				break;
		}
	}


	return 0;
}
