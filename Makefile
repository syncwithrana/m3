#------------------ Toolchain ------------------
CC := arm-none-eabi-gcc
LD := arm-none-eabi-ld
OBJCOPY := arm-none-eabi-objcopy

CFLAGS := -c -g -mcpu=cortex-m3 -mthumb -Iinclude -Iplatform
LDFLAGS := -T platform/lm3s6965_layout.ld

#------------------ Parse ACTION + MODULE ------------------
ACTION := $(firstword $(MAKECMDGOALS))
MODULE := $(word 2, $(MAKECMDGOALS))

#------------------ Module dependencies ------------------
DEPS_systick := uart nvic sysctl
DEPS_uart := nvic sysctl
DEPS_sysctl :=
DEPS_nvic :=

MODULES := $(MODULE) $(DEPS_$(MODULE))

#------------------ Paths ------------------
OBJDIR := obj/$(MODULE)
BINDIR := bin/$(MODULE)

#------------------ Source discovery ------------------
DRIVER_SRCS := $(foreach m,$(MODULES),$(wildcard drivers/$(m)/*.c))
PLATFORM_SRCS := $(wildcard platform/*.c)
TEST_SRC := $(wildcard test/test_$(MODULE).c)

SRCS := $(DRIVER_SRCS) $(PLATFORM_SRCS) $(TEST_SRC)

OBJ_NAMES := $(notdir $(SRCS:.c=.o))
OBJS := $(addprefix $(OBJDIR)/,$(OBJ_NAMES))

#------------------ Directory creation ------------------
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(BINDIR):
	mkdir -p $(BINDIR)

#------------------ Compile rule (.c → .o) ------------------
$(OBJDIR)/%.o: | $(OBJDIR)
	@src="$$(printf "%s\n" $(SRCS) | grep -E '/$*.c$$')"; \
	echo "CC $$src -> $@"; \
	$(CC) $(CFLAGS) -o $@ "$$src"

#------------------ Link + convert ------------------
$(BINDIR)/app.elf: $(OBJS) | $(BINDIR)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(BINDIR)/app.bin: $(BINDIR)/app.elf
	$(OBJCOPY) -O binary $< $@

#------------------ Actions ------------------
.PHONY: build run rundbg clean

build:
	@if [ -z "$(MODULE)" ]; then echo "Usage: make build <module>"; exit 1; fi
	@if [ -z "$(TEST_SRC)" ]; then echo "ERROR: test/test_$(MODULE).c missing (needs main())"; exit 1; fi
	$(MAKE) MODULE=$(MODULE) $(BINDIR)/app.bin

run:
	@if [ -z "$(MODULE)" ]; then echo "Usage: make run <module>"; exit 1; fi
	$(MAKE) MODULE=$(MODULE) build
	qemu-system-arm -M lm3s6965evb -kernel bin/$(MODULE)/app.bin -nographic

rundbg:
	@if [ -z "$(MODULE)" ]; then echo "Usage: make rundbg <module>"; exit 1; fi
	$(MAKE) MODULE=$(MODULE) build
	qemu-system-arm -S -M lm3s6965evb -kernel bin/$(MODULE)/app.bin -gdb tcp::5678 -nographic

clean:
	rm -rf obj bin *.elf *.bin
