#ifndef _PORTS_H
#define _PORTS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static inline void outb(uint16_t port, uint8_t val);
static inline uint8_t inb(uint16_t port);

static inline void outb(uint16_t port, uint8_t val){
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}


#endif
