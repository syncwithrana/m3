
# Tooling/paths
CC := arm-none-eabi-gcc
LD := arm-none-eabi-ld
OBJCOPY := arm-none-eabi-objcopy
NM := arm-none-eabi-nm
OBJDUMP := arm-none-eabi-objdump
SIZE := arm-none-eabi-size

CFLAGS := -c -g -mcpu=cortex-m3 -mthumb
LDFLAGS := -T lm3s6965_layout.ld

# Module handling: call `make <module>` or `make MODULE=<module>` to build
# a module whose layout is:
#   <module>/src/*.c
#   <module>/inc/*.h
# Default module when MODULE is not provided is `uart`.
MODULE ?= tick

SRCDIR := $(MODULE)/src
INCDIR := $(MODULE)/inc
INC := -I inc -I irq/inc -I uart/inc -I tick/inc -I common/uart/inc
OBJDIR := obj/$(MODULE)
BINDIR := bin/$(MODULE)

SRCS := $(wildcard $(SRCDIR)/*.c) common/uart/src/uart_drv.c
OBJS := $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(wildcard $(SRCDIR)/*.c)) obj/common_uart_drv.o

all: $(BINDIR)/system.bin

# Allow `make uart` to run the module build: it delegates to `make MODULE=uart build`
.PHONY: %
%:
	$(MAKE) MODULE=$@ build

build: $(BINDIR)/system.bin

$(OBJDIR):
	mkdir -p $(OBJDIR)



$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) $(INC) -o $@ $<

# Compile common/uart/src/uart_drv.c to obj/common_uart_drv.o
obj/common_uart_drv.o: common/uart/src/uart_drv.c | obj
	$(CC) $(CFLAGS) $(INC) -o $@ $<


$(BINDIR):
	mkdir -p $(BINDIR)

$(BINDIR)/startup_lm3s6965.elf: $(OBJS) | $(BINDIR)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(BINDIR)/system.bin: $(BINDIR)/startup_lm3s6965.elf
	$(OBJCOPY) -O binary $< $@


run: $(BINDIR)/system.bin
	qemu-system-arm -M lm3s6965evb -kernel $(BINDIR)/system.bin -nographic -monitor telnet:127.0.0.1:1234,server,nowait

rundbg: $(BINDIR)/system.bin
	qemu-system-arm -S -M lm3s6965evb -kernel $(BINDIR)/system.bin -gdb tcp::5678 -nographic -monitor telnet:127.0.0.1:1234,server,nowait


clean:
	rm -rf $(OBJDIR) $(BINDIR) *.elf *.bin

cleanall:
	rm -rf obj bin *.elf *.bin

dump:
	@for o in $(OBJS); do \
		$(NM) -n $$o || true; \
		$(OBJDUMP) -h $$o || true; \
	done
	$(NM) -n $(BINDIR)/startup_lm3s6965.elf || true
	$(OBJDUMP) -h $(BINDIR)/startup_lm3s6965.elf || true
	$(SIZE) $(BINDIR)/startup_lm3s6965.elf
