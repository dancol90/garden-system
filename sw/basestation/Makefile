TARGET    = basestation
LIBRARIES = SPI Wire EEPROM RF24 LightLCD LightMPR121 MenuLib

F_CPU     = 80000000

UPLOAD_SPEED = 921600
UPLOAD_PORT  = /dev/cu.SLAB_USBtoUART

ARDUINO_PACKAGE_DIR = /Users/daniele/Library/Arduino15/packages/esp8266
ARDUINO_CORE_DIR    = $(ARDUINO_PACKAGE_DIR)/hardware/esp8266/1.6.4-758-ga194024

#############################################
# Core paths and advanced configuration

PROJECT_DIR         = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
OUTPUT_DIR          = $(PROJECT_DIR)/build

SDK_PATH            = $(ARDUINO_CORE_DIR)/tools/sdk
COMPILER_PATH       = $(ARDUINO_PACKAGE_DIR)/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9/bin
ESPTOOL_PATH		= $(ARDUINO_PACKAGE_DIR)/tools/esptool/0.4.5

ARDUINO_IDE_VERSION = 10604
ARDUINO_BOARD       = ESP8266_ESP01

FLASH_SIZE          = 512K
CORE_FILE           = esp8266-core-generic-$(FLASH_SIZE).a
LD_SCRIPT           = eagle.flash.512k.ld

#############################################
# Arduino libraries path

LIBRARIES_CORE_DIR  = $(ARDUINO_CORE_DIR)/libraries
LIBRARIES_IDE_DIR   = /Applications/Arduino.app/Contents/Java/libraries
LIBRARIES_USER_DIR  = /Users/daniele/Progetti/Arduino/libraries

