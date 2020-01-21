/*******************************************************

	Definitions- und Zuordnungsdatei zum

	RGBW_DirectControl Treiber 

	Marcus Knips KW47/2015


*******************************************************/

#ifndef F_CPU
#define F_CPU           8000000                   // processor clock frequency
//#warning kein F_CPU definiert
#endif

// PORT A
	#define	D0			0 	// Datenleitung 0
	#define D1			1 	// Datenleitung 1
	#define D2			2 	// Datenleitung 2
	#define D3			3 	// Datenleitung 3
	#define D4			4 	// Datenleitung 4
	#define D5			5 	// Datenleitung 5
	#define D6 			6  	// Datenleitung 6
	#define D7			7 	// Datenleitung 7
	
	#define PA_DDR		0xff	// Port A is an Output
// PORT B
	#define D_RS		0 	// Display register select
	#define D_RW		1	// Display read / write
	#define D_E			2 	// Display enable
	#define PWM_W		3	// PWM Channel white
	
	#define PB_DDR 		(1<<PWM_W)|(1<<D_E)|(1<<D_RW)|(1<<D_RS)	//output=1, input=0

// PORT C
	#define R_LED_YELLOW	7
	#define R_LED_GREEN		6
	#define L_LED_YELLOW	5
	#define L_LED_GREEN		4
	#define NRE_RS485		3	// RS485 NOT read enable
	#define DE_RS485		2	// RS485 data enable
//	#define PC_DDR			(1<<R_LED_YELLOW)|(1<<R_LED_GREEN)|(1<<L_LED_YELLOW)|(1<<L_LED_GREEN)|(1<<NRE_RS485)|(1<<DE_RS485)|(1<<TWI_SDA)|(1<<TWI_SCL)
	#define PC_DDR			0xFF	// Port C is an Output

// PORT D
	#define DA		2 		// Drehimpulsgeber Taste A
	#define DB		3		// Drehimpulsgeber Taste B
	#define TASTER	6		// Drehimpulsgeber Taster
	#define PWM_B 	7
	#define PWM_G	5
	#define PWM_R	4
	#define TXD_AVR	1
	#define RXD_AVR	0
	#define PD_DDR	(1<<PWM_B)|(1<<PWM_G)|(1<<PWM_R)|(1<<TXD_AVR)	// out=1, in=0

// Default strar condition
	#define RED_VALUE 		0xC0
	#define GREEN_VALUE		0x60
	#define BLUE_VALUE		0x00
	#define WHITE_VALUE		0xFF

	#define RED_VALUE2 		0x10
	#define GREEN_VALUE2	0x90
	#define BLUE_VALUE2		0x20
	#define WHITE_VALUE2	0xA0	

