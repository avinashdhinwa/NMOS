#include "misc.h"
#include "screen.h"
#include "keyboard.h"
#include "power.h"
#include "idt.h"
#include "config.h"
#include "rand.h"
#include "time.h"
#include "vga.h"
#include "sound.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#define GREET_NUM 9

void parseCommand(char* command);
void terminal();
void help(unused char* arg);
void echo(char* arg);
void ping(unused char* arg);
void time(unused char* arg);
void mode(unused char* arg);
void test(char* arg);

struct cmd_entry_t {
    char *cmd_str;
    void (*cmd_fun)(char* arg);
};
typedef struct cmd_entry_t cmd_entry;

cmd_entry commands[20] = {
	{"help", &help},
	{"reboot", &reboot},
  {"echo", &echo},
  {"ping", &ping},
  {"time", &time},
  {"mode", &mode},
  //{"test", &test},
	{(void *)0x121212, NULL}			// Command list signiature
};

char* greetings[] = {
  "It's NMOS! (or is it?)",
  "0% bug free!",
  "Totally no stolen code!",
  "Cool ASCII Art!",
  "OS name?",
  "NMOS is da best!",
  "(;",
  "Give me a N! Give me a M! Give me an O! Give me a S!",
  "SOMN: coming in a parallel universe near you!"
};

void main() {
	clearScreen();

	printf(" Welcome to NMOS ");
  printf(SYS_VER);
  printf("!\n");
	printfc("  _   _ __  __  ____   _____\n", LGREEN_ON_BLACK);
	printfc(" | \\ | |  \\/  |/ __ \\ / ____|\n", LGREEN_ON_BLACK);
	printfc(" |  \\| | \\  / | |  | | (___  \n", LGREEN_ON_BLACK);
	printfc(" | . ` | |\\/| | |  | |\\___ \\ \n", LGREEN_ON_BLACK);
	printfc(" | |\\  | |  | | |__| |____) |\n", LGREEN_ON_BLACK);
	printfc(" |_| \\_|_|  |_|\\____/|_____/\n\n", LGREEN_ON_BLACK);

	init_idt();

  //__asm__("int $0x0");

  srand(second()); // seed psuedo-rng with time
  char* greeting = greetings[rand_uniform(GREET_NUM)];
  printf(greeting);
  printf("\n\n");

  //playSound(1000);

	terminal();
}

void terminal() {
	while (1) {				//Forever
		printf("NMOS:>");
		char command[20];
		getText(command);
		delay(0.1);
		parseCommand(command);
		printf("\n");
	}
}

void parseCommand(char* commandin) {
	int i = 0;
	int success = 0;
  char command[20];
  splitstr(commandin, 1, ' ', command);
  char arg[20];
  splitstr(commandin, 2, ' ', arg);
	for(i = 0; commands[i].cmd_str != (void *)0x121212; i++) {
		if (strcmp(command, commands[i].cmd_str)) {
			success = 1;
			void (*commandPtr)(char* arg);
			commandPtr = commands[i].cmd_fun;
			(*commandPtr)(arg);
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

void help(unused char* arg) {
	int i;
	printf("Commands:");
	for(i = 0; commands[i].cmd_str != (void *)0x121212; i++) {
		printf("\n");
		printf(commands[i].cmd_str);
	}
}

void echo(char* arg) {
  printf(arg);
}

void ping(unused char* arg) {
  printf("Pong!");
}

void mode(unused char* arg) {
  _VGA_switchMode();
}

int contPrompt() {
  prompt_start:
  printf("\nPress 1 to continue, 0 to abort:");
  char c = getChar();
  if(c == '0') {
    printf("\nAborting...");
    return 0;
  } else if(c == '1') {
    return 1;
  } else {
    printf("\nPlease type 0 or 1.");
    goto prompt_start;
  }
}

/*void test(char* arg) {
  if(strcmp(arg, "")) {
    printf("What would you like to test?\n");
    printf("sound\n");
    printf("int");
  } else if(strcmp(arg, "sound")) {
    printf("Testing sound...");
    int result = contPrompt();
    if(!result) {
      return;
    }
    printf("\nSending int 0x0...");
  } else {
    printseq(2, "Unknown argument: ", arg);
  }
}*/

void time(unused char* arg) {
  printf("The time is:\n");
  printi(second(), 10);
  printf(":");
  printi(minute(), 10);
  printf(":");
  printi(hour(), 10);
}
