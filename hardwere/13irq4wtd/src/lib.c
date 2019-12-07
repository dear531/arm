#include <common.h>
#include <lib.h>

void delay(int ms)
{
	volatile int i = 0, j = 0;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 10000; j++)
			;
}

void *memcpy(void *dest, const void *src, int count)
{
        char *tmp = dest;
        const char *s = src;

        while (count--)
                *tmp++ = *s++;
        return dest;
}
