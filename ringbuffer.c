#undef NDEBUG
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ringbuffer.h"
#include "dbg.h"

RingBuffer *RingBuffer_create(int length)
{
    RingBuffer *buffer = calloc(1, sizeof(RingBuffer));
    buffer->length = length + 1;
    buffer->start = 0;
    buffer->end = 0;
    buffer->buffer = calloc(buffer->length, 1);

    return buffer;
}

void RingBuffer_destroy(RingBuffer* buffer) 
{
    if (buffer) {
        free(buffer->buffer);
        free(buffer);
    }
}

int RingBuffer_write(RingBuffer *buffer, char *data, int length)
{
    if (RingBuffer_available_data(buffer) == 0) {
        buffer->start = buffer->end = 0;
    }

    check(length <= RingBuffer_available_space(buffer),
        "Not enough space: %d request, %d available",
        RingBuffer_available_data(buffer), length);

    void *result = memcpy(RingBuffer_ends_at(buffer), data, length);
    check(result != NULL, "Failed to write data into buffer.");

    RingBuffer_commit_write(buffer, length);

    return length;
error:
    return -1;
}

int RingBuffer_read(RingBuffer* buffer, char *target, int amount)
{
    check_debug(amount <= RingBuffer_available_data(buffer),
        "Not enough space in the buffer: has %d, needs %d",
        RingBuffer_available_data(buffer), amount);

    void *result = memcpy(target, RingBuffer_starts_at(buffer), amount);
    check(result != NULL, "Failed to write buffer into data");

    RingBuffer_commit_read(buffer, amount);

    if(buffer->end == buffer->start) {
        buffer->start = buffer->end = 0;
    }

    return amount;
error:
    return -1;
}

char* RingBuffer_gets(RingBuffer *buffer, int amount)
{
    check(amount > 0, "Need more time for gets, you gave: %d ", amount);
    check_debug(amount <= RingBuffer_available_data(buffer),
        "Not enough in the buffer");

    char* result = malloc(sizeof(char*)* (amount + 1));
    int res  = memcpy(result, buffer->buffer, amount);
    check(result != NULL, "Failed to create gets result.");
    check(strlen(result) == amount, "Wrong result length");

    RingBuffer_commit_read(buffer, amount);
    assert(RingBuffer_available_data(buffer) >= 0 && "Error in read commit ");

    return result;
error:
    return NULL;
}
