#include "../drivers/misc.h"
#include "../drivers/screen.h"
#include "../drivers/keyboard.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void fallback() {	
}

void main() {
	//char* video_memory = (char*) 0xb8000;
	
	//*video_memory = 'X';
	
	clearScreen();
	
	printf(" Welcome to NMOS!\n");
	printfc("   _   _ __  __  ____   _____\n", LGREEN_ON_BLACK);
	printfc(" | \\ | |  \\/  |/ __ \\ / ____|\n", LGREEN_ON_BLACK);
	printfc(" |  \\| | \\  / | |  | | (___  \n", LGREEN_ON_BLACK);
	printfc(" | . ` | |\\/| | |  | |\\___ \\ \n", LGREEN_ON_BLACK);
	printfc(" | |\\  | |  | | |__| |____) |\n", LGREEN_ON_BLACK);
	printfc(" |_| \\_|_|  |_|\\____/|_____/\n\n", LGREEN_ON_BLACK);
	printf("Press 1 for terminal, or 2 for GUI [1]:");
	//printi(getScancode(), 16);
	getText();
	
	//setCursor(getOffset(2, 3));
	
	fallback();
}
