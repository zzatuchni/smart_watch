CFLAGS  ?=  -W -Wall -Wextra -Wundef -Wshadow -Wdouble-promotion \
            -Wformat-truncation -fno-common -Wconversion \
            -g3 -Os -ffunction-sections -fdata-sections -I. \
            -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 $(EXTRA_CFLAGS)
LDFLAGS ?= -Tlink.ld -nostartfiles -nostdlib --specs nano.specs -specs nosys.specs -lc -lgcc -lnosys -Wl,--gc-sections -Wl,-Map=$@.map
SOURCES = $(wildcard *.c) $(wildcard */*.c) $(wildcard */*/*.c)

ifeq ($(OS),Windows_NT)
  RM = cmd /C del /Q /F
else
  RM = rm -f
endif

build: ./out/main.bin

./out/main.elf: $(SOURCES)
	arm-none-eabi-gcc $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

./out/main.bin: ./out/main.elf
	arm-none-eabi-objcopy -O binary $< $@

flash: ./out/main.bin
	st-flash --reset write $< 0x8000000

clean:
	$(RM) ./out/main.*
