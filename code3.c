#include <LPC17xx.h>
unsigned int i,j;

unsigned int LED = 0x00008000;
int main()
{
	SystemInit(); //Add these two function for its internal operation
	SystemCoreClockUpdate();
	LPC_PINCON->PINSEL1 = 0x00000000;
	LPC_PINCON->PINSEL0 = 0x00000000;
	LPC_PINCON->PINSEL4 &= 0x00000000;
	//Configure Port0 PINS P0.15-P0.22 as GPIO function
	LPC_GPIO0->FIODIR = 0x007F8000;
	LPC_GPIO2->FIODIR = 0x00000000;
	while(1)
	{	
		int k=LPC_GPIO2->FIOPIN & 1<<12;
		 LED = 0x00008000;
		
		 for(i=0; i<8; i++)
		{
				if(k==0)
			{
				break;
			}
				LPC_GPIO0->FIOPIN = LED;
				for(j=0; j<10000; j++);
				LED<<=1;
		 }
	 
	}
}