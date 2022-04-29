/*
*	RGBW LED-Band Treiber
*
*	Marcus Knips Mai 2013
*
*/

// Include**************************************
#include <avr/io.h>
#include <avr/interrupt.h>
#include "RGBW_LED-Band_Treiber_Def.h"
#include "eye_correction.h"

//#include "startup.c"

// Variaben*************************************
// Info avr-gcc:
// unsigned char = 0..255
// unsigned short, unsigned int = 0..65535
volatile unsigned char 	dmxData[512];
volatile unsigned short channelCounter;
volatile unsigned short dmxChannel;

volatile unsigned short port, port_alt;


// Constants************************************
//const unsigned char sieben_seg[10] = {
//	0xC0,	0xF9,	0xA4,	0xB0,	0x99,	0x92,	0x82,	0xF8,	0x80,	0x90
//	}; // 0 to 9; MSB activate DOT		// low level will light the led
volatile const unsigned char sieben_seg[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x99, 0x6D, 0x7D, 0x07, 0x7F, 0x6F}; // High level will light the led	
volatile const unsigned char UBRR_DMX = 	1;	// Baud 250000 for DMX(RS485) comunication

//Inerrupt Routinen*****************************

ISR(USART_RXC_vect){	// with 250 kBaut and 10Bit per canel all 40us
	cli();	// deactuvate inerrupts
	//Normal code begin
	dmxData[channelCounter] =	UDR;
	channelCounter++;
	
	// Sprechstunde KawoNet
	// ACHTUNG ENTFERNEN
	dmxData[256] = eye_correction[0xFF];
	dmxData[257] = eye_correction[0xFF];
	dmxData[258] = eye_correction[0xE0];
	dmxData[259] = eye_correction[0xFF];
	// Ende Sprechstunde

	sei();	// reactivate inerrupts
}


//Funktionen************************************

void PortConfig(void){
	// Port A ist Eingang f�r DMX Kanal-wahl Bit0 bis Bit7; Bit8 ist PB0 ; Achtung: umgekehte Reihefolge
	DDRA = 		0x00;		// Complete Port A is Input
	PORTA =		0xFF;		// Aktivate all internal Pull Up Resistors for PortA
	
	// Port B nur PB0 Eingang, der Rest wird duch andere Funktionen Definiert
	DDRB = 		PB_DDR;		// PWM_W (PB3=OC1) is output	
	PORTB |=	PB_PULL_UP;	// S9 (PB0) activate Pull Up
	
	// Port C hat nur Ausg�nge
	DDRC =		PC_DDR;		// Complete Port C is an output
	PORTC |=	(1<<R_LED_YELLOW)|(1<<R_LED_GREEN)|(1<<L_LED_YELLOW)|(1<<L_LED_GREEN);	//LEDs off (= Pin high)
	PORTC &=	~((1<<TWI_SDA)|(1<<TWI_SCL));		// Data lines low

	// Port D wird von Funktionen gesteuert
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
}


// DIP-Switch at PORTA is with MSB First 
// This Routin changes the order.
unsigned short GetSelectedDmxChannel(void) {
	unsigned short 	y;
	unsigned char 	x, t, ShiftIndex, i;
	y= 	0;
	x = ~PINA;
	for(i=0;i<4;i++){
		ShiftIndex = ((i+1)<<1)-1;	// 1,3,5,7

		t =		(1<<(4+i));			// Maske Bilden
		t &= 	x;					// Datenbit herrausnehmen
		t = 	(t>>ShiftIndex); 	// high nible to low nible
		y |=	t;				

		t = 	(1<<(3-i));			// Maske auf low nible
		t &= 	x;					// Datenbit herrausnehmen
		t = 	(t<<ShiftIndex); 	// low nible to high nible
		y |= 	t;

	}
	// Bit number 9....
	x = 	~PINB;
	x &= 	(1<<S9);	// Mask to the Bit from S9
	y |= 	(x<<8);		// S9 Bit to Bit-positon 8 in y	
	return y;
}


void WriteDisplay(unsigned int channel) {
//	unsigned int temp, temp10, k;
	unsigned char d0, d1, d2, i;
	unsigned char temp =0 ;

	// Umformung in Dezimalstellen
/*	temp = 	channel;
	k =	0;
	while(~(SREG & (1<<2))){	// ziehe so oft 100 ab, bis negativ
		temp =	temp-100;
		k++;					// z�hle die abgezogenen 100er
	}
	d2 =	k-1;				// in d2 stehen die ganzzahligen 100er
	temp =	channel - (d2*100);	//ganzzahlige 100er anbziehen
	temp10 = temp;
	k=0;
	while(~(SREG & (1<<2))){
		temp =	temp-10;
		k++;
	}
	d1 =	k-1;				// in d1 stehen die ganzzahligen 10er
	temp = 	temp10 - (d1*10);
	k =	0;
	while(~(SREG & (1<<2))){
		temp =	temp-1;
		k++;
	}
	d0 =	k-1;				// in d0 stehen die 1er
*/

	d2 =		channel/100;			// hunderter	
	d1 =		(channel-d2*100)/10;	// zehner
	d0 =		channel % 10;			// einer

	// Schreiben auf Display; MSB voraus
	// Schreiben der dritten Stelle
	for(i=0;i<=7;i++){
		temp = 	sieben_seg[d2] & (1<<(7-i)); 		// Maske! Beginnend mit MSB
		temp = 	(temp>>(7-i));						// Bit schieben auf Position Null
		if(temp) 	{PORTC |= 	(1<<TWI_SDA);}		// If "1" SDA high
		else 		{PORTC &=	~(1<<TWI_SDA);}		// If "0" SDA low
		asm volatile ("nop");						// 2 Takte Warten
		asm volatile ("nop");						// 2 Takte Warten
		PORTC |= 	(1<<TWI_SCL);					// SCL high
		asm volatile ("nop");						// 2 Takte Warten
		asm volatile ("nop");						// 2 Takte Warten
		PORTC &=	~(1<<TWI_SCL);					// SCL low
	}
	// Schreiben der zweiten Stelle
	for(i=0;i<=7;i++){
		temp = 	sieben_seg[d1] & (1<<(7-i)); 		// Maske! Beginnend mit MSB
		temp = 	(temp>>(7-i));						// Bit schieben auf Position Null
		if(temp) 	{PORTC |= 	(1<<TWI_SDA);}		// If "1" SDA high
		else 		{PORTC &=	~(1<<TWI_SDA);}		// If "0" SDA low
		asm volatile ("nop");						// 2 Takte Warten
		asm volatile ("nop");						// 2 Takte Warten
		PORTC |= 	(1<<TWI_SCL);					// SCL high
		asm volatile ("nop");						// 2 Takte Warten
		asm volatile ("nop");						// 2 Takte Warten
		PORTC &=	~(1<<TWI_SCL);					// SCL low
	}
	// Schreiben der ersten Stelle
	for(i=0;i<=7;i++){
		temp = 	sieben_seg[d0] & (1<<(7-i)); 		// Maske! Beginnend mit MSB
		temp = 	(temp>>(7-i));						// Bit schieben auf Position Null
		if(temp) 	{PORTC |= 	(1<<TWI_SDA);}		// If "1" SDA high
		else 		{PORTC &=	~(1<<TWI_SDA);}		// If "0" SDA low
		asm volatile ("nop");						// 2 Takte Warten
		asm volatile ("nop");						// 2 Takte Warten
		PORTC |= 	(1<<TWI_SCL);					// SCL high
		asm volatile ("nop");						// 2 Takte Warten
		asm volatile ("nop");						// 2 Takte Warten
		PORTC &=	~(1<<TWI_SCL);					// SCL low
	}
	
	// Additonal Takt
	PORTC &=	~(temp<<TWI_SDA);
	asm volatile ("nop");						// 2 Takte Warten
	asm volatile ("nop");						// 2 Takte Warten
	PORTC |= 	(1<<TWI_SCL);					// SCL high
	asm volatile ("nop");						// 2 Takte Warten
	asm volatile ("nop");						// 2 Takte Warten
	PORTC &=	~(1<<TWI_SCL);					// SCL low

}

