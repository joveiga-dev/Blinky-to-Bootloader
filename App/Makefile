

SRC_DIR := Src
BUILD_DIR := Build

TARGET := firmware

LINKER := -T linker.ld

# Tools
TC := arm-none-eabi
CC := $(TC)-gcc
OBJCPY := $(TC)-objcopy
GDB := $(TC)-gdb

# CPU / ARCH
CPU := -mcpu=cortex-m4
FLOAT := -mfloat-abi=soft
# FLAGS
CFLAGS := $(CPU) -mthumb $(FLOAT) -o0 -g -Wall
LDFLAGS := $(LINKER) -nostdlib

# Find C source files recursively
SOURCES := $(shell find $(SRC_DIR) -type f -name '*.c')

# Generate object file names based on source file names
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

.PHONY: all clean flash debug gdb

all: $(BUILD_DIR)/$(TARGET).bin

# Compile to ELF
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

# Convert ELF to BIN
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCPY) -O binary $< $@

# compile each source
$(BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# Flash target
flash: $(BUILD_DIR)/$(TARGET).bin
	@echo "Flashing firmware via ST-LINK GDB server..."
	"/mnt/c/Program Files (x86)/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI.exe" \
	-c port=SWD mode=UR -w $< 0x08000000 -rst
	
# Debug helper
debug: $(BUILD_DIR)/$(TARGET).elf
	@echo "Starting OpenOCD..."
	@openocd -f openocd.cfg &
	sleep 1
	$(GDB) $< -ex "target remote localhost:61234" -ex "monitor reset init"

clean:
	rm -rf $(BUILD_DIR)



