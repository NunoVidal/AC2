#include <detpic32.h>
void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < FREQ/2000 * ms);
}


void sendToDisplay(unsigned char ch)
{
	unsigned char cods[] = {
		0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
	};
	
	static int flag = 0;

	unsigned char digit_low = cods[ch & 0xF]; 
	unsigned char digit_high = cods[ch >> 4]; 

	if(flag)
	{
		LATDbits.LATD5 = 1;
		LATDbits.LATD6 = 0;
		LATB = (LATB & 0x80FF) | (digit_low << 8);
	}else{

		LATDbits.LATD5 = 0;
		LATDbits.LATD6 = 1;
		LATB = (LATB & 0x80FF) | (digit_high << 8);
	}
	
	flag = !flag; 	
}




int main()
{

	TRISE = (TRISE & 0xFF80);
	TRISB = (TRISB & 0x80FF) | 0x0003; 
	TRISD = (TRISD & 0xFF9F);

	int counter = 0;
	int timer = 0;
	while(1)
	{
		int cd = (PORTB & 0x000F);
		unsigned char c,aux;
		switch(cd)
		{
			case 1:
			case 2:
				aux = inkey();
				if(aux >= '0' && aux <= '9')
				{
					if(cd == 1)
					{
						c = aux << 4;
					}else{
						c = aux & 0x0F;
					}
				}
			break;
			case 3:
				c = (unsigned char) counter;
			break;
		}
		if(cd != 0)
		{
			sendToDisplay(c);
		}else
		{

			LATDbits.LATD5 = 0;
			LATDbits.LATD6 = 0;
		}

		delay(10);
		timer ++;
		if(timer == 20)
		{
			timer = 0;
			counter ++;
		}
		if(counter >= 60)
			counter = 0;
		LATE = (LATE & 0xFF80) | counter; 
	}



	return 0;
}

