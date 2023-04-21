#include<LPC17xx.h>
#include<stdlib.h>
void lcd_init();
void lcd_puts(int);
void clear_ports()
{
	LPC_GPIO0->FIOCLR = 0x0F << 23;
	LPC_GPIO0->FIOCLR = 1<<27;
	LPC_GPIO0->FIOCLR = 1<<28;
	return;
}
void delay_lcd(unsigned int r1)
{
	unsigned int r;
	for(r = 0;r<r1;r++);
	return;
}
void write(int temp2,int type) //write to command/data register
{
	clear_ports();
	LPC_GPIO0->FIOPIN = temp2;
	if(type == 0)
		LPC_GPIO0->FIOCLR = 1<<27; //RS = 0 FOR COMMAND
	else
		LPC_GPIO0->FIOSET = 1<<27; //RS = 1 FOR DATA
	LPC_GPIO0->FIOSET = 1<<28; //EN = 1
	delay_lcd(25);
	LPC_GPIO0->FIOCLR = 1<<28; // EN = 0
	return;
}
void lcd_comdata(int temp1,int type)
{
	int temp2 = temp1 & 0xF0; //get high nibble
	temp2 = temp2 << 19; //data lines from 23 to 26
	write(temp2,type);
	delay_lcd(1000);
	temp2 = temp1 & 0x0f; //26-4+1
	temp2 = temp2 << 23;
	write(temp2, type);
	delay_lcd(1000);
	return;
}
void lcd_puts(int num)
{
	lcd_comdata(num,1);
	delay_lcd(800);
	return;
}
void lcd_init()
{
	LPC_PINCON->PINSEL3 &= 0xFC003FFF; //P0.23 to P0.28
	LPC_GPIO0->FIODIR |= 0x0F<<23|1<<27|1<<28; //p0.23 to p0.28 initialized
	clear_ports(); delay_lcd(3200);
	lcd_comdata(0x33,0); delay_lcd(30000);
	lcd_comdata(0x32,0); delay_lcd(30000);
	lcd_comdata(0x28,0); delay_lcd(30000); // function set
	lcd_comdata(0x0c,0); delay_lcd(800); //display cursor on off
	lcd_comdata(0x06,0); delay_lcd(800); //increment cursor, shift cursor to the right
	lcd_comdata(0x01,0); delay_lcd(10000); //clear display screen 
	return;
} 
int main()
{
	int num;
	SystemInit();
	SystemCoreClockUpdate();
	lcd_init();
	while(1)
	{
		if(!(LPC_GPIO2->FIOPIN & 1<<12)) //P2.12 PRESSED
		{
			num = rand() % 6 + 1;
			num = num + 0x30; // to convert to ascii
			lcd_comdata(0x80,0); //0x80-command-force cursor to beginning
			delay_lcd(800);
			lcd_puts(num);
		}
	}
}




