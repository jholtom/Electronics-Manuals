/* This program puts the output from the ADC onto the IO Pins */

#include "lpc111x.h"
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
    // Port 1 bit 6 -> generic io
    IOCON_PIO1_6 |= BIT0;
    // Port 1 bit 7 -> generic io
    IOCON_PIO1_7 |= BIT0;
    // Port 1 bit 8 -> generic io
    IOCON_PIO1_8 |= BIT0;
    
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
    return ((AD0GDR>>6)&0x3ff);
}
void Output(int data)
{
    //GPIO1DATA = data;
    GPIO1DATA |= BIT1;
    GPIO1DATA |= BIT2;
    GPIO1DATA |= BIT3;
    GPIO1DATA |= BIT4;
    GPIO1DATA |= BIT5;
    GPIO1DATA |= BIT6;
    GPIO1DATA |= BIT7;
    GPIO1DATA |= BIT8;
    GPIO1DATA &= ~BIT4;
    GPIO1DATA &= ~BIT6;
    GPIO1DATA &= ~BIT8;
}
int main()
{	
    ConfigPins();

//    while(1) 
  //  {
        Output(ReadADC());
  //  }    
}
