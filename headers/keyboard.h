#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#define KEY_NUM						700

unsigned char scancode[KEY_NUM];
char printKey();
char getChar();
char* getText();
char getScancode();

int capsStatus = 0;

struct key_packet_s {
	uint8_t keyflags;
	char key;
};

/*
keyflags:

bit 1: pressed (1) / released (0)
bits 2-8: unused
*/

typedef struct key_packet_s key_packet;

key_packet curPacket;
key_packet blankPacket;

unsigned char scancode[KEY_NUM] = {
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

char* getText() {
	char* output;
	int key = 0;
	int n = 0;
	int prog = 1;
	while(prog) {
		char key = getChar();
		if (key == '\b') {
			if (n != 0) {
				cursorBack(1);
				printChar(' ');
				cursorBack(1);
				n--;
				output[n] = ' ';
			}
		} else if (key == '\n') {
			prog = 0;
		} else {
			if (n < 20) {
				printChar(key);
				output[n] = key;
				n++;
			}
		}
	}
	output[n] = '\0';
	printf("\n");
	return output;
}

char getChar() {
	while (curPacket.key == 0 || curPacket.keyflags & 0b00000001) {}
	char key = curPacket.key;
	curPacket = blankPacket;
	return key;
}

char printKey() {
	char key=getChar();
	printChar(key);
	return key;
}

#endif
