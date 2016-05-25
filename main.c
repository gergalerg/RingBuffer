#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ringbuffer.h"
#include "dbg.h"

int main(int argc, char const *argv[])
{
    RingBuffer *rb = RingBuffer_create(100);
    RingBuffer_puts(rb, "Gerg");
    RingBuffer_puts(rb, "Delicious");
    char* res = RingBuffer_gets(rb, 10);
    printf("res = %s\n", res);
    return 0;
}