#ifndef _MISC_H
#define _MISC_H

#include <stddef.h>

#define TRUE				1
#define FALSE				0

#define global			extern
#define GLOBAL			global

#define unused __attribute__((unused))

char* itoa(int value, char* str, int base);
int memcmp(const void* aptr, const void* bptr, size_t size);

char* itoa(int value, char* str, int base){
	char* rc;
	char* ptr;
	char* low;
	// Check for supported base.
	if(base < 2 || base > 36){
		*str = '\0';
		return str;
	}
	rc = ptr = str;
	// Set '-' for negative decimals.
	if (value < 0 && base == 10){
		*ptr++ = '-';
	}
	// Remember where the numbers start.
	low = ptr;
	// The actual conversion.
	do {
		// Modulo is negative for negative value.
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
		value /= base;
	} while(value);
	// Terminating the string.
	*ptr-- = '\0';
	// Invert the numbers.
	while (low < ptr){
		char tmp = *low;
		*low++ = *ptr;
		*ptr-- = tmp;
	}
	return rc;
}

// memcmp and memcpy wos here

int strlen(char input[]) {
	int i;
	for (i = 0;input[i]!='\0';i++) {}		// Measure length of input[] by looking for \0.
	return i;
}

int strcmp(char* input, char* check) {
  int i,result=1;
	if (strlen(input) != strlen(check)) {
		return 0;
	}
  for (i=0;input[i]!='\0' && check[i]!='\0';i++){
    if(input[i]!=check[i]) {
      result = 0;
      break;
    }
  }
  return result;
}

int strcharfind(char* input, char check, int cont) { // Finds first occurance of check in input.
	static int saveptr;
	int i;
	if(cont == 1) {
		i = saveptr;
	} else {
		i = 0;
	}
  for(;input[i]!='\0';i++){
    if(input[i]==check) {
			saveptr = i;
      return i;
    }
  }
  return -1;
}

void splitstr(char str[], int part, char search, char out[]) {
	int i;
	for(i=0; str[i]!='\0'; i++) {
    if(str[i] == search) {
      break;
    }
  }
	int j;
	int k;
	if(part == 1) {
		for(j = 0; j < i; j++) {
			out[j] = str[j];
  	}
		out[j] = '\0';
	} else if(part == 2) {
		for(k = 0,j = i+1; j <= strlen(str)-1; j++, k++) {
			out[k] = str[j];
  	}
		out[k] = '\0';
	}
}
void delay(int delay) {
	int n = 1;
	while (n < delay) {
		n++;
	}
}

#endif
