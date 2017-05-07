#ifndef _KEKBOARD_H
#define _KEYBOARD_H

#define KEY_NUM						700
#define KEY_DELAY					1000000

char scancode[KEY_NUM];
char printKey();
char getChar();
void getText();
void keyDelay();
char getScancode();

char scancode[KEY_NUM] = {
	0x1b,	// ASCII for escape
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'0',
	'-',
	'=',
	'\b',	// ASCII for backspace
	'\t',	// ASCII for tab
	'q',
	'w',
	'e',
	'r',
	't',
	'y',
	'u',
	'i',
	'o',
	'p',
	'[',
	']',
	'\n',	// ASCII for newline (enter)
	0x80,	// NMOS code for ctrl
	'a',
	's',
	'd',
	'f',
	'g',
	'h',
	'j',
	'k',
	'l',
	';',
	'\'',
	'`',
	0x81,	// NMOS code for shift
	'\\',
	'z',
	'x',
	'c',
	'v',
	'b',
	'n',
	'm',
	',',
	'.',
	'/',
	0x81,	// NMOS code for shift
	'*',
	0x82,	// NMOS code for alt
	' '
};

char getScancode(){
	char c=0;
	do {
		if(inb(0x60)!=c){
			c=inb(0x60);
			if(c>0) {
				return c;
			}
		}
	} while(1);
}

char getChar() {
	return scancode[getScancode() - 1];
}
void getText(char* output) {
	int n = 0;
	while(getChar() != '\n') {
		char key = getChar();
		if (key == '\b') {
			if (n > 0) {
				cursorBack(1);
				printChar(' ');
				cursorBack(1);
				keyDelay();
				n--;
				output[n] = ' ';
			}
		} else if (key == '\n') {

		} else {
			if (n < 20) {
				printChar(key);
				keyDelay();
				output[n] = key;
				n++;
			}
		}
	}
	output[n] = '\0';
	printf("\n");
}

char printKey() {
	char key=getChar();
	printChar(key);
	return key;
}

void keyDelay() {
	int i = 1;
	while(i<KEY_DELAY) { 			// Delay to stop chars getting typed too fast (i.e 1000000 characters per button press)
		i++;
	}
}

#endif
