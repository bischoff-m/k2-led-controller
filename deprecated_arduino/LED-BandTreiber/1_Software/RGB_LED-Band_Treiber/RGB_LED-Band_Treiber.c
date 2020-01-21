/*
 * RGB_LED_Band_Treiber.c
 *
 * Created: 01.03.2013 16:00:04
 *  Author: mkn
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

//Variables***************************
unsigned char state;
unsigned char usart_selection;
unsigned char uart_data[3];
unsigned char dmx_data[512];
unsigned int channel_counter;
unsigned char i;


//Constants
const unsigned char sieben_seg[10] = {
	0xC0,	0xF9,	0xA4,	0xB0,	0x99,	0x92,	0x82,	0xF8,	0x80,	0x90
	}; // 0 to 9; MSB activate DOT	
const unsigned char UBRR_PC = 51;	// Baud 9600 for PC(USB) comunication
const unsigned char UBRR_DMX = 	1;	// Baud 250000 for DMX(RS485) comunication

//Interrupt Routinen******************

ISR(USART_RXC_vect){
	// If PWREN(=PB2) is LOW an usb cable is connected
//	if(!(PORTB & (1<<PB2))){
//		uart_data[state] =	(unsigned char)	UDR;
//		PORTA =		sieben_seg[state];
//		state =	state+1;
//		if(state > 2) {
//			state = 	0;
//		}
//		OCR1B =	uart_data[0];	// PWM Red
//		OCR1A = uart_data[1];	// PWM Green
//		OCR2 = 	uart_data[2];	// PWM Blue
//	}
//	else {
		dmx_data[channel_counter] =	UDR;
		channel_counter = channel_counter+1;
//		PORTA = 	sieben_seg[2];
//	}
}

ISR(TIMER0_COMP_vect) {
	OCR1B =	dmx_data[4];	// PWM Red
	OCR1A = dmx_data[5];	// PWM Green
	OCR2 = 	dmx_data[6];	// PWM Blue
}


//Functions***************************

void config(void){
	// Port direction
	DDRA =	(1<<PA7)|(1<<PA6)|(1<<PA5)|(1<<PA4)|(1<<PA3)|(1<<PA2)|(1<<PA1)|(1<<PA0);	//Port A Output
	
	DDRB &=		~(1<<PB2);	//Pin PB2 is input (CBUS3 of FTDI Chip; PWREN#)
	PORTB &=	~(1<<PB2);	// deactivate internal PullUp Resistor	
	
	DDRC =		(1<<PC4)|(1<<PC3)|(1<<PC2);	// /CAN_EN, /RE_RS485, DE_RS485
	PORTC =		(1<<PC4);	// activate internal PullUp for /CAN_EN. Transmitter is off by default
	
	DDRD &=		~(1<<PD3)|~(1<<PD2);	//CBUS2(TXEN) and /INT MCP2515 are inputs
	DDRD =		(1<<PD7)|(1<<PD5)|(1<<PD4); // OC2, OC1A and OC1B must be set as Output to enable PWM output driver
	PORTD &=	~(1<<PD3)|~(1<<PD2);	// disable internal PullUp Resistor
}

void pwm_config(void){
	//TC1
	// Fast PWM in non inverting mode; 8-bit bottom to top; clk/256
	TCCR1A =	(2<<COM1A0)|(2<<COM1B0)|(1<<WGM10);
	TCCR1B =	(1<<WGM12)|(4<<CS10);
	// Initialization of Output Compare Register's to 50% duty cycle
	OCR1A =		0xF0;	// =50% of 0xFF
	OCR1B =		0xF0;	// =5ß% of 0xFF
	
	//TC2
	// Fast PWM in non inverting mode; (8-bit bottom to top by default in TC2); clk/256
	TCCR2 =		(1<<WGM20)|(1<<WGM21)|(2<<COM20)|(6<<CS20);
	// Initialization of Output Compare Register
	OCR2 =		0x20;	// =50% of 0xFF
	}

void timer_conter_0_config(void) {
	// Timer Counter 0
	// CTC,clk/1024
	TCCR0 = 	(2<<WGM00)|(3<<CS00);
	TIMSK = 	(1<<OCIE0);	// Compare Inerrupt Enalbe
}
	
void uart_config(unsigned char usart_selection){
	unsigned char 	sreg;
//	sreg 	= 	SREG;		// Save Global Interrupt Flag
//	cli();					// Disable global interrupt
	// Check TXC and RXC Flag
	// Set baud rate
	if(usart_selection) {
		UBRRH = (unsigned char) (UBRR_PC>>8);
		UBRRL = (unsigned char) UBRR_PC;
		// enable receiver, enable receiver inerrupt
		UCSRB =		(1<<RXEN)|(1<<RXCIE);	
		// 1stop bit, 8 bit
		UCSRC =		(1<<URSEL)|(3<<UCSZ0);
		// disable R485 chip
		PORTC = 	(1<<PC3);	// /RE = High
		PORTC &=	~(1<<PC2); 	// DE = Low
	}
	else {
		UBRRH = (unsigned char) (UBRR_DMX>>8);
		UBRRL = (unsigned char) UBRR_DMX;
		// enable receiver, enable receiver inerrupt
		UCSRB =		(1<<RXEN)|(1<<RXCIE);	
		// 2stop bit, 8 bit
		UCSRC =		(1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
		//Enable receiver mode of the RS485 chip
		PORTC &= 	~(1<<PC3)|~(1<<PC2);	// /RE and DE low

		PORTA = sieben_seg[8];
	}
//	SREG 	=	sreg;	// Restore Global Interrupt Flag
	//sei();
}

int main(void)
{
	unsigned char temp;
	config();
	pwm_config();
	uart_config(0);		// Config for DMX reception
	PORTA = 	sieben_seg[4] & ~(1<<7);
	sei();		// Global interrupt enable
//	PORTA = sieben_seg[7];
	while(1) {
//		// If PWREN(=PB2) is LOW an usb cable is connected
//		if(!(PORTB & (1<<PB2))){
//			uart_config(1);		// Config UART for communication with the PC
//		}

		// Detecting DMX Break with frame error fleg
		if (UCSRA & (1<<FE)) {
			channel_counter =	0;	// Initialize dmx channel counter
			dmx_data[0] =	UDR;	// reading UDR set the error fleg to zero
			OCR1B =	dmx_data[4];	// PWM Red
			OCR1A = dmx_data[5];	// PWM Green
			OCR2 = 	dmx_data[6];	// PWM Blue
		}
		else{
			OCR1B =		0xA0;	// Red
			OCR1A =		0x20;	// Green
			OCR2 =		0x00;;		// Blue
		}
		temp = PORTC;
		PORTA = temp;
		for(i=1;i<3;i++){
			temp = i;
		}
 	}
}
