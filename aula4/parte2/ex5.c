#include <detpic32.h>
#include "delay.c"


int main(void)
{

	TRISB = (TRISB & 0x80FF); // rb14 a rb8 como output
	TRISD = (TRISD & 0xFF9F); // rd6 e rd5 como output
	LATDbits.LATD5 = 1;
	LATDbits.LATD6 = 0;
	//0..9, a..g
	int cods[] ={0x3F, //0
		0x06, //1
		0x5B, //2
		0x4F, //3
		0x66, //4
		0x6D, //5
		0x7D, //6
		0x07, //7
		0x7F, //8
		0x67, //9
		0x77, //A
		0x7C, //b
		0x39, //c
		0x5E, //d
		0x79, //E
		0x71
	};

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

		LATB = (LATB & 0x80FF) | (toWrite << 8) ;  
	
		LATDbits.LATD5 = 0;
		LATDbits.LATD6 = 1;
	}

	return 0;
}
