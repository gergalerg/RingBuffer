GCC=gcc
CFLAGS=-g -Wall --std=gnu11
TARGET=main
SOURCES:=main.c ringbuffer.c
DEPS:=ringbuffer.h
# Math libraries
# MATH:=-lm
OBJECTS:=$(SOURCES:.c=.o)

%.o: %.c
	@echo "Making objects! Input = " $< "Output = " $@
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all
all: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) 

.PHONY: clean
clean:
	rm $(TARGET) $(OBJECTS)
