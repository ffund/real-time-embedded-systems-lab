PROJECT_NAME=digital-dice

BUILDDIR = build

LIBDIR = ../common
DEVICE = $(LIBDIR)/STM32F4xx
CORE = $(LIBDIR)/CMSIS
PERIPH = $(LIBDIR)/STM32F4xx_StdPeriph_Driver
PROJHEADERS = inc

SOURCES = startup_stm32f4xx.S
SOURCES += system_stm32f4xx.c

SOURCES += src/main.c
SOURCES += $(PERIPH)/src/stm32f4xx_gpio.c
SOURCES += $(PERIPH)/src/stm32f4xx_rcc.c
SOURCES += $(PERIPH)/src/stm32f4xx_tim.c
SOURCES += $(PERIPH)/src/stm32f4xx_exti.c
SOURCES += $(PERIPH)/src/misc.c
SOURCES += $(PERIPH)/src/stm32f4xx_syscfg.c
SOURCES += $(PERIPH)/src/stm32f4xx_rng.c
SOURCES += $(PERIPH)/src/stm32f4xx_spi.c

OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(abspath $(basename $(SOURCES)))))

INCLUDES += -I$(DEVICE) \
			-I$(PROJHEADERS) \
			-I$(CORE) \
			-I$(PERIPH)/inc \
			-Isrc \
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
   -std=c99 -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork \
   -mfpu=fpv4-sp-d16 -mfloat-abi=hard \
   $(INCLUDES) -DUSE_STDPERIPH_DRIVER

SEMIHOSTING_FLAGS = --specs=rdimon.specs -lc -lrdimon

LDSCRIPT = stm32_flash.ld
LDFLAGS += -T$(LDSCRIPT) -mthumb -mcpu=cortex-m4 

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(ELF): $(OBJECTS)
	$(LD) $(LDFLAGS) $(CFLAGS) $(SEMIHOSTING_FLAGS) -o $@ $(OBJECTS) $(LDLIBS)
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
