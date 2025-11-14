CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS=-mcpu=cortex-m3 -mthumb -O2 -Wall -nostdlib -ffreestanding
LDFLAGS=-T linker.ld

SRCS=$(wildcard src/*.c drivers/*.c isr/*.c)
OBJS=$(SRCS:%.c=build/%.o)

all: build/firmware.bin

build/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/startup.o: startup.s
	mkdir -p build
	$(CC) $(CFLAGS) -c startup.s -o build/startup.o

build/firmware.elf: build/startup.o $(OBJS)
	$(CC) $(CFLAGS) build/startup.o $(OBJS) $(LDFLAGS) -o build/firmware.elf

build/firmware.bin: build/firmware.elf
	$(OBJCOPY) -O binary build/firmware.elf build/firmware.bin

clean:
	rm -rf build
