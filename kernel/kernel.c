#include "../drivers/misc.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"
#include "../drivers/power.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void parseCommand(char* command);
void terminal();

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
	
	terminal();
	
	//setCursor(getOffset(2, 3));
	
	fallback();
}

void terminal() {
		while (1) {				//Forever
			printf("NMOS:>");
			char* command;
			getText(command);
			char* buffer;
			delay(1000000);
			parseCommand(command);
			printf("\n");
		}
}

void parseCommand(char* command) {
	//command = "reboot";
	if (strcmp(command, "reboot")) {
		printf("Rebooting...");
		delay(1000000);
		reboot();
	} else {
		printf("Command not found! Type \'help\' for help!");
	}
}