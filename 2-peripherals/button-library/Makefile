PROJECT_NAME=button-library

LIBDIR = ../common
BUILDDIR = build
DEVICE = $(LIBDIR)/STM32F4xx
CORE = $(LIBDIR)/CMSIS
PERIPH = $(LIBDIR)/STM32F4xx_StdPeriph_Driver

SOURCES += startup_stm32f4xx.S
SOURCES += system_stm32f4xx.c

SOURCES += src/main.c


OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(abspath $(basename $(SOURCES)))))

INCLUDES += -I$(DEVICE) \
			-I$(CORE) \
			-I$(PERIPH)/inc \
			-I.

ELF = $(BUILDDIR)/$(PROJECT_NAME).elf
HEX = $(BUILDDIR)/$(PROJECT_NAME).hex
BIN = $(BUILDDIR)/$(PROJECT_NAME).bin

CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
GDB = arm-none-eabi-gdb
SIZE = arm-none-eabi-size

CFLAGS  = -O0 -g -Wall -I.\
   -mcpu=cortex-m4 -mthumb \
   -mfpu=fpv4-sp-d16 -mfloat-abi=hard \
   $(INCLUDES) -DUSE_STDPERIPH_DRIVER

LDSCRIPT = stm32_flash.ld
LDFLAGS += -T$(LDSCRIPT) -mthumb -mcpu=cortex-m4 -nostdlib

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(ELF): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)
	$(SIZE) $@


$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.S
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

flash: $(BIN)
	st-flash write $(BIN) 0x8000000

debug: $(ELF)
	$(GDB) -tui $(ELF)

openocd: $(ELF)
	openocd -f board/stm32f4discovery.cfg

all: $(HEX) $(BIN) $(ELF)

functions: all

.PHONY: clean
clean:
	rm -rf build
