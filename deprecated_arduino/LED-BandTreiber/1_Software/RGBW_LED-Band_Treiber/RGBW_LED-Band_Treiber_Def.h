/*******************************************************

	Definitions- und Zuordnungsdatei zum

	RGBW_LED-Band_Treiber

	Marcus Knips KW19/2014

*******************************************************/

#ifndef F_CPU
#define F_CPU           8000000                   // processor clock frequency
//#warning kein F_CPU definiert
#endif

// PORT A
	// An Port A befindet sich der Dip-Wahlschalter zur Kanaleinstellung,
	// wobei Port 0 Schalter Nummer 8 ist. Also in umgekehrter Reihenfolge.
	// Schalter Nummer 9 befindet sich an Port (PB0)
	// Port A ist Eingang, wobei die internen Pullups aktiviert werden müssen.
	#define PA_DDR		0x00	//  input=0
	#define PA_PULL_UP	0xFF	// geschrieben in PORTA aktiviert die internen PullUps

// PORT B
	#define PWM_W		3
	#define S9			0	// DIP Schalter #9
	#define PB_DDR 		(1<<PWM_W)	//output=1, input=0
	#define PB_PULL_UP	(1<<S9)

// PORT C
	#define R_LED_YELLOW	7
	#define R_LED_GREEN		6
	#define L_LED_YELLOW	5
	#define L_LED_GREEN		4
	#define NRE_RS485		3
	#define DE_RS485		2
	#define TWI_SDA			1
	#define TWI_SCL			0
//	#define PC_DDR			(1<<R_LED_YELLOW)|(1<<R_LED_GREEN)|(1<<L_LED_YELLOW)|(1<<L_LED_GREEN)|(1<<NRE_RS485)|(1<<DE_RS485)|(1<<TWI_SDA)|(1<<TWI_SCL)
	#define PC_DDR			0xFF
	// alle Ports sind Ausgänge

// PORT D
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

