# Change this to the same name of your Keil Uvision project (without extension).
EXECUTABLE = makefile-playground

# Change this to the target name used by Keil to build for FRDM-KL25Z.
KEIL_TARGET_NAME = "Target 1"

# Change this according to your local configuration.
COMPILER_PATH = C:/Keil_v5/ARM/ARMCLANG/bin

# Change this according to your local configuration.
UPLOADER_TOOL = C:/Keil_v5/UV4/uvision.com

# Change this according to your local configuration.
DEBUGGER = C:/Keil_v5/UV4/UV4.exe

COMPILER_NAME = armclang.exe
LINKER_NAME = armlink.exe
CC = $(COMPILER_PATH)/$(COMPILER_NAME)
LINKER = $(COMPILER_PATH)/$(LINKER_NAME)
BUILD_DIR = ./Objects
FORMATTER = $(COMPILER_PATH)/clang-format.exe

# Add here your include paths.
LOCAL_INC_DIRS = ./RTE/Device/MKL25Z128xxx4

INC_DIRS = $(LOCAL_INC_DIRS)

# Change this according to your local configuration.
INC_DIRS += C:/Users/tonto/AppData/Local/Arm/Packs/ARM/CMSIS/5.8.0/CMSIS/Core/Include
INC_DIRS += C:/Users/tonto/AppData/Local/Arm/Packs/Keil/Kinetis_KLxx_DFP/1.15.0/Device/Include

CFLAGS = \
	-xc -std=c99 --target=arm-arm-none-eabi -mcpu=cortex-m0plus -c \
	-fno-rtti -funsigned-char -fshort-enums -fshort-wchar \
	-D__EVAL -gdwarf-4 -O0 -ffunction-sections \
	-Weverything -Wno-packed -Wno-reserved-id-macro -Wno-unused-macros \
	-Wno-documentation-unknown-command -Wno-documentation \
	-Wno-license-management -Wno-parentheses-equality \
	-DMKL25Z128xxx4 -MD \
	$(foreach d, $(INC_DIRS), -I$(d))

ASMFLAGS = \
    --target=arm-arm-none-eabi -mcpu=cortex-m0plus -masm=auto -c \
    -gdwarf-4 -Wa,armasm,--pd,"__EVAL SETA 1"

LDFLAGS = \
	--cpu Cortex-M0+ \
    --ro-base 0x00000000 --entry 0x00000000 --rw-base 0x1FFFF000 \
    --entry Reset_Handler --first __Vectors --strict \
    --callgraph_file=mycallgraph.txt --callgraph_output=text \
    --summary_stderr --info summarysizes --map --load_addr_map_info \
    --xref --callgraph --symbols \
    --info sizes --info totals --info unused --info veneers \
    --list ".\Listings\makefile-playground.map"

# Add here your C source files.
CSRC = ./main.c
CSRC += ./RTE/Device/MKL25Z128xxx4/system_MKL25Z4.c

# Add here your .s source files.
ASMSRC = ./RTE/Device/MKL25Z128xxx4/startup_MKL25Z4.s

LOCAL_HEADERS = $(foreach dir, $(LOCAL_INC_DIRS), $(wildcard $(dir)/*.h))

build:
	@echo Compile C sources
	@$(foreach f, $(CSRC), $(CC) $(CFLAGS) $(f) -o $(BUILD_DIR)/$(basename $(notdir $(f))).o; )

	@echo Compile ASM sources
	@$(foreach f, $(ASMSRC), $(CC) $(ASMFLAGS) $(f) -o $(BUILD_DIR)/$(basename $(notdir $(f))).o;)

	@echo Link all together
	@$(LINKER) $(BUILD_DIR)/*.o $(LDFLAGS) -o $(BUILD_DIR)/$(EXECUTABLE).axf

flash: build
	$(UPLOADER_TOOL) -f ./$(EXECUTABLE).uvprojx -t $(KEIL_TARGET_NAME)

format:
# Format .c files
	@$(foreach f, $(CSRC), $(FORMATTER) -i $(f);)
# Format .h files
	@$(foreach f, $(LOCAL_HEADERS), $(FORMATTER) -i $(f);)

debug: flash
	$(DEBUGGER) -d ./$(EXECUTABLE).uvprojx -t $(KEIL_TARGET_NAME)

clean:
	rm -f $(BUILD_DIR)/*.o 
	rm -f $(BUILD_DIR)/*.axf
	rm -f $(BUILD_DIR)/*.d
