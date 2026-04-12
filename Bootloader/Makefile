#--------------------------------------------------------------------------------
# Toolchain
#---------------------------------------------------------------------------------
TC = arm-none-eabi-
CC = $(TC)gcc
LD = $(TC)ld
OBJCOPY = $(TC)objcopy
OBJDUMP = $(TC)objdump
SIZE = $(TC)size

# CPU and architecture flags
CPU = -mcpu=cortex-m4
FLOAT = -mfloat-abi=hard -mfpu=fpv4-sp-d16 -u _printf_float

BOOT_START = 0x08000000

#--------------------------------------------------------------------
# Target
#-------------------------------------------------------------------
PROJECT = bootloader
TARGET = $(PROJECT).elf
BIN = $(PROJECT).bin
HEX = $(PROJECT).hex
MAP = $(PROJECT).map
ROOT_DIR = ..
BUILD_DIR = Build/
STM32PROGRAMMER_CLI = "/mnt/c/Program Files (x86)/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI.exe"

LINKER = -T $(ROOT_DIR)/Linkers/STM32L476RG_Boot.ld
MAP_FILE = -Wl,-Map=$(BUILD_DIR)/$(MAP)

#-------------------------------------------------------------------
# Flags
#------------------------------------------------------------------
CFLAGS = $(CPU)
CFLAGS += -mthumb $(FLOAT)
CFLAGS += -Wall -Wextra -Werror
CFLAGS += -Os
CFLAGS += -ffreestanding -nostdlib -nostartfiles
CFLAGS += -fno-common -ffunction-sections -fdata-sections

LDFLAGS =  $(CPU) -mthumb $(FLOAT) --specs=nosys.specs  $(LINKER) $(MAP_FILE) -g \
			-Wl,--gc-sections -static --specs=nano.specs -Wl,--start-group -lc -lm -Wl,--end-group \
 			-Wl,--print-memory-usage

#-----------------------------------------------
# Inludes
#--------------------------------------------
INCLUDES = -I$(ROOT_DIR)/Bootloader/Inc
INCLUDES += -I$(ROOT_DIR)/Drivers/Inc
INCLUDES += -I$(ROOT_DIR)/Common/Inc

#---------------------------------------
# Source Files discovery recursively
#-----------------------------------
BOOT_SRC := $(wildcard $(ROOT_DIR)/Bootloader/Src/*.c)
DRIVERS_SRC := $(wildcard $(ROOT_DIR)/Drivers/Src/*.c)
COMMON_SRC := $(wildcard $(ROOT_DIR)/Common/Src/*.c)
STARTUP_SRC := $(wildcard $(ROOT_DIR)/Startup/*.c)

SOURCES := $(BOOT_SRC) $(STARTUP_SRC) $(DRIVERS_SRC) $(COMMON_SRC)

#---------------------------------------------------------------------------------
# Object files (mirrored into Build)
#---------------------------------------------------------------------------------
OBJECTS := $(patsubst $(ROOT_DIR)/%, $(BUILD_DIR)/%, $(SOURCES:.c=.o))

#---------------------------------------------------------------------------------
# Default target
#---------------------------------------------------------------------------------
all: $(BUILD_DIR)/$(TARGET) $(BUILD_DIR)/$(BIN)


# Ensure build directories exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# ---------------------------------------------------------------------
# Compile rules (C)
# ---------------------------------------------------------------------
$(BUILD_DIR)/%.o: $(ROOT_DIR)/%.c | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ---------------------------------------------------------------------
# Linking
# ---------------------------------------------------------------------
$(BUILD_DIR)/$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	@echo "Linking $@"
	$(CC) $(LDFLAGS) -o $@ $^

#----------------------------------------------------------------------
# BIN
#----------------------------------------------------------------------
$(BUILD_DIR)/$(BIN): $(BUILD_DIR)/$(TARGET)
	@echo "Generating BIN"
	$(OBJCOPY) -O binary $< $@
#	python3 pad_bootloader.py

# ---------------------------------------------------------------------
# Flashing targets
# ---------------------------------------------------------------------
flash:
	$(STM32PROGRAMMER_CLI) -c port=SWD mode=UR -w $(BUILD_DIR)/$(TARGET) $(BOOT_START) -v -rst

dfu:
	dfu-util -a 0 -s $(BOOT_START):leave -D $(BUILD_DIR)/$(BIN) $(BOOT_START)

# ---------------------------------------------------------------------
# Clean
# ---------------------------------------------------------------------
clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)

.PHONY: all clean flash dfu 