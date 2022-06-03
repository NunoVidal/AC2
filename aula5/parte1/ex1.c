

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
	
	unsigned char digit_low = val & 0x0F;
	unsigned char digit_high = val >> 4;
	
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



int main()
{
	TRISB = (TRISB & 0x80FF); //rb14 a rb8 como output
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0; //bit 5 e 6 como output para enable de displays

	while(1)
	{
		send2displays(0xA5);
		delay(10);
	}



	return 0;
}


