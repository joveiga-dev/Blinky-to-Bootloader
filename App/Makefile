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
CPU   = -mcpu=cortex-m4
FLOAT = -mfloat-abi=hard -mfpu=fpv4-sp-d16

#--------------------------------------------------------------------------------
# APP MEMORY CONFIG
#--------------------------------------------------------------------------------
APP_START = 0x08008000
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

#--------------------------------------------------------------------------------
# DEFINES
#--------------------------------------------------------------------------------
DEFINES = -DAPP_START=$(APP_START)

#-------------------------------------------------------------------
# Flags
#------------------------------------------------------------------
CFLAGS  = $(CPU)
CFLAGS += -mthumb $(FLOAT)
CFLAGS += -Wall -Wextra -Werror
CFLAGS += -O0 -g
CFLAGS += -ffreestanding -nostdlib -nostartfiles
CFLAGS += -fno-common -ffunction-sections -fdata-sections
#CFLAGS += $(DEFINES)

LDFLAGS = $(CPU) -mthumb $(FLOAT)
LDFLAGS += --specs=nosys.specs
LDFLAGS += $(LINKER)
LDFLAGS += $(MAP_FILE)
LDFLAGS += -g
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -static
LDFLAGS += --specs=nano.specs
LDFLAGS += -Wl,--start-group -lc -lm -Wl,--end-group
LDFLAGS += -Wl,--print-memory-usage

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
#APP_SRC_S    := $(wildcard $(ROOT_DIR)/App/Src/*.S)
DRIVERS_SRC  := $(wildcard $(ROOT_DIR)/Drivers/Src/*.c)
COMMON_SRC   := $(wildcard $(ROOT_DIR)/Common/Src/*.c)
STARTUP_SRC  := $(wildcard $(ROOT_DIR)/Startup/*.c)
SOURCES      := $(APP_SRC_C) $(STARTUP_SRC) $(DRIVERS_SRC) $(COMMON_SRC)

#---------------------------------------------------------------------------------
# Object files (mirrored into Build)
#---------------------------------------------------------------------------------

OBJECTS := $(patsubst $(ROOT_DIR)/%.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SOURCES)))
OBJECTS += $(patsubst $(ROOT_DIR)/%.S,$(BUILD_DIR)/%.o,$(filter %.S,$(SOURCES)))

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
	@echo 'Finished compiling $<'
	@echo '-----------------------------------'

#----------------------------------------------------------------------------
# Compile rules (S)
#---------------------------------------------------------------------------
#$(BUILD_DIR)/%.o: $(ROOT_DIR)/%.S | $(BUILD_DIR)
#	@mkdir -p $(dir $@)
#	@echo "Assembling $<"
#	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# ---------------------------------------------------------------------
# Linking
# ---------------------------------------------------------------------
$(BUILD_DIR)/$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	@echo "Linking $@"
	$(CC) $(LDFLAGS) -o $@ $^
	@echo 'Finished linking $@'
	@echo '-----------------------------------'

#----------------------------------------------------------------------
# BIN
#----------------------------------------------------------------------
$(BUILD_DIR)/$(BIN): $(BUILD_DIR)/$(TARGET)
	@echo "Generating BIN"
	$(OBJCOPY) -O binary $< $@


# ---------------------------------------------------------------------
# Flashing targets
# ---------------------------------------------------------------------
flash: $(BUILD_DIR)/$(BIN)
	$(STM32PROGRAMMER_CLI) -c port=SWD mode=UR -w $(BUILD_DIR)/$(TARGET) $(APP_START) -v -rst

dfu:
	dfu-util -a 0 -s $(APP_START):leave -D $(BUILD_DIR)/$(BIN)

# ---------------------------------------------------------------------
# Clean
# ---------------------------------------------------------------------
clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)

.PHONY: all clean flash dfu 