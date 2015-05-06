/* Simple ADC passthrough program for the LPC1114 */

#include "lpc111x.h"
void ConfigPins()
{
	SYSAHBCLKCTRL |= BIT6 + BIT16; // Turn on clock for GPIO and IOCON 
	// Make Port 0 bit 5 behave as a generic output port (open drain)
	IOCON_PIO0_5 |= BIT8;
	// Make Port 0 bit 10 behave as a generic I/O port
	IOCON_SWCLK_PIO0_10  = 1; 
	// Make Port 0 bit 11 behave as a generic I/O port
	IOCON_R_PIO0_11  = 1; 
	// Make pin 10 behave as a PWM output CT32B1_MAT0
	IOCON_R_PIO1_1 |= BIT1 + BIT0; 
}
void initPWM()
{
	// will use counter/timer CT32B1
	// Turn on CT32B1
	SYSAHBCLKCTRL |= BIT10;
	// Use match register 3 as period register because its output
	// is not pinned out.  A value of 48000000 produces a frequency of 1Hz
	// so, to generate a 30kHz pwm signal, set MR3 = 48000000/30000 = 1600	
	TMR32B1MR3 = 1600; 
	TMR32B1MR0 = 1600; // Zero output to begin with
	TMR32B1MCR = BIT10; // Reset TC on match with MR3
	TMR32B1TC = 0 ; // Zero the counter to begin with
	TMR32B1PWMC = BIT0; // Enable PWM on channel 0
	TMR32B1TCR = 1; // Enable the timer	
	
}
void setFrequency(int Frequency)
{
	int Divisor;
	TMR32B1TCR = 0; // Enable the timer	
	Divisor = 48000000/Frequency;
	TMR32B1MR3 = Divisor; 
	TMR32B1MR0 = Divisor >> 1; 
	TMR32B1TC = 0 ; // Zero the counter to begin with
	TMR32B1TCR = 1; // Enable the timer	
}
void setVolume(int Volume)
{	
	// sets the volume to a value in the range 0 to 100%
	int MatchValue;
	MatchValue = TMR32B1MR3;
	MatchValue = ((MatchValue * Volume) / 100)>>1;  // a duty of 50% produces most noise
	TMR32B1MR0 = MatchValue;
}
void delay(int dly)
{
	while(dly--);
}
int main()
{	
	int Frequency=262; // middle C
	int Volume = 0;
	ConfigPins();
	initPWM();
	setFrequency(Frequency);
	while(1) 
	{
		setVolume(Volume++);
		if (Volume >100)
			Volume = 0;
		delay(10000);
	}    
}



