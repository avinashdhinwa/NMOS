// Depends on:
// Screen.h

#define KEY_NUM 700
#define KEY_DELAY 900000

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
	'/'
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
void getText() {
	while(1) {
		printascii(getChar());
		int i = 1;
		while(i<KEY_DELAY) { 			// Delay to stop chars getting typed too fast (i.e 1000000 characters per button press)
			i++;
		}
	}
}