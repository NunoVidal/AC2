#include <detpic32.h>
#include "delay.c"


int main(void)
{

	TRISB = (TRISB & 0x80FF); // rb14 a rb8 como output
	TRISD = (TRISD & 0xFF9F); // rd6 e rd5 como output
	LATDbits.LATD5 = 1;
	LATDbits.LATD6 = 0;

	int cods[7] ={0xF700,0xFF00,0xB900,0xBF00,0xF900,0xF100,0xFD00};

	while(1)
	{	
		char ch;
		do{
			char* str = "\n Introduza um caracter entre a e g: ";
			printStr(str);
			ch = getChar();
		}while(ch < 'a' || ch > 'g');
	
		int toWrite;
		switch(ch)
		{
			case 'a':
			case 'A':
				toWrite = cods[0];
				break;
			case 'b':
			case 'B':
				toWrite = cods[1];
				break;
			case 'c':
			case 'C':
				toWrite = cods[2];
				break;

			case 'd':
			case 'D':
				toWrite = cods[3];
				break;
			case 'e':
			case 'E':
				toWrite = cods[4];
				break;
			case 'f':
			case 'F':
				toWrite = cods[5];
				break;
			case 'g':
			case 'G':
				toWrite = cods[6];
				break;
		};

		LATB = (LATB & 0x80FF) | toWrite;  
	
		LATDbits.LATD5 = 0;
		LATDbits.LATD6 = 1;
	}

	return 0;
}
