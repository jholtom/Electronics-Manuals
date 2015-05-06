/* This program presents a counter on a 4 digit seven segment display.
 * The display is refreshed in the main program loop so if program
 * becomes more complex the display will be refreshed less frequently
 * leading to a flickering display.  A better solution will be shown
 * later that uses a periodic interrupt to update the display
 * 
 * Operation:
 * A global array represents the 'video' memory for the display.  
 * In order to display a number, the DisplayNumber function must be 
 * called which breaks the number into its component digits and then
 * translates these to the bit patterns that cause the appropriate
 * display segments to turn on. These patterns are stored in the 
 * global DisplayMemory array.
 * The RefreshDisplay function takes the contents of the DisplayMemory
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
	SYSAHBCLKCTRL |= BIT6 + BIT16; // Turn on clock for GPIO and IOCON 
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
	
}

int main()
{	
	
	ConfigPins();
	int i=0;
	
	while(1) 
	{
	
		DisplayNumber(i++);
		RefreshDisplay();
		if (i > 9999)
			i=0;
		
	}    
}



