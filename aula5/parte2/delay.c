
#include <detpic32.h>

void delay(unsigned int ms)
{
	resetCoreTimer();
	while(readCoreTimer() < (FREQ/2000)*ms);
}
