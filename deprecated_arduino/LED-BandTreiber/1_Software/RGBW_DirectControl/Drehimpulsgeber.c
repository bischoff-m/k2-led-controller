
#include <stdint.h>		// Datentypen
#include "RGBW_DirectControlDef.h"

// um Drucktaster erweitern!

// Declarations
void RightTurn(void);
void LeftTurn(void);

IncrementalEncoder(static uint8_t keyPressFuncFunc, ){

	static uint8_t	keyPressAltFunc;
	static uint8_t 	zustandAltFunc;
	
	keyPressAltFunc = keyPressFunc; 	//save current keys

	// Detect new state
	if(((keyPressFunc & (1<<DA)) ==0) && ((keyPressFunc & (1<<DB)) ==0)) {zustandFunc =	0;
		keyPressFunc &= ~((1<<DA)|(1<<DB));	// clear keyPressFunc
		}
	else if(((keyPressFunc & (1<<DA)) ==0) && ((keyPressFunc & (1<<DB)) ==1)) {zustandFunc =	1;
		keyPressFunc &= ~((1<<DA)|(1<<DB));	// clear keyPressFunc
		}
	else if(((keyPressFunc & (1<<DA)) ==1) && ((keyPressFunc & (1<<DB)) ==0)) {zustandFunc =	2;
		keyPressFunc &= ~((1<<DA)|(1<<DB));	// clear keyPressFunc
		}
	else if(((keyPressFunc & (1<<DA)) ==1) && ((keyPressFunc & (1<<DB)) ==1)) {zustandFunc =	3;
		keyPressFunc &= ~((1<<DA)|(1<<DB));	// clear keyPressFunc
		}
	else{zustandFunc = 0;}	

	
	if(zustandFunc != zustandAltFunc){
			switch(zustandFunc){
				case 0:	
					if(zustandAltFunc == 1){RightTurn();}
					if(zustandAltFunc == 2){LeftTurn();}
				break;
				
				case 1:
					if(zustandAltFunc == 3){RightTurn();}
					if(zustandAltFunc == 0){LeftTurn();}
				break;
				
				case 2:
					if(zustandAltFunc == 0){RightTurn();}
					if(zustandAltFunc == 3){LeftTurn();}
				break;
				
				case 3:
					if(zustandAltFunc == 2){RightTurn();}
					if(zustandAltFunc == 1){LeftTurn();}
				break;				
			}
			zustandAltFunc 	= zustandFunc;	//save current state
	}
}