find_libraries = $(filter $(LIBRARIES), $(patsubst $(strip $(1)/%/), %, $(wildcard $(1)/*/)))

CORE_LIBRARIES = $(call find_libraries, $(LIBRARIES_CORE_DIR))
IDE_LIBRARIES  = $(call find_libraries, $(LIBRARIES_IDE_DIR))
USER_LIBRARIES = $(call find_libraries, $(LIBRARIES_USER_DIR))

#$(info core lib = $(CORE_LIBRARIES))
#$(info ide  lib = $(IDE_LIBRARIES))
#$(info user lib = $(USER_LIBRARIES))

LIBRARIES_FOUND     = $(CORE_LIBRARIES) $(IDE_LIBRARIES) $(USER_LIBRARIES)
LIBRARIES_NOT_FOUND = $(filter-out $(LIBRARIES_FOUND), $(LIBRARIES))

ifneq ($(strip $(LIBRARIES_NOT_FOUND)), )
   $(error Unable to find the following libraries: $(LIBRARIES_NOT_FOUND))
endif

#############################################
# Sources files

find_sources = $(patsubst $(strip $(1)/%), %, $(shell find $(1) -type f \( \
					-name '*.c' -o \
					-name '*.cpp' -o \
					-name '*.cc' -o \
					-name '*.S' \
				\) ! -path '*/examples/*' ))

find_libs_sources = $(foreach lib, $(1), $(addprefix $(lib)/, $(call find_sources, $(2)/$(lib))))

TARGET_SOURCES = $(call find_sources, $(PROJECT_DIR))

CORE_LIB_SOURCES    = $(call find_libs_sources, $(CORE_LIBRARIES), $(LIBRARIES_CORE_DIR))
IDE_LIB_SOURCES     = $(call find_libs_sources, $(IDE_LIBRARIES),  $(LIBRARIES_IDE_DIR))
USER_LIB_SOURCES    = $(call find_libs_sources, $(USER_LIBRARIES), $(LIBRARIES_USER_DIR))


DIRECTORIES = $(dir $(TARGET_SOURCES))

#$(info taget src    = $(TARGET_SOURCES))
#$(info core lib src = $(CORE_LIB_SOURCES))
#$(info ide  lib src = $(IDE_LIB_SOURCES))
#$(info user lib src = $(USER_LIB_SOURCES))

#############################################

OBJECTS  = $(CORE_LIB_SOURCES:%=libs/core/%.o)
OBJECTS += $(IDE_LIB_SOURCES:%=libs/ide/%.o)
OBJECTS += $(USER_LIB_SOURCES:%=libs/user/%.o)
OBJECTS += $(TARGET_SOURCES:%=%.o)

#$(info objects = $(OBJECTS))

#DFILES = $(addprefix $(OUTPUT_DIR)/,$(OBJECTS:.o=.d))
#-include $(DFILES)

#############################################
# Compiler commands

CC      = xtensa-lx106-elf-gcc
CPP     = xtensa-lx106-elf-g++
AS      = xtensa-lx106-elf-as
AR      = xtensa-lx106-elf-ar
ESPTOOL = esptool

#############################################
# Flags and parameters

DEFINES  = -DF_CPU=$(F_CPU) \
		   -DARDUINO=$(ARDUINO_IDE_VERSION) \
		   -DARDUINO_$(ARDUINO_BOARD) \
		   -DARDUINO_ARCH_ESP8266 \
		   -DESP8266 \
		   -D__ets__ \
		   -DICACHE_FLASH \
		   -U__STRICT_ANSI__ \
		   "-I$(SDK_PATH)/include"

CFLAGS   = -c -Os -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -falign-functions=4 -MMD -std=c99
SFLAGS   = -c -g -x assembler-with-cpp -MMD
CPPFLAGS = -c -Os -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -MMD
LNFLAGS  = -nostdlib -Wl,--no-check-sections -u call_user_start -Wl,-static "-L$(SDK_PATH)/lib" "-L$(SDK_PATH)/ld" "-T$(LD_SCRIPT)"

LIBS     = -lm -lgcc -lhal -lphy -lnet80211 -llwip -lwpa -lmain -lpp -lsmartconfig

INCLUDES = -I$(SDK_PATH)/include \
		   -I$(ARDUINO_CORE_DIR)/cores/esp8266 \
		   -I$(ARDUINO_CORE_DIR)/variants/generic

get_lib_inc_path = $(if $(and $(wildcard $(1)/src/), $(wildcard $(1)/library.properties)), \
						-I$(1)/src, \
						-I$(1) \
					)
get_libs_include = $(foreach lib, $(1), $(call get_lib_inc_path, $(2)/$(lib)))

INCLUDES += $(call get_libs_include, $(CORE_LIBRARIES), $(LIBRARIES_CORE_DIR)) \
			$(call get_libs_include, $(IDE_LIBRARIES),  $(LIBRARIES_IDE_DIR)) \
			$(call get_libs_include, $(USER_LIBRARIES), $(LIBRARIES_USER_DIR))

#############################################
# Actual compile command

COMPILE_C   = echo "C   $(2:.o=)" && "$(COMPILER_PATH)/$(CC)" $(DEFINES) $(CFLAGS) $(INCLUDES) "$1" -o "$(OUTPUT_DIR)/$2"
COMPILE_CPP = echo "CPP $(2:.o=)" && "$(COMPILER_PATH)/$(CPP)" $(DEFINES) $(CPPFLAGS) $(INCLUDES) "$1" -o "$(OUTPUT_DIR)/$2"
COMPILE_S   = echo "ASM $(2:.o=)" && "$(COMPILER_PATH)/$(CC)" $(DEFINES) $(SFLAGS) $(INCLUDES) "$1" -o "$(OUTPUT_DIR)/$2"

MKDIR = mkdir -p "$(OUTPUT_DIR)/$(dir $1)"

#############################################
# Recipes

.PHONY: all upload

all: $(OUTPUT_DIR)/$(TARGET).bin
	@echo "--- $(TARGET): done ---"

upload: $(OUTPUT_DIR)/$(TARGET).bin
	@echo "Uploading firmware"
	@"$(ESPTOOL_PATH)/$(ESPTOOL)" -vv -cd ck -cb $(UPLOAD_SPEED) -cp $(UPLOAD_PORT) -ca 0x00000 -cf "$(OUTPUT_DIR)/$(TARGET).bin"
	@echo "--- Done uploading $(TARGET) ---"

libs/core/%.c.o: $(LIBRARIES_CORE_DIR)/%.c
	@$(call MKDIR, $@)
	@$(call COMPILE_C,$<,$@)
libs/core/%.cpp.o: $(LIBRARIES_CORE_DIR)/%.cpp
	@$(call MKDIR, $@)
	@$(call COMPILE_CPP,$<,$@)
libs/core/%.S.o:  $(LIBRARIES_CORE_DIR)/%.s
	@$(call MKDIR, $@)
	@$(call COMPILE_S,$<,$@)

libs/ide/%.c.o: $(LIBRARIES_IDE_DIR)/%.c
	@$(call MKDIR, $@)
	@$(call COMPILE_C,$<,$@)
libs/ide/%.cpp.o: $(LIBRARIES_IDE_DIR)/%.cpp
	@$(call MKDIR, $@)
	@$(call COMPILE_CPP,$<,$@)
libs/ide/%.S.o:  $(LIBRARIES_IDE_DIR)/%.s
	@$(call MKDIR, $@)
	@$(call COMPILE_S,$<,$@)

libs/user/%.c.o: $(LIBRARIES_USER_DIR)/%.c
	@$(call MKDIR, $@)
	@$(call COMPILE_C,$<,$@)
libs/user/%.cpp.o: $(LIBRARIES_USER_DIR)/%.cpp
	@$(call MKDIR, $@)
	@$(call COMPILE_CPP,$<,$@)
libs/user/%.S.o:  $(LIBRARIES_USER_DIR)/%.s
	@$(call MKDIR, $@)
	@$(call COMPILE_S,$<,$@)

%.c.o: %.c
	@$(call MKDIR, $@)
	@$(call COMPILE_C,$<,$@)
%.cpp.o: %.cpp
	@$(call MKDIR, $@)
	@$(call COMPILE_CPP,$<,$@)
%.S.o: %.S
	@$(call MKDIR, $@)
	@$(call COMPILE_S,$<,$@)


$(OUTPUT_DIR)/$(TARGET).elf: $(OBJECTS)
	@echo "Creating elf file"
	@"$(COMPILER_PATH)/$(CC)" $(LNFLAGS) "-L$(OUTPUT_DIR)" -o "$(OUTPUT_DIR)/$(TARGET).elf" -Wl,--start-group $(addprefix $(OUTPUT_DIR)/, $(OBJECTS)) "$(OUTPUT_DIR)/$(CORE_FILE)" $(LIBS) -Wl,--end-group

$(OUTPUT_DIR)/$(TARGET).bin: $(OUTPUT_DIR)/$(TARGET).elf
	@echo "Creating hex file"
	@"$(ESPTOOL_PATH)/$(ESPTOOL)" -eo "$(ARDUINO_CORE_DIR)/bootloaders/eboot/eboot.elf" -bo "$(OUTPUT_DIR)/$(TARGET).bin" -bm qio -bf 40 -bz $(FLASH_SIZE) -bs .text -bp 4096 -ec -eo "$<" -bs .irom0.text -bs .text -bs .data -bs .rodata -bc -ec
	@echo "--- Done compiling $(TARGET) ---"
