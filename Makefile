# PROJECT: name of the output file
PROJECT = main
#DEV : your TM4C123GH6PM when connected to your system,usually will be /dev/ttyACM0
DEV = /dev/cu.usbmodem0E233CBB1
MCU = TM4C123GH6PM
# SRCS: all source files from src directory
SRCS = $(wildcard src/*.c) \
	  $(wildcard libs/*.c)
OBJ = obj/
# OBJS: list of object files
OBJS = $(addprefix $(OBJ),$(notdir $(SRCS:.c=.o)))

#Flag points to the INC folder containing header files
INC = -Iinc

# LD_SCRIPT: linker script
LD_SCRIPT = ld/$(MCU).ld

#UTILITY VARIABLES
CC = arm-none-eabi-gcc #compiler
LD = arm-none-eabi-ld #linker
OBJCOPY = arm-none-eabi-objcopy #final executable builder
FLASHER = lm4flash #flashing utility
RM      = rm -rf
MKDIR   = @mkdir -p $(@D) #creates folders if not present

# GCC FLAGS
CFLAGS = -ggdb -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -lgcc
CFLAGS += -mfloat-abi=softfp -Os -MD -std=c99 -c    

# LINKER FLAGS
LDFLAGS = -T $(LD_SCRIPT) -e Reset_Handler 

# Rules to build bin
all: bin/$(PROJECT).bin

#turns .c source files into object files
$(OBJ)%.o: src/%.c 
	$(MKDIR)              
	$(CC) -o $@ $^ $(INC) $(CFLAGS)

#turns .c source files into object files
$(OBJ)%.o: libs/%.c 
	$(MKDIR)              
	$(CC) -o $@ $^ $(INC) $(CFLAGS)
	
#contains debug symbols for GNU GDB
bin/$(PROJECT).elf: $(OBJS)
	$(MKDIR)              
	$(LD) -o $@ $^ $(LDFLAGS)

# debug symbols for GNU GDB stripped by objcopy
# finished binary ready for flashing
bin/$(PROJECT).bin: bin/$(PROJECT).elf
	$(OBJCOPY) -O binary $< $@


# Flashes bin to TM4C
flash:
	$(FLASHER) -S $(DEV) bin/$(PROJECT).bin

# remove object and bin files
clean:
	-$(RM) obj
	-$(RM) bin

.PHONY: all clean
