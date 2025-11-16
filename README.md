
# m3 Bare-Metal Project (LM3S6965 / Cortex‑M3)

A clean, modular, bare‑metal firmware framework for the **TI Stellaris LM3S6965** microcontroller. This project uses a simple driver structure, a reusable startup system, and tiny test programs for each peripheral — all written from scratch without vendor libraries.

Perfect for learning:

* ARM Cortex‑M3 low‑level programming
* Interrupts & NVIC
* UART, SysTick, NVIC, SysCtl basics
* Boot process: vector table + startup
* Modular driver layout

---

## 🔧 Project Layout

```
.
├── drivers/           # Peripheral drivers
│   ├── nvic/
│   ├── sysctl/
│   ├── systick/
│   └── uart/
├── include/           # Shared public headers
├── platform/          # Startup, vector table, linker script
├── test/              # Test apps for each module
├── Makefile           # Flexible module-based build
└── README.md
```

---

## 🚀 Build & Run

The Makefile supports module-level builds.

### Build & run any test module

```
make run <module>
```

Examples:

```
make run uart
make run systick
make run irq
```

### Build only

```
make build <module>
```

### Debug mode (QEMU + GDB)

```
make rundbg <module>
```

This starts QEMU halted and waits for GDB on port **5678**.

### Clean

```
make clean
```

(Removes obj/bin directories)

---

## 📦 Modules

Each module lives under:

```
drivers/<module>/
include/<module>.h
```

Tests live under `test/`.

### **UART Driver**

Files:

* `drivers/uart/uart_drv.c`
* `drivers/uart/serial_print.c`

Features:

* UART0 configuration
* Baud rate setup
* Basic transmit functions (`serial_putc`, `serial_puts`, `serial_put_uint`)

Test:

```
test/test_uart.c
```

---

### **SysTick Driver**

Files:

* `drivers/systick/systick.c`
* `include/systick.h`

Features:

* SysTick init
* Millisecond tick conversion
* Optional SysTick interrupt demo

Test:

```
test/test_systick.c
```

---

### **NVIC Driver**

Files:

* `drivers/nvic/nvic.c`

Features:

* Enable/disable interrupts
* Clear pending
* Set priority

Test:

```
test/test_irq.c
```

---

### **SysCtl Driver**

Files:

* `drivers/sysctl/sysctl.c`
* `include/sysctl.h`

Provides clock access for UART/SysTick.

---

## 🧠 Platform Code

The **platform** directory contains all boot-critical files.

### `startup_lm3s6965.c`

* Vector table (all Cortex‑M3 + LM3S6965 interrupts)
* Weak handler aliases
* Clean Reset_Handler
* Data/BSS initialization
* Calls main()

### `lm3s6965_layout.ld`

Linker script defining:

* Flash memory layout
* SRAM layout
* `.vectors`, `.startup`, `.text`, `.data`, `.bss`

### `lm3s6965_memmap.h`

Register definitions & memory map constants.

---

## 🧪 Test Programs

Each test is a standalone `main()` that builds only the required drivers.

Examples:

* `test_uart.c` → UART printing
* `test_systick.c` → SysTick handler + timer demo
* `test_irq.c` → NVIC enable/disable

The Makefile automatically pulls dependency drivers.

---

## 🛠 Makefile Dependency Logic

Modules may depend on other modules.

Examples:

* **systick** needs: systick + uart + nvic + sysctl
* **irq** needs: nvic + uart

The Makefile auto-resolves dependencies and builds everything into:

```
bin/<module>/app.bin
```

Then runs in QEMU:

```
qemu-system-arm -M lm3s6965evb -kernel app.bin
```

---

## 🧰 Toolchain

This project uses:

* `arm-none-eabi-gcc`
* `arm-none-eabi-ld`
* `arm-none-eabi-objcopy`
* QEMU `lm3s6965evb` machine model

---

## ✨ Goals of the Project

* Pure bare-metal (no vendor HAL)
* Clean reusable driver architecture
* Clear startup process understanding
* Minimal & readable code
* Easy testing via QEMU

---

## 📄 License

MIT License

---

## 🤝 Contributing

Open to improvements:

* New drivers (GPIO, Timer, SSI)
* Cleanup/refactors
* Documentation

Submit PRs or issues on GitHub.

---

Happy hacking on your M3 board! 🚀
