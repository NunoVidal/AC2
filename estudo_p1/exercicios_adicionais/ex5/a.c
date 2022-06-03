
#include <detpic32.h>

void delay(unsigned int ms)
{
	resetCoreTimer();

	while(readCoreTimer() < FREQ/2000 * ms);
}


int main(void)
{

	int counter = 0,ms = 100;
	while(1)
	{
		unsigned char ch = inkey();
		putChar('\r');
		printInt(counter, 10 | 2 << 16);
		delay(ms);
		int keyValue = (int)ch - 48;	//ascii val - ascii zero val	
		switch(ch)
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
				ms = 1000 / (2* (1+ keyValue)); // freq  =  2 * (1 + tecla)	
			break;
			case '\n':	//press enter -> escreve freq
				putChar(',');
				printInt((int) 1000/ ms, 10 | 3 << 16);
				printStr("Hz");
			break;
		}


		counter++;
		if(counter >= 100)
		{
			counter = 0;
		}


	}	



	return 0;
}
