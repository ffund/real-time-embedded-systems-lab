PROJECT_NAME=iot
BUILDDIR = build

LIBDIR = ../common
DEVICE = $(LIBDIR)/STM32F4xx
CORE = $(LIBDIR)/CMSIS
PERIPH = $(LIBDIR)/STM32F4xx_StdPeriph_Driver
PROJHEADERS = inc

SOURCES = startup_stm32f4xx.S \
          system_stm32f4xx.c

SOURCES += src/main.c \
	   src/rgbled.c \
	   src/lightsensor.c \
	   src/bluetooth.c \
	   src/leds.c \
	   src/power.c \
	   src/buffer.c \
	   audio/src/Audio.c \
	   audio/src/mp3_data.c \
	   audio/src/audioutils.c \
	   helix/mp3dec.c

SOURCES += $(PERIPH)/src/stm32f4xx_pwr.c \
	   $(PERIPH)/src/stm32f4xx_gpio.c \
	   $(PERIPH)/src/stm32f4xx_rcc.c \
	   $(PERIPH)/src/stm32f4xx_exti.c \
	   $(PERIPH)/src/stm32f4xx_tim.c \
	   $(PERIPH)/src/stm32f4xx_adc.c \
	   $(PERIPH)/src/stm32f4xx_usart.c \
	   $(PERIPH)/src/misc.c \
	   $(PERIPH)/src/stm32f4xx_syscfg.c


OBJECTS = $(addprefix $(BUILDDIR)/, $(addsuffix .o, $(abspath $(basename $(SOURCES)))))

INCLUDES += -I$(DEVICE) \
			-I$(PROJHEADERS) \
			-I$(CORE) \
			-I$(PERIPH)/inc \
			-Iinc \
			-Ihelix/pub \
			-Iaudio/inc

ELF = $(BUILDDIR)/$(PROJECT_NAME).elf
HEX = $(BUILDDIR)/$(PROJECT_NAME).hex
BIN = $(BUILDDIR)/$(PROJECT_NAME).bin

CC = arm-none-eabi-gcc
LD = arm-none-eabi-gcc
AR = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
GDB = arm-none-eabi-gdb
SIZE = arm-none-eabi-size

LIBS    = -Lhelix -lhelix -lm 

CFLAGS  = -O0 -ggdb -Wall -I.\
   -fsingle-precision-constant -Wdouble-promotion \
   -mfpu=fpv4-sp-d16 -mfloat-abi=hard \
   -std=c99 -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork \
   $(INCLUDES) $(LIBS) -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000

SEMIHOSTING_FLAGS = --specs=rdimon.specs -lc -lrdimon

LDSCRIPT = stm32_flash.ld
LDFLAGS += -T$(LDSCRIPT) 

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

$(HEX): $(ELF)
	$(OBJCOPY) -O ihex $< $@

$(ELF): $(OBJECTS)
	$(MAKE) -C helix
	$(LD) $(LDFLAGS) $(CFLAGS) $(SEMIHOSTING_FLAGS) -o $@ $(OBJECTS) $(LDLIBS) $(LIBS)
	$(SIZE) $@

$(BUILDDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(SEMIHOSTING_FLAGS) $(CFLAGS) $< -o $@

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
	$(MAKE) clean -C helix
