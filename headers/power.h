#ifndef _POWER_H
#define _POWER_H

#define KBRD_INTRFC           0x64

//keyboard interface bits
#define KBRD_BIT_KDATA        0 								    //keyboard data is in buffer (output buffer is empty) (bit 0)
#define KBRD_BIT_UDATA        1 								    //user data is in buffer (command buffer is empty) (bit 1)

#define KBRD_IO               0x60 							    //keyboard IO port
#define KBRD_RESET            0xFE 							    //reset CPU command

#define bit(n)                (1<<(n)) 							//Set bit n to 1

#define check_flag(flags, n) ((flags) & bit(n)) 		//Check if bit n in flags is set

void reboot()
{
    uint8_t temp;

    __asm__ __volatile__ ("cli");

    do {
        temp = inb(KBRD_INTRFC);						        //empty user data
        if (check_flag(temp, KBRD_BIT_KDATA) != 0)
            inb(KBRD_IO); 								          //empty keyboard data
    } while (check_flag(temp, KBRD_BIT_UDATA) != 0);

    outb(KBRD_INTRFC, KBRD_RESET);						      //pulse CPU reset line
loop:
    __asm__ __volatile__ ("hlt");
    goto loop;
}

#endif
