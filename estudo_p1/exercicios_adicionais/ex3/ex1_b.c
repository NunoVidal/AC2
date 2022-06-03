


#include <detpic32.h>


int main(void)
{

	TRISE = (TRISE & 0xFF00);
	TRISB = (TRISB | 0x000F);

	while(1)
	{
	
		unsigned int aux = (PORTB & 0x000F);
		int i;
		unsigned int a = (aux & 8) >> 3;
		a |= (aux & 4) >> 1;
		a |= (aux & 2) << 1;
		a |= (aux & 1) << 3;
		
			
		unsigned int b = ~aux;
		a = a << 4;
		a |= (b & 0x000f);
		LATE = (LATE & 0xFF00) | a;	

	}


	return 0;
}
