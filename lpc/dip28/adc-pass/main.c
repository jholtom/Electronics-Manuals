/* This program displays the output from the ADC on an LED display
 * 
 * Operation:
 * A global array represents the 'video' memory for the display.  
 * In order to display a number, the DisplayNumber function must be 
 * called which breaks the number into its component digits and then
 * translates these to the bit patterns that cause the appropriate
 * display segments to turn on. These patterns are stored in the 
 * global DisplayMemory array.
 * The Systick handler takes the contents of the DisplayMemory
 * array and copies them out to the port pins driving the display 
 * segments.  Each display digit is activated in turn, its are 
 * turned on (or not) and there is a slight pause which fools our
 * eyes into thinking that all display digits are on all of the time.
 * 
 * Wiring.
 * The display is wired to Port 0.  Segments are wired to bits 5-11
 * and digits are wired to bits 1-4.  The display in question is a 
 * 4 digit common cathode device.  In order to place a number on a 
 * digit the segments must be driven high (through a resistor to limit
 * current) and the common cathode for that display must be driven
 * low.  
 * 
 * The keypad is wired to Port 1 bits 1-5,8,9.  The keypad is arranged
 * in rows and columns
 */

#include "lpc111x.h"
// Association of bits to segments due to wiring
// Display is on Port 0
#define SEG_A BIT5
#define SEG_B BIT6
#define SEG_C BIT7
#define SEG_D BIT8
#define SEG_E BIT9
#define SEG_F BIT10
#define SEG_G BIT11
#define DIG_1 BIT1
#define DIG_2 BIT2
#define DIG_3 BIT3
#define DIG_4 BIT4
void initSysTick(void);

// Digit segment definitions
const short digits[]={SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F        ,\
					          SEG_B | SEG_C                                ,\
					  SEG_A | SEG_B |         SEG_D | SEG_E |         SEG_G,\
					  SEG_A | SEG_B | SEG_C | SEG_D |       	      SEG_G,\
					          SEG_B | SEG_C |                 SEG_F | SEG_G,\
					  SEG_A |         SEG_C | SEG_D |         SEG_F | SEG_G,\
					  SEG_A |         SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,\
					  SEG_A | SEG_B | SEG_C                                ,\
					  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G,\
					  SEG_A | SEG_B | SEG_C | SEG_D |         SEG_F | SEG_G \
					  };
int DisplayMemory[4];

void SysTick(void)
{
	static int DigitNumber=1;
	switch (DigitNumber) 
	{
		case 1: {
			// Turn on (make low) the desired digit and blank all segments
			GPIO0DATA = DIG_1 | DIG_2 | DIG_3;
			// Set the relevant segment bits
			GPIO0DATA |= DisplayMemory[0];
			// Wait for display to light up
			break;
		}
		case 2: {
			// Turn on (make low) the desired digit and blank all segments
			GPIO0DATA = DIG_1 | DIG_2 | DIG_4;
			// Set the relevant segment bits
			GPIO0DATA |= DisplayMemory[1];
			// Wait for display to light up
			break;
		}
		case 3: {
			// Turn on (make low) the desired digit and blank all segments
			GPIO0DATA = DIG_1 | DIG_3 | DIG_4;
			// Set the relevant segment bits
			GPIO0DATA |= DisplayMemory[2];
			// Wait for display to light up
			break;
		}
		case 4: {
			// Turn on (make low) the desired digit and blank all segments
			GPIO0DATA = DIG_2 | DIG_3 | DIG_4;
			// Set the relevant segment bits
			GPIO0DATA |= DisplayMemory[3];
			// Wait for display to light up
			break;
		}
	}
	DigitNumber++;
	if (DigitNumber > 4)
		DigitNumber = 1;
}
void initSysTick()
{
	
	// The systick timer is driven by a 48MHz clock
	// Divide this down to achieve a 1ms timebase
	// Divisor = 48MHz/1000Hz
	// Reload value = 48000-1
	// enable systick and its interrupts
	SYST_CSR |=(BIT0+BIT1+BIT2); 
	SYST_RVR=48000-1; // generate 1 second time base
	SYST_CVR=5;
	enable_interrupts();
}
void delay(int dly)
{
  while (dly--);
}

void DisplayNumber(int Number)
{
	DisplayMemory[0]=digits[Number % 10];
	Number = Number / 10;
	DisplayMemory[1]=digits[Number % 10];
	Number = Number / 10;
	DisplayMemory[2]=digits[Number % 10];
	Number = Number / 10;
	DisplayMemory[3]=digits[Number % 10];
}
void RefreshDisplay(void)
{ 
	// Turn on (make low) the desired digit and blank all segments
	GPIO0DATA = DIG_1 | DIG_2 | DIG_3;
	// Set the relevant segment bits
	GPIO0DATA |= DisplayMemory[0];
	// Wait for display to light up
	delay(1000);
	
	// repeat for next digit
	GPIO0DATA = DIG_1 | DIG_2 | DIG_4;
	GPIO0DATA |= DisplayMemory[1];
	delay(1000);
	
	// repeat for next digit
	GPIO0DATA = DIG_1 | DIG_3 | DIG_4;
	GPIO0DATA |= DisplayMemory[2];
	delay(1000);


	// repeat for next digit
	GPIO0DATA = DIG_2 | DIG_3 | DIG_4;
	GPIO0DATA |= DisplayMemory[3];
	delay(1000);
}
void ConfigPins()
{
	SYSAHBCLKCTRL |= BIT6 + BIT13 + BIT16; // Turn on clock for GPIO, ADC and IOCON 
	// Make all of the segment and digit bits outputs
	GPIO0DIR = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G | DIG_1 | DIG_2 | DIG_3 | DIG_4;
	// Turn off (make high) all display digits
	GPIO0DATA = DIG_1 | DIG_2 | DIG_3  | DIG_4;
	// Make Port 0 bit 5 behave as a generic output port (open drain)
	IOCON_PIO0_5 |= BIT8;
	// Make Port 0 bit 10 behave as a generic I/O port
	IOCON_SWCLK_PIO0_10  = 1; 
	// Make Port 0 bit 11 behave as a generic I/O port
	IOCON_R_PIO0_11  = 1;
	
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
	return ((AD0GDR>>6)&0x3ff);
}

int main()
{	
	initSysTick();
	ConfigPins();

	while(1) 
	{
	
		DisplayNumber(ReadADC());
		
	}    
}



