/* This program puts the output from the ADC onto the IO Pins */

#include "lpc111x.h"
#include <stdint.h>
#include <stdlib.h>

#define a 0.7
void delay(int dly)
{
	while (dly--);
}
void ConfigPins()
{
	SYSAHBCLKCTRL |= BIT6 + BIT13 + BIT16; // Turn on clock for GPIO, ADC and IOCON 
	// Output pin cofiguration
	// Port 1 Bit 1 -> generic io
	IOCON_R_PIO1_1 |= BIT0;
	// Port 1 Bit 2 -> generic io
	IOCON_R_PIO1_2 |= BIT0;
	// Port 1 Bit 3 -> generic io
	IOCON_SWDIO_PIO1_3 |= BIT0;
	// Port 1 bit 4 behaves as a generic IO port by default
	// Port 1 bit 5 behaves as a generic IO port by default
	// Port 1 bit 6 behaves as a generic IO port by default
	// Port 1 bit 7 behaves as a generic IO port by default
	// Port 1 bit 8 behaves as a generic IO port by default
	// Configure output addressing
	GPIO1DIR |= (BIT1+BIT2+BIT3+BIT4+BIT5+BIT6+BIT7+BIT8);
	GPIO1DATA=0;

	// Get the ADC started
	// select analog mode for PIO1_0
	IOCON_R_PIO1_0 = 2;
	// Power up the ADC
	PDRUNCFG &= ~BIT4; 
	// set ADC clock rate and select channel 1
	AD0CR=(11<<8)+BIT1;

}
int ReadADC()
{
	// Start a conversion
	AD0CR |=BIT1+BIT24;
	// Wait for conversion to complete
	while((AD0GDR&BIT31)==0);
	// return the result
	return ((AD0GDR>>8));  //Shift from 6 for all 10bits [6:15]
}
void Output(uint8_t data)
{
	GPIO1DATA = data;
}
int main()
{	
	ConfigPins();
	uint8_t data = 0;
	uint8_t prev = 0;
	while(1){
	data = ReadADC();
	uint8_t out = a*data + (1-a)*prev;
	prev = out;
	Output(out);
	delay(23);
	}
}
