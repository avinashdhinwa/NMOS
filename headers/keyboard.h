#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#define KEY_NUM						700

unsigned char scancode[KEY_NUM];
char printKey();
char getChar();
void getText(char* out);
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
	0x1b, /* ASCII for escape */ '\0',	// null charachter
	'1','!',
	'2','@',
	'3','\0',
	'4','$',
	'5','%',
	'6','^',
	'7','&',
	'8','*',
	'9','(',
	'0',')',
	'-','_',
	'=','+',
	'\b',	/* ASCII for backspace */ '\0',
	'\t',	/* ASCII for tab */ '\0',
	'q','Q',
	'w','W',
	'e','E',
	'r','R',
	't','T',
	'y','Y',
	'u','U',
	'i','I',
	'o','O',
	'p','P',
	'[','{',
	']','}',
	'\n',	/* ASCII for newline (enter) */ '\0',
	0x80,	/* NMOS code for ctrl */ '\0',
	'a','A',
	's','S',
	'd','D',
	'f','F',
	'g','G',
	'h','H',
	'j','J',
	'k','K',
	'l','L',
	';',':',
	'\'','"',
	'`','~',
	0x81,	/* NMOS code for shift */ '\0',
	'\\','|',
	'z','Z',
	'x','X',
	'c','C',
	'v','V',
	'b','B',
	'n','N',
	'm','M',
	',','<',
	'.','>',
	'/','?',
	0x81,	/* NMOS code for shift */ '\0',	// shift-shift = ???
	'*','\0',
	0x82,	/* NMOS code for alt */ '\0',
	' ','\0'
	// TODO: find out what shift-* and shift-3 actually are
};

void getText(char* out) {
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
				out[n] = ' ';
			}
		} else if (key == '\n') {
			prog = 0;
		} else {
			if (n < 20) {
				printChar(key);
				out[n] = key;
				n++;
			}
		}
	}
	out[n] = '\0';
	printf("\n");
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
