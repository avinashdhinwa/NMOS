#include "misc.h"
#include "screen.h"
#include "keyboard.h"
#include "power.h"
#include "idt.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

void parseCommand(char* command);
void terminal();
void help();
void foo();

void* commands[5] = {
	"help",
	&help,
	"reboot",
	&reboot,
	(void *)0x121212			// Command list signiature
};

void fallback() {
}

void main() {
	//char* video_memory = (char*) 0xb8000;

	//*video_memory = 'X';

	int x;

	clearScreen();

	printf(" Welcome to NMOS!\n");
	printfc("  _   _ __  __  ____   _____\n", LGREEN_ON_BLACK);
	printfc(" | \\ | |  \\/  |/ __ \\ / ____|\n", LGREEN_ON_BLACK);
	printfc(" |  \\| | \\  / | |  | | (___  \n", LGREEN_ON_BLACK);
	printfc(" | . ` | |\\/| | |  | |\\___ \\ \n", LGREEN_ON_BLACK);
	printfc(" | |\\  | |  | | |__| |____) |\n", LGREEN_ON_BLACK);
	printfc(" |_| \\_|_|  |_|\\____/|_____/\n\n", LGREEN_ON_BLACK);

	init_idt();

	terminal();

	//setCursor(getOffset(2, 3));

	fallback();
}

void terminal() {
	while (1) {				//Forever
		printf("NMOS:>");
		char* command;
		getText(command);
		delay(1000000);
		parseCommand(command);
		printf("\n");
	}
}

void parseCommand(char* command) {
	int i;
	int success = 0;
	for(i = 0; commands[i] != (void *)0x121212; i+=2) {
		if (strcmp(command, commands[i])) {
			success = 1;
			int (*commandPtr)();
			commandPtr = commands[i+1];
			(*commandPtr)();
		}
	}
	if(success == 0) {
		if(strcmp(command, "")) {
			printf("Command not found!");
		} else {
			printf("Command ");
			printf(command);
			printf(" not found!");
		}
	}
}

void help() {
	int i;
	printf("Commands:");
	for(i = 0; commands[i] != (void *)0x121212; i+=2) {
		printf("\n");
		printf(commands[i]);
	}
}
