#ifndef _SCREEN_H
#define _SCREEN_H

#include "ports.h"
#include "misc.h"
#include "memory.h"
#include <stddef.h>
#include <stdarg.h>

#define VIDEO_ADDRESS				0xb8000
#define REG_SCREEN_CTRL			0x3D4
#define REG_SCREEN_DATA			0x3D5

/* Define colors */

#define WHITE_ON_BLACK			0x0f /* 0b00001111 */
#define LGREEN_ON_BLACK			0x0a /* 0b00001010 */

#define MAX_ROWS						25
#define MAX_COLS						80
#define MAX_OFFSET					(MAX_ROWS*MAX_COLS+MAX_COLS)*2

#define WILL_SCROLL					offset > MAX_OFFSET - MAX_COLS * 3

int offset;
int getOffset(int x, int y);
int getCursor();
void setCursor(int x, int y);
void __setCursor__(int offset);
void printf(char text[]);
void newLine();
void printi(int input, int base);
void printChar(char c);
void cursorForwards(int num);
void cursorBack(int num);
void scrollDown();

void printf(const char text[]) {
	char* video_memory = (char*) VIDEO_ADDRESS;
	int i;
	for(i = 0; text[i] != 0; i++, offset+=2) {
		if(text[i] == '\n') {
			newLine();
		} else {
			if(text[i] >= 32 && text[i] <= 126) {
				video_memory[offset] = (text[i]);
				video_memory[offset+1] = WHITE_ON_BLACK;
			}
		}
		if (WILL_SCROLL) {
			scrollDown();
		}
	}

	__setCursor__(1+offset);
}


void printChar(char c) {
	char* video_memory = (char*) VIDEO_ADDRESS;
	if(c == '\n') {
		newLine();
	} else {
		if(c >= 32 && c <= 126) {
			video_memory[offset] = c;
			video_memory[offset+1] = WHITE_ON_BLACK;
			offset += 2;
		}
	}

	if (WILL_SCROLL) {
		scrollDown();
	}

	__setCursor__(1+offset);
}

void printfc(const char text[], char attr) {
	char* video_memory = (char*) VIDEO_ADDRESS;
	int i;
	for(i = 0; text[i] != 0; i++, offset+=2) {
		if(text[i] == '\n') {
			newLine();
		} else {
			video_memory[offset] = (text[i]);
			video_memory[offset+1] = attr;
		}
		if (WILL_SCROLL) {
			scrollDown();
		}
	}

	__setCursor__(1+offset);
}

int getOffset(int x, int y) {
	return (y*MAX_COLS+x)*2;
}

void __setCursor__(int offset) {

	offset /= 2;

	outb(REG_SCREEN_CTRL, 14);
	outb(REG_SCREEN_DATA, (unsigned char)(offset >> 8) & 0xff);
	outb(REG_SCREEN_CTRL, 15);
	outb(REG_SCREEN_DATA, offset & 0xff);
}

void setCursor(int x, int y) {
	int __offset__ = getOffset(x, y);
	__setCursor__(__offset__);
	offset = __offset__;
}

void newLine() {
	int rows = offset / (2 * MAX_COLS);
	setCursor(80, rows);
}

int getCursor() {
	outb(REG_SCREEN_CTRL, 14);
	int offset = inb(REG_SCREEN_DATA) << 8;
	outb(REG_SCREEN_CTRL, 15);
	offset += inb(REG_SCREEN_DATA);

	return offset * 2;
}

void clearScreen(){
	int row = 0;
	int col = 0;

	char* video_memory = (char*) VIDEO_ADDRESS;

	for(row = 0; row < MAX_ROWS; row ++){
		for(col = 0; col < MAX_COLS; col ++){
			video_memory[getOffset(col, row)] = ' ';
			video_memory[getOffset(col, row)+1] = WHITE_ON_BLACK;
		}
	}

	setCursor(0, 0);
}

void printi(int input, int base) {
	char buf[64];
	printf(itoa(input, buf, base));
}

void cursorBack(int num) {
	int i = 1;
	while (i <= num) {
		offset -= 2;
		__setCursor__(offset);
		i++;
	}
}

void cursorForwards(int num) {
	int i = 1;
	while (i <= num) {
		offset += 2;
		__setCursor__(offset);
		i++;
	}
}

void scrollDown() {
	char* video_memory = (char*) VIDEO_ADDRESS;
	size_t row_s = (size_t) MAX_COLS * 2;

	// Copy rows

	for(int row = 0; row <= MAX_ROWS; row++){
		memcpy(video_memory + getOffset(0, row), video_memory + getOffset(0, row+1), row_s);
	}

	//Move cursor back

	cursorBack(MAX_COLS);
}

#endif