void InitDisplay(void) {
		PORTC &= 	~(1<<TWI_SDA);						// Data line zero "0"
		for(unsigned char i=0;i<=21;i++) {
			PORTC |= 	(1<<TWI_SCL);					// SCL high
			asm volatile ("nop");						// 2 Takte Warten
			asm volatile ("nop");						// 2 Takte Warten
			PORTC &=	~(1<<TWI_SCL);					// SCL low
			asm volatile ("nop");						// 2 Takte Warten
			asm volatile ("nop");						// 2 Takte Warten
		}
}

void Rj45Leds(void) {
	static unsigned char ledCount =0;
	if(ledCount > 50) {	// ->every second
		PORTC ^= 	(1<<R_LED_YELLOW)|(1<<R_LED_GREEN)|(1<<L_LED_YELLOW)|(1<<L_LED_GREEN); 	//toggle LEDs
		ledCount =	0;
	}
	ledCount++;
}


// Main Programm********************************

int main(void) {

	unsigned char muell;

	PortConfig();
	PwmConfig();
	// Start Values for PWM
		OCR1B =		0xA0;//200;			//red
		OCR1A =		0x20;//64;//128;	//green
		OCR2 =		0;//128;			//blue
		OCR0 = 		0xA0;			//white
	UartConfig();
	dmxChannel = 	GetSelectedDmxChannel();

//	dmxChannel = 1;	//Testing

	WriteDisplay(dmxChannel);
	PORTC &= 	~((1<<R_LED_YELLOW)|(1<<R_LED_GREEN)|(1<<L_LED_YELLOW)|(1<<L_LED_GREEN)); // All LEDs on
//	startup();	// Test Routin. After this Routin all Lights are set to 50%
	InitDisplay();
	port = 1;
	port_alt = 0;
	sei();		// Global interrupt enable

	while(1) {

		//Common code begin 
		

		// If DIP-Switch changes, read selected DMX Chanel and write the Chanel to the Display
		// (Bit 8 of DIP-Switch is not considered yet)
		port = PINA | ((PINB & (1<<S9)) << 8);	// The ninth bit is in Port B on position S9 (=Bit 0)
		if (port != port_alt) {
			cli();
			port_alt = PINA | ((PINB & (1<<S9)) << 8);
			dmxChannel = 	GetSelectedDmxChannel();
			InitDisplay();
			WriteDisplay(dmxChannel);
		
			sei();
		}
			
		// Detecting DMX Break with frame error fleg
		// Will be called approx. every 20ms with 250kBoud and 512 DMX
		if (UCSRA & (1<<FE)) {
			channelCounter =	0;	// Initialize dmx channel counter
			OCR1B =	eye_correction[dmxData[dmxChannel]];		// PWM R
			OCR1A = eye_correction[dmxData[dmxChannel + 1]];	// PWM G
			OCR2 = 	eye_correction[dmxData[dmxChannel + 2]];	// PWM B
			OCR0 =	eye_correction[dmxData[dmxChannel + 3]];	// PWM W
			muell =	UDR;	// reading UDR set the error fleg to zero
			Rj45Leds();
		}
		
		// Abfangen, falls kein DMX Kanal gew�hlt ist (dmxCannel = 0)
		if(dmxChannel ==0){
			cli();	// Somit DMX off

			OCR1B =		0xA0;//200;			//red
			OCR1A =		0x20;//64;//128;	//green
			OCR2 =		0;//128;			//blue
			OCR0 = 		0xA0;				//white
		}
		else	sei();


		//Common code end
		
 	}

}


