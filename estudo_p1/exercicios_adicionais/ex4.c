
#include <detpic32.h>

void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < FREQ/2000 * ms);
}


void send2displays(unsigned char ch)
{

	static int flag = 0;

	
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

	
	unsigned char digit_h = ch >> 4;
	unsigned char digit_l = (ch & 0x0F);
	
	if(ch == 0xF0)
	{digit_h = 0; digit_l = 0;
	}else
	{
		digit_h = cods[digit_h];
		digit_l = cods[digit_l];
	}

	if(flag)
	{
		LATDbits.LATD5 = 0;
		LATDbits.LATD6 = 1;
	
		LATB = (LATB & 0x80FF) | digit_h << 8; 
	}else{
		LATDbits.LATD5 = 1;
		LATDbits.LATD6 = 0;
	
		LATB = (LATB & 0x80FF) | digit_l << 8; 
	}


	flag = !flag;
}



int main(void)
{

	TRISE = (TRISE & 0xFFF0);
	TRISB = (TRISB & 0x80FF);
	TRISD = (TRISD & 0xFF9F);
	unsigned char ch,aux;
	while(1)
	{
		ch = inkey();
		int counter;
		switch(ch)
		{
			case '0':
				LATE = (LATE & 0xFFF0) | 1;		
				aux = 0x0;
				break;
			case '1':
				LATE = (LATE & 0xFFF0) | 2;	
				aux = 0x1;
				break;
			case '2':
				LATE = (LATE & 0xFFF0) | 4;	
				aux = 0x2;
				break;
			case '3':
				LATE = (LATE & 0xFFF0) | 8;	
				aux = 0x3;
				break;
			case '4':
				LATE = (LATE & 0xFFF0) | 0xF;
				counter = 0;
				do{
			  	   	send2displays(0xFF);
					delay(10);
				}while(++counter < 100);
				inkey(); //clear buffer\
				send2displays(0xF0);
				send2displays(0xF0);
				LATE = (LATE & 0xFFF0); 
				break;
			case 0:	//do nothing in 0
				break;
			default:
				counter = 0;
				do{
			  	   	send2displays(0xFF);
					delay(10);
				}while(++counter < 100);
				send2displays(0xF0);
				send2displays(0xF0);
				inkey();
				break;
			
		}
		send2displays(aux);
		delay(10);
	}


	return 0;
}
