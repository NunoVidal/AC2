
#include <detpic32.h>
//revisoes para teste pratico 2 de AC2
// Realizar este exercicio por fases:
//step 1 -> colocar um Adc a funcionar com interrutps;
//step 2 -> Ativar um timer que ative o ADC, ou seja uma freq de amostragem, de 5 Hz
//Step 2 -> mostrar o resultado nos displays de 7 segmentos o resultado em hexa ou bcd dependendo do DS1, 0->hexa, 1->bcd, com 
//refresh rate de 100Hz utilizando um timer
//Step 3 -> gerar um sinal PWM, e mostrar no LED D11 (porto rc14). Duty cycle é controlado pelo potenciometro( rodado ao max é 100%, min 0% ), utilizado timer3 com 100Hz do refresh rate do display
//Step 4 -> mandar uma string "O valor do Potenciometro e: [valor]", atraves de UART com as configuracoes default 115200, N, 8, 1
//Step 5 -> quando recebe, através da UART, o caracter r, deve limpar os valores do display de 7 segmentos


volatile int voltage = 0;


typedef struct{
	char mem[100]; //buffer de 100 bytes
	int nchars; //num caracteres a transmitir
	int posrd; //posicao de leitura para transmissao
} str_buffer;


volatile str_buffer buf;

void dutyCyleUpdate()
{
	int dutyCyle = voltage * 100 / 33;
	OC1RS = (int) (((PR3 + 1) * dutyCyle) / 100); //constante que controla PWM (default começar com 50%)
}

void putcpooling(char byte2send)
{
	while(U2STAbits.UTXBF == 1);
	U2TXREG = byte2send;
}


char getcpooling()
{
	if(U2STAbits.OERR == 1) //erro overun, esta-se a tentar ler mas o FIFO de rececao está cheio
	{
		U2STAbits.OERR = 0;
	}
	while(U2STAbits.URXDA == 0);
	return U2RXREG;
}


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
	
	unsigned char digitHigh = c >> 4;
	unsigned char digitLow = c & 0x0F; 
 	if(PORTBbits.RB0 == 0)
	{
		digitHigh = codigos[digitHigh];
		digitLow = codigos[digitLow];
	}else{
		digitHigh = codigos[(unsigned int) c / 10];
		digitLow = codigos[(unsigned int) c % 10];
	}	

	LATDbits.LATD5 = !flag;
	LATDbits.LATD6 = flag;
	LATB = (LATB & 0x80FF) | (((flag == 0) ? digitLow : digitHigh) << 8); 


	flag = !flag;
}

int main()
{
	//Configurar o AD1 -------
	TRISBbits.TRISB4 = 1;
	AD1PCFGbits.PCFG4 = 0;	
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = 4; // 5 conversoes
	AD1CHSbits.CH0SA = 4;
	AD1CON1bits.ON = 1;

	//configurar Display e DS1----------------
	TRISB = (TRISB & 0x80FE) | 0x0001 ;
	TRISDbits.TRISD5 = 0;
	TRISDbits.TRISD6 = 0;


	//Configurar o Timer2 (5Hz para frequencia de amostragem)-------------
	T2CONbits.TCKPS = 6; //prescaler = PBCLK / ((65535 +1) * fout) -> 64
	PR2 = 62499; 
	TMR2 = 0; 
	T2CONbits.TON = 1;

	//configurar Timer3 (100Hz para taxa de refrescamento do display) ---------------
	T3CONbits.TCKPS = 2; //kPrescaler = 4
	PR3 = 49999;
	TMR3 = 0;
	T3CONbits.TON = 1;

	//configurar OCR para gerar sinal PWM ----------------------------------
	OC1CONbits.OCM = 6; //pwm mode
	OC1CONbits.OCTSEL = 1; //1->timer3, 0 -> timer2	
	OC1RS = 25000; //constante que controla PWM (default começar com 50%)
	OC1CONbits.ON = 1;

	//configura led D11(porto rc14)
	TRISC = TRISC & 0xBFFF;

	//configurar UART2 (600,N,8,1)------------------------------
	U2BRG = 10;	
	U2MODEbits.BRGH = 0; //divisor de 16
	U2MODEbits.PDSEL = 0; //8 bits, no parity 
	U2MODEbits.STSEL = 0; //1 stop bit
	U2STAbits.URXEN = 1; //enable rx
	U2STAbits.UTXEN = 1; //enable tx
	U2MODEbits.ON = 1; //enable uart
	U2STAbits.URXISEL = 00; //modo de interrupt de RX é quando ha pelo menos um char no FIFO a ser lido



	//Interrupts --------
	//AD1 ----------------------
	IPC6bits.AD1IP = 2;
	IEC1bits.AD1IE = 1;
	IFS1bits.AD1IF = 0;
	//timer 2 ---------------
	IPC2bits.T2IP = 1;
	IEC0bits.T2IE = 1;
	IFS0bits.T2IF = 0;
	//timer3-----------------
	IPC3bits.T3IP = 1;
	IEC0bits.T3IE = 1;
	IFS0bits.T3IF = 0;
	//uart2 --------------
	IPC8bits.U2IP = 1;
	IEC1bits.U2RXIE = 1; //dou enable as interrupts para ja so de rx porque so se ativa tx quando houver algo a transmitir
	IFS1bits.U2RXIF = 0;
	IFS1bits.U2TXIF = 0;
	EnableInterrupts();

	while(1)
	{
		LATCbits.LATC14 = PORTDbits.RD0;
	};
	return 0;
}


void _int_(32) isr_uart2(void)
{
	if(IFS1bits.U2TXIF == 1)
	{
		if(buf.nchars > 0)
		{
			U2TXREG = buf.mem[buf.posrd++];
			buf.nchars--;
		}else{
			IEC1bits.U2TXIE = 0; //se nao houver nada a transmitir quero que nao seja ativadas interrupts de tx
		}
		IFS1bits.U2TXIF = 0;
		return;
	}

	if(IFS1bits.U2RXIF == 1)
	{
		char c = U2RXREG;
		if(c == 'r')
		{
			LATDbits.LATD5 = 0; //so se nota um blink pq o refresh rate do display é alto
			LATDbits.LATD6 = 0;
		}
		
		IFS1bits.U2RXIF = 0;
		return;
	}
}


void _int_(12) isr_t3(void)
{
	sendDisplay(voltage);
	
	IFS0bits.T3IF = 0;
}

void _int_(8) isr_t2(void)
{
	AD1CON1bits.ASAM = 1; //começar conversoes

	IFS0bits.T2IF = 0;
}


void _int_(27) isr_ad(void)
{
	//ler conversoes 
	int *p = (int*) &ADC1BUF0;
	int sum = 0;
	for(; p < (int *) &ADC1BUF5;p++)
	{
		sum += *p;	
	}

	int media = sum/5;
	voltage = ((media * 33 + 511) / 1023);
	dutyCyleUpdate();
	char *str = "Voltagem: ";
	char *pmem = buf.mem;
	if(buf.nchars <= 0) // o transmite se  o que la estava antes acabou
	{
		while(*str != '\0')
		{
			*pmem = *str;
			pmem++;
			str++;
			buf.nchars ++;
		}
		
		*pmem = (voltage / 10) + '0';  //ascii zero começa em 48, faz se a soma para que 48 + x dê o nr ascii correto 
		*(pmem+1) = (voltage % 10) + '0';
		*(pmem+2) = '\n';
		buf.posrd = 0;	
		buf.nchars += 3;
		IEC1bits.U2TXIE = 1;
	}
	
	IFS1bits.AD1IF = 0;
}
