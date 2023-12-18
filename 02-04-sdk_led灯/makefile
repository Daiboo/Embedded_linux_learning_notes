# *************name************
NAME 			= ledc

# *************compiler*************
arm_linux 		:= arm-linux-gnueabihf-
arm_none  		:= arm-none-eabi-
CROSS_COMPLIE 	?= $(arm_linux)

CC 				:= $(CROSS_COMPLIE)gcc
LD 				:= $(CROSS_COMPLIE)ld
OBJCOPY 		:= $(CROSS_COMPLIE)objcopy
OBJDUMP 		:= $(CROSS_COMPLIE)objdump

# *************dir*************
SRC_DIR 		:= .
INC_DIR 		:= .
BUILD_DIR 		:= build

# *************src*************
SRC 			:= $(wildcard $(SRC_DIR)/*.c)
SRC 			+= $(wildcard $(SRC_DIR)/*.s)
# *************inc*************
INC 			:= $(wildcard $(INC_DIR)/*.h)


# *************obj*************
OBJ := $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(filter %.c,$(SRC))))
OBJ += $(patsubst %.s,$(BUILD_DIR)/%.o,$(notdir $(filter %.s,$(SRC))))



# *************flags*************
CFLAGS 			:= -Wall -O2 -nostdlib 
CFLAGS 			+= -I$(INC_DIR)


# *************link*************
LDFLAGS 		:= -T imx6u.lds 



# *************target*************
$(NAME).bin: $(NAME).elf
	$(OBJCOPY) -O binary $< $@
	$(OBJDUMP) -D $< > $(NAME).dis

$(NAME).elf: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s $(INC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# *************download***********
device ?= /dev/sdb
.PHONY: download
download: imxdownload $(device)
	./imxdownload $(NAME).bin $(device)

	

# *************others*************
.PHONY: clean print
print:
	@echo $(SRC)
	@echo $(OBJ)


clean:
	rm -rf $(BUILD_DIR) $(NAME).elf $(NAME).bin $(NAME).dis