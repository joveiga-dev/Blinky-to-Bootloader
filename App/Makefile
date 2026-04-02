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
FLOAT=-mfloat-abi=soft

# Constants for flashing
BOOT_SIZE = 0x08000000

#--------------------------------------------------------------------
# Target
#-------------------------------------------------------------------
PROJECT = firmware
TARGET = $(PROJECT).elf
BIN = $(PROJECT).bin
HEX = $(PROJECT).hex
MAP = $(PROJECT).map
ROOT_DIR = ..
BUILD_DIR = Build
STM32PROGRAMMER_CLI = "/mnt/c/Program Files (x86)/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin/STM32_Programmer_CLI.exe"

LINKER = -T $(ROOT_DIR)/Linkers/STM32L476RG_App.ld
MAP_FILE = -Wl,-Map=$(BUILD_DIR)/$(MAP)

#-------------------------------------------------------------------
# Flags
#------------------------------------------------------------------
CFLAGS = $(CPU)
CFLAGS += -mthumb $(FLOAT)
CFLAGS += -Wall -Wextra -Werror
CFLAGS += -O0 -g 
CFLAGS += -ffreestanding -nostdlib -nostartfiles
CFLAGS += -fno-common -ffunction-sections -fdata-sections

LDFLAGS =  $(CPU) -mthumb $(FLOAT) --specs=nosys.specs  $(LINKER) $(MAP_FILE) -g \
			-Wl,--gc-sections -static --specs=nano.specs -Wl,--start-group -lc -lm -Wl,--end-group \
 			-Wl,--print-memory-usage

#-----------------------------------------------
# Inludes
#--------------------------------------------
INCLUDES = -I$(ROOT_DIR)/App/Inc
INCLUDES += -I$(ROOT_DIR)/Drivers/Inc
INCLUDES += -I$(ROOT_DIR)/Common/Inc

#---------------------------------------
# Source Files discovery recursively
#-----------------------------------
APP_SRC_C    := $(wildcard $(ROOT_DIR)/App/Src/*.c)
APP_SRC_S    := $(wildcard $(ROOT_DIR)/App/Src/*.S)
DRIVERS_SRC  := $(wildcard $(ROOT_DIR)/Drivers/Src/*.c)
COMMON_SRC   := $(wildcard $(ROOT_DIR)/Common/Src/*.c)
STARTUP_SRC  := $(wildcard $(ROOT_DIR)/Linkers/*.c)
SOURCES      := $(APP_SRC_C) $(APP_SRC_S) $(STARTUP_SRC) $(DRIVERS_SRC) $(COMMON_SRC)

#---------------------------------------------------------------------------------
# Object files (mirrored into Build)
#---------------------------------------------------------------------------------
#OBJECTS      := $(patsubst $(ROOT_DIR)/%, $(BUILD_DIR)/%, $(SOURCES:.c=.o))
#OBJECTS      += $(patsubst $(ROOT_DIR)/%, $(BUILD_DIR)/%, $(SOURCES:.S=.o))

OBJECTS := $(patsubst $(ROOT_DIR)/%.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SOURCES)))
OBJECTS += $(patsubst $(ROOT_DIR)/%.S,$(BUILD_DIR)/%.o,$(filter %.S,$(SOURCES)))

#---------------------------------------------------------------------------------
# Default target
#---------------------------------------------------------------------------------
all: $(BUILD_DIR)/$(TARGET) $(BUILD_DIR)/$(BIN) size


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

#----------------------------------------------------------------------------
# Compile rules (S)
#---------------------------------------------------------------------------
$(BUILD_DIR)/%.o: $(ROOT_DIR)/%.S | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	@echo "Assembling $<"
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

size: $(BUILD_DIR)/$(TARGET)
	@echo ""
	@echo "=== Size Information ==="
	@$(SIZE) $<
	@echo ""
	@echo "=== Memory Usage ==="
	@SIZE=$$($(SIZE) -B $< | tail -1 | awk '{print $$1+$$2}'); \
	echo "Total (text+data): $$SIZE bytes"; \
	echo "Available SRAM: 65536 bytes (64KB)"; \
	PERCENT=$$(echo "scale=1; $$SIZE * 100 / 65536" | bc); \
	echo "Usage: $$PERCENT%"

# ---------------------------------------------------------------------
# Flashing targets
# ---------------------------------------------------------------------
flash:
	$(STM32PROGRAMMER_CLI) -c port=SWD mode=UR -w $(BUILD_DIR)/$(PROJECT).bin 0x08000000 -v -rst

dfu:
	dfu-util -a 0 -s 0x08000000:leave -D $(BUILD_DIR)/$(PROJECT).bin

# ---------------------------------------------------------------------
# Clean
# ---------------------------------------------------------------------
clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)/*.o $(BUILD_DIR)/*.elf $(BUILD_DIR)/*.bin $(BUILD_DIR)/*.map
	@echo "Deleting all"
	@rm -rf $(BUILD_DIR)

.PHONY: all clean size flash dfu 