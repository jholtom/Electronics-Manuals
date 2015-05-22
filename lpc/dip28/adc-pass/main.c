/* This program puts the output from the ADC onto the IO Pins */

#include "lpc111x.h"
#include <stdint.h>
#include <stdlib.h>
#include "serial.h"
#define TABLE_SIZE 1024
#define TWO_PI (3.14159 * 2)
float phaseIncrement = TWO_PI/TABLE_SIZE;
void initSysTick(void);
void delay(int dly)
{
	while (dly--);
}
void Output(uint8_t data)
{
	GPIO1DATA = data;
}
double sine_taylor(double x)
{
    double x2 = x*x;
    double x4 = x2*x2;
    double t1 = x * (1.0 - x2 / (2*3));
    double x5 = x * x4;
    double t2 = x5 * (1.0 - x2 / (6*7)) / (1.0* 2*3*4*5);
    double x9 = x5 * x4;
    double t3 = x9 * (1.0 - x2 / (10*11)) / (1.0* 2*3*4*5*6*7*8*9);
    double x13 = x9 * x4;
    double t4 = x13 * (1.0 - x2 / (14*15)) / (1.0* 2*3*4*5*6*7*8*9*10*11*12*13);
    double result = t4;
    result += t3;
    result += t2;
    result += t1;
    return result;
}
int cnt = 0; //counting for the sine wave generation
void SysTick(void){
	uint8_t data = 0;
	uint8_t out = 0;
	float currentPhase = 0.0;
	int val = 0;
	char str[5];
	data = ReadADC();
	out = (uint8_t) (data * sine_taylor(currentPhase));
	if(cnt == TABLE_SIZE)
	{
		cnt = 0;
	}
	else{
	cnt += 1;
	}
	//	temp = data & 0x3ff;
	//	dec_to_str(str,temp,3u);	
	//	printString(str);
	//	printString("\r\n");
	Output(out);
}
void initSysTick(){
	SYST_CSR |= (BIT0+BIT1+BIT2);
	SYST_RVR=1088-1;
	SYST_CVR=5;
	enable_interrupts();
}
void dec_to_str (char* str, uint8_t val, size_t digits)
{
	size_t i=1u;
	for(; i<=digits; i++)
	{
		uint8_t ch = (uint8_t)(val % 10u);
		ch = (uint8_t)(ch + '0');
		str[digits-i] = (char)ch;
		val/=10u;
	}
	str[i-1u] = '\0'; // assuming you want null terminated strings?
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
int main()
{	
	initSysTick();
	//	initUART();
	//	enable_interrupts();
	ConfigPins();
	while(1){
		delay(10);
	}
}
