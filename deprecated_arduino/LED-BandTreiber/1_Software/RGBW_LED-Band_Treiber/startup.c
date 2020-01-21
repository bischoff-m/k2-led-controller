void startup(void) {
	unsigned char k;
	unsigned short z;

	// WHITE from 0% to 100% and back to 50%
	for (k=0;k<255;k++){
		OCR0 =	eye_correction[k];
		for(z=0;z<=10000;z++) {asm volatile ("nop");}
	}
	for (k=255;k>128;k--){
		OCR0 =	eye_correction[k];
		for(z=0;z<=10000;z++) {asm volatile ("nop");}
	}	

	// RED from 0% to 100% and back to 0%
	for (k=0;k<255;k++){
		OCR1B =	eye_correction[k];
		for(z=0;z<=10000;z++) {asm volatile ("nop");}
	}
	for (k=255;k>0;k--){
		OCR1B =	eye_correction[k];
		for(z=0;z<=10000;z++) {asm volatile ("nop");}
	}	

	// GREEN from 0% to 100% and back to 0%
	for (k=0;k<255;k++){
		OCR1A =	eye_correction[k];
		for(z=0;z<=10000;z++) {asm volatile ("nop");}
	}
	for (k=255;k>0;k--){
		OCR1A =	eye_correction[k];
		for(z=0;z<=10000;z++) {asm volatile ("nop");}
	}	

	// BLUE from 0% to 100% and back to 0%
	for (k=0;k<255;k++){
		OCR2 =	eye_correction[k];
		for(z=0;z<=10000;z++) {asm volatile ("nop");}
	}
	for (k=255;k>0;k--){
		OCR2 =	eye_correction[k];
		for(z=0;z<=10000;z++) {asm volatile ("nop");}
	}	

	// RGB (=white) from 0% to 50% 
	for (k=0;k<128;k++){
		OCR1B =	eye_correction[k];
		OCR1A =	eye_correction[k];
		OCR2 =	eye_correction[k];
		for(z=0;z<=10000;z++) {asm volatile ("nop");}
	}
	
}
