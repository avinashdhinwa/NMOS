#include "misc.h"
#include "screen.h"
#include "keyboard.h"
#include "power.h"
#include "idt.h"
#include "config.h"
#include "rand.h"
#include "time.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#define GREET_NUM 8

void parseCommand(const char* command);
void terminal();
void help(unused const char* arg);
void echo(const char* arg);
void ping(unused const char* arg);
void time(unused const char* arg);
void reboot(unused const char* arg);

struct cmd_entry_t {
    const char *cmd_str;
    void (*cmd_fun)(const char* arg);
};
typedef struct cmd_entry_t cmd_entry;

cmd_entry commands[20] = {
	{"help", &help},
	{"reboot", &reboot},
  {"echo", &echo},
  {"ping", &ping},
  {"time", &time},
	{(const char *)0x121212, NULL}			// Command list signiature
};

const char* greetings[] = {
  "It's NMOS! (or is it?)",
  "0% bug free!",
  "Totally no stolen code!",
  "Cool ASCII Art!",
  "OS name?",
  "NMOS is da best!",
  "(;",
  "Give me a N! Give me a M! Give me an O! Give me a S!"
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

  srand(second()); // seed psuedo-rng with time
  const char* greeting = greetings[rand_uniform(GREET_NUM)];
  printf(greeting);
  printf("\n\n");
	terminal();
}

void terminal() {
	while (1) {				//Forever
		printf("NMOS:>");
		char command[20];
		getText(command);
		delay(1000000);
		parseCommand(command);
		printf("\n");
	}
}

void parseCommand(const char* commandin) {
	int i = 0;
	int success = 0;
  char command[20];
  splitstr(commandin, 1, ' ', command);
  char arg[20];
  splitstr(commandin, 2, ' ', arg);
	for(i = 0; commands[i].cmd_str != (void *)0x121212; i++) {
		if (strcmp(command, commands[i].cmd_str)) {
			success = 1;
			void (*commandPtr)(const char* arg);
			commandPtr = commands[i].cmd_fun;
			(*commandPtr)(arg);
		}
	}
	if(success == 0) {
		if(strcmp(command, "")) {
			printf("Command not found!");
		} else {
			printf("Command ");
			printf((const char*)command);
			printf(" not found!");
		}
	}
}

void help(unused const char* arg) {
	int i;
	printf("Commands:");
	for(i = 0; commands[i].cmd_str != (void *)0x121212; i++) {
		printf("\n");
		printf(commands[i].cmd_str);
	}
}

void echo(const char* arg) {
  printf(arg);
}

void ping(unused const char* arg) {
  printf("Pong!");
}

void time(unused const char* arg) {
  printf("The time is:\n");
  printi(hour(), 10);
  printf(":");
  printi(minute(), 10);
  printf(":");
  printi(second(), 10);
}

void reboot(unused const char* arg) {
  restart();
}
