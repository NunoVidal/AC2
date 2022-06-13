

#include<detpic32.h>
#include <math.h>

volatile int counter = 0;
volatile int flag1Hz = 0;
void sendDisplay(unsigned char c)
{

	static int flag = 0;
	static const char codigos[] = {
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

	char digitHigh = codigos[c / 10];
	char digitLow = codigos[c % 10];

	if(!flag)
	{
		LATDbits.LATD5 = 1;
		LATDbits.LATD6 = 0;
		LATB = (LATB & 0x80FF) | (digitLow <<8);
	}else{
		LATDbits.LATD5 = 0;
		LATDbits.LATD6 = 1;
		LATB = (LATB & 0x80FF) | (digitHigh <<8);
	}


	flag = !flag;
}

void setFreq(int valADC)
{
	//int freq = 2 * (1 + tecla);
	int freq =  1 + (valADC / 127);
	if(freq == 1)
	{
		flag1Hz = 1;
		freq = 2; //timer nao consegue 1Hz, coloco 2 e uso 2 ciclos de relogio
	}else{
		flag1Hz = 0;
	}
	int k = ceil(20000000/(65536*freq));

	int prescalers[] = {1,8,64,256};	
	int prescalerIndex = ceil(log(k)/log(8)); 
	
	T1CONbits.TCKPS = prescalerIndex;
	PR1 = (int) (20000000/(prescalers[prescalerIndex]*freq)) -1;
}

int main()
{
	TRISB = TRISB & 0x80FF;
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0;


	//configure timer 1 counter (10hz)
	T1CONbits.TCKPS = 2; //64
	PR1 = 31249;
	TMR1 = 0;
	T1CONbits.TON = 1;


	T2CONbits.TCKPS = 3; //8
	PR2 = 49999;
	TMR2 = 0;
	T2CONbits.TON = 1;

	//AD
	TRISBbits.TRISB4 = 1; 
	AD1PCFGbits.PCFG4= 0;
	AD1CON1bits.SSRC = 7; 
	AD1CON1bits.CLRASAM = 1; 
	AD1CON3bits.SAMC = 16; 
	AD1CON2bits.SMPI = 1-1;
	AD1CHSbits.CH0SA = 4; 
	AD1CON1bits.ON = 1;
		
	//interrupts 
	IPC1bits.T1IP = 2;
	IEC0bits.T1IE = 1;
	IFS0bits.T1IF = 0;

	IPC2bits.T2IP = 2;
	IEC0bits.T2IE = 1;
	IFS0bits.T2IF = 0;

	IPC6bits.AD1IP = 2;
	IFS1bits.AD1IF = 0;
	IEC1bits.AD1IE = 1;

	EnableInterrupts();
	resetCoreTimer();
	while(1)
	{
		/*char c = inkey();

		switch(c)
		{
			case'0' :
			case '1':
			case '2':
			case '3':
			case '4':
				putChar('\n');
				printStr("Nova Frequencia: ");
				putChar(c);	
				setFreq(((int) c) - 48);
			break;
			default:
				
			break;
		}*/

		if(readCoreTimer() > 20000* 250)
		{
			resetCoreTimer();
			AD1CON1bits.ASAM = 1;
		}
		
	}	
	return 0;
}


void _int_(27) isr_ad(void)
{
	int val = ADC1BUF0;	
	setFreq(val);
	IFS1bits.AD1IF = 0;
}


void _int_(4) isr_t1(void)
{
	static int cnt = 0;
	if(flag1Hz == 1)
	{
		if(cnt == 0) //dar 2 ticks a 2 hz para ter o 1Hz 
		{
			cnt ++;	
			IFS0bits.T1IF = 0;
			return;
		}else{
			cnt = 0;
		}	
	}
	if(counter < 100)
	{
		counter++;
	}else{
		counter = 0;
	}
	IFS0bits.T1IF = 0;
}


void _int_(8) isr_t2(void)
{
	sendDisplay(counter);
	IFS0bits.T2IF = 0;
}




