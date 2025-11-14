CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=cortex-m3 -mthumb -O2 -ffreestanding -nostdlib -Wall -Wextra
LDFLAGS = -T linker.ld

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst %.c,build/%.o,$(SRCS))

all: build/firmware.bin

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/startup.o: startup.s
	@mkdir -p build
	$(CC) $(CFLAGS) -c startup.s -o $@

build/firmware.elf: build/startup.o $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

build/firmware.bin: build/firmware.elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -rf build
