/*******************************************************

	Configurationsfunktionen zum

	RGBW_DirectControl Treiber 

	Marcus Knips KW47/2015


*******************************************************/
#include <avr/io.h>
#include "RGBW_DirectControlDef.h"
#include "RGBW_DirectControlConfig.h"


void PortConfig(void){
	// Port A - LCD Display datalines are connected to this Port
	DDRA = 		PA_DDR;	
	
	// Port B - Display Control, PWM_W Cannel 
	DDRB = 		PB_DDR;			
	
	// Port C 
	DDRC =		PC_DDR;
	PORTC |=	(1<<R_LED_YELLOW)|(1<<R_LED_GREEN)|(1<<L_LED_YELLOW)|(1<<L_LED_GREEN);	//LEDs off (= Pin high)

	// Port D - PWM cannels and UART out are outputs, rest inputs
	DDRD =		PD_DDR; 	// OC2, OC1A and OC1B must be set as Output to enable PWM output driver
}

void UartConfig(void){
	UBRRH = (unsigned char) (UBRR_DMX>>8);
	UBRRL = (unsigned char) UBRR_DMX;
	// enable receiver, enable receiver inerrupt
	UCSRB =		(1<<RXEN)|(1<<RXCIE);	
	// 2stop bit, 8 bit
	UCSRC =		(1<<URSEL)|(1<<USBS)|(3<<UCSZ0);

	//Enable receiver mode of the RS485 chip
	PORTC &= 	~((1<<NRE_RS485)|(1<<DE_RS485));	// /RE and DE low
}

void PwmConfig(void){ 	// kontrolliert KW21/2014 -> OK
	//TC0****************
	// Fast PWM Mode, Cleare OC0 on compare match (non inverting), clk/256
	TCCR0 = 	(1<<WGM00)|(1<<WGM01)|(2<<COM00)|(4<<CS00); 
	
	//TC1****************
	// Fast PWM in non inverting mode; 8-bit bottom to top; clk/256
	TCCR1A =	(2<<COM1A0)|(2<<COM1B0)|(1<<WGM10);
	TCCR1B =	(1<<WGM12)|(4<<CS10);

	//TC2****************
	// Fast PWM in non inverting mode; (8-bit bottom to top by default in TC2); clk/256
	TCCR2 =		(1<<WGM20)|(1<<WGM21)|(2<<COM20)|(6<<CS20);
	
	// Initial PWM Values
	OCR1B =		0x8f;		//red
	OCR1A =		0x8f;		//green
	OCR2 =		0x8f;		//blue
	OCR0 = 		0x8f;		//white
}