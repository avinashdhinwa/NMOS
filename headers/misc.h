#include <stddef.h>

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

// todo: move to new file memory.h

int memcmp(const void* aptr, const void* bptr, size_t size) {
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
	for (size_t i = 0; i < size; i++) {
		if (a[i] < b[i])
			return -1;
		else if (b[i] < a[i])
			return 1;
	}
	return 0;
}

void* memcpy(void* restrict dstptr, const void* restrict srcptr, size_t size) {
	unsigned char* dst = (unsigned char*) dstptr;
	const unsigned char* src = (const unsigned char*) srcptr;
	for (size_t i = 0; i < size; i++)
		dst[i] = src[i];
	return dstptr;
}

int strlen(char input[]) {
	int i;
	for (i = 0;input[i]!='\0';i++) {}		// Measure length of input[] by looking for \0.
	return i;
}

int strcmp(char* input, char* check)
{
    int i,result=1;
	if (strlen(input) != strlen(check)) {
		return 0;
	}
    for (i=0;input[i]!='\0' && check[i]!='\0';i++){
        if(input[i]!=check[i]){
            result=0;
            break;
        }
    }
    return result;
}

void delay(int delay) {
	int n = 1;
	while (n < delay) {
		n++;
	}
}
