

#include <detpic32.h>
#include "delay.c"

void send2displays(unsigned char val)
{
	static const char cods[] ={
		0x3F, //0
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
	static int displayFlag = 0;
	
	unsigned int digit_low = val % 10;
	unsigned int digit_high = val / 10;
	
	
	if(displayFlag == 1)
	{
		LATDbits.LATD5 = 0;// apresentar no high
		LATDbits.LATD6 = 1;
		LATB = (LATB & 0x80FF) | (cods[digit_high] << 8);
	}else{
		LATDbits.LATD6 = 0;
		LATDbits.LATD5 = 1;// apresentar low
		LATB = (LATB & 0x80FF) | (cods[digit_low] << 8);	
	}

	displayFlag = !displayFlag;
}

unsigned char toBcd(unsigned char value)
{
	return ((value / 10) << 4) + (value % 10);
}


void blink(int counter,int way)
{
	int ciclevar = 40, msoff = 200;
	if(way == -1)
	{
		ciclevar += 60;
		msoff += 300;
	}

	int spass = 0;	
	while(spass < 5000)
	{
		int k = 0;
		do{
			send2displays(counter);
			delay(5);
		}while(++k < ciclevar);
	
		spass += 5* ciclevar;

		LATDbits.LATD5 = 0;
		LATDbits.LATD6 = 0;
		delay(msoff);

		spass += msoff;
	}
}


int main()
{
	TRISB = (TRISB & 0x80FE) | 0x0001; //rb14 a rb8 como output, rb0 como input
	TRISE = (TRISE & 0xFF00); // re7 a re0 como output
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0; //bit 5 e 6 como output para enable de displays

	int counter = 0;
	int way = 1, ms = 25;

	while(1)
	{
		if(PORTBbits.RB0 == 1)
		{
			way = 1;
			ms = 25;
		}else
		{
			way = -1;
			ms = 20;
		}

		LATE = (LATE & 0xFF00) | toBcd(counter);
		
		if(counter == 0 || counter == 59)
		{
			blink(counter,way);
			counter+=way;
			continue;
		}

		int i = 0;
		do{
			send2displays(counter);
			delay(10);
		}while(++i <  ms); // 20 * 10 = 200ms de atraso para contador de taxa 5hz 

		
		counter += way;
		if(counter ==59 && way == 1)
			counter = 0;	
	
		if (counter == 0 && way == -1)
			counter = 59;	

	
	}



	return 0;
}


