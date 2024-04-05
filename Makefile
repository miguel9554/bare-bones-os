# Define directories
SRC_DIR := src
BUILD_DIR := build
GRUB_DIR := grub
ISO_DIR := $(BUILD_DIR)/isodir

OS_BIN := $(BUILD_DIR)/myos.bin
OS_ISO := $(BUILD_DIR)/myos.iso

GRUB_CFG := $(GRUB_DIR)/grub.cfg

# Get all .c files in src/
C_FILES := $(wildcard $(SRC_DIR)/*.c)

# Get all .s files in src/
S_FILES := $(wildcard $(SRC_DIR)/*.s)

# Set linker script
LINKER_SCRIPT := $(SRC_DIR)/linker.ld

# Generate corresponding object file names
C_OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_FILES))
S_OBJ_FILES := $(patsubst $(SRC_DIR)/%.s, $(BUILD_DIR)/%.o, $(S_FILES))

# Compiler config
CC := i686-elf-gcc
CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# Assembler config
AS := i686-elf-as

# Linker config
LD := i686-elf-gcc
LFLAGS := -ffreestanding -O2 -nostdlib

# Targets
all: $(OS_ISO)

# Rule to compile .c files into object files
$(C_OBJ_FILES): $(C_FILES)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to compile .s files into object files
$(S_OBJ_FILES): $(S_FILES)
	$(AS) $< -o $@

# Build the OS binary
$(OS_BIN): $(C_OBJ_FILES) $(S_OBJ_FILES) $(LINKER_SCRIPT)
	$(LD) -T $(LINKER_SCRIPT) -o $@ $(LFLAGS) $(C_OBJ_FILES) $(S_OBJ_FILES) -lgcc
	grub-file --is-x86-multiboot $@

# Build the OS ISO
$(OS_ISO): $(OS_BIN) $(GRUB_CFG)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(OS_BIN) $(ISO_DIR)/boot/myos.bin
	cp $(GRUB_CFG) $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(OS_ISO) $(ISO_DIR)

# Clean target
clean:
	rm -rf $(BUILD_DIR)/*

# Phony targets
.PHONY: all clean
