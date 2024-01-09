# **************************TARGET*************************
TARGET 			= beepc

# **************************compiler**************************
arm_linux 		:= arm-linux-gnueabihf-
arm_none  		:= arm-none-eabi-
CROSS_COMPLIE 	?= $(arm_linux)

CC 				:= $(CROSS_COMPLIE)gcc
LD 				:= $(CROSS_COMPLIE)ld
OBJCOPY 		:= $(CROSS_COMPLIE)objcopy
OBJDUMP 		:= $(CROSS_COMPLIE)objdump

# **************************dir**************************
SRC_DIR 		:= 	imx6ul \
					bsp/clk \
					bsp/delay \
					bsp/beep \
					project \

INC_DIR 		:= 	$(SRC_DIR)     # 这里的INC_DIR是为了后面的INCLUDE而定义的，因此这里的INC_DIR必须包含SRC_DIR

BUILD_DIR 		:= obj

# **************************src**************************
C_SRC 			:= $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.c))
S_SRC 			:= $(foreach dir,$(SRC_DIR),$(wildcard $(dir)/*.s))



# **************************inc**************************
INC 			:= $(foreach dir,$(INC_DIR),$(wildcard $(dir)/*.h))
# INCLUDE 		:= $(patsubst %,-I %,$(INC_DIR))	
# INCLUDE 		:= $(foreach dir,$(INC_DIR),-I $(dir))	
# INCLUDE 		:= $(addprefix -I ,$(INC_DIR))	 # 注释的三个，一样的效果
INCLUDE 		:= $(INC_DIR:%=-I%)



# **************************obj**************************
S_OBJ 			:= $(patsubst %.s,$(BUILD_DIR)/%.o,$(notdir $(S_SRC)))
C_OBJ 			:= $(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(C_SRC)))
OBJ 			:= $(S_OBJ) $(C_OBJ)

# **************************path**************************
vpath %.s $(SRC_DIR)
vpath %.c $(SRC_DIR)
vpath %.h $(INC_DIR)




# **************************cflags**************************
CFLAGS 			:= -Wall -O2 -nostdlib 


# **************************link**************************
LDFLAGS 		:= -T imx6u.lds 



# **************************target**************************
$(BUILD_DIR)/$(TARGET).bin: $(BUILD_DIR)/$(TARGET).elf
	$(OBJCOPY) -O binary $< $@
	$(OBJDUMP) -D -m arm $< > $(BUILD_DIR)/$(TARGET).dis

$(BUILD_DIR)/$(TARGET).elf: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

# $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INC) | $(BUILD_DIR)
# 	$(CC) $(CFLAGS) -c -o $@ $<

# $(BUILD_DIR)/%.o: $(SRC_DIR)/%.s $(INC) | $(BUILD_DIR)
# 	$(CC) $(CFLAGS) -c -o $@ $<

$(S_OBJ): $(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<
$(C_OBJ): $(BUILD_DIR)/%.o: %.c $(INC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<


$(BUILD_DIR):
	mkdir $(BUILD_DIR)

# **************************download************************
device ?= /dev/sdb   # !!!!!!!!! 这里一定要改对，不然系统可能会崩溃，因此不建议用make download
.PHONY: download
download: imxdownload $(device) $(BUILD_DIR)/$(TARGET).bin
	./imxdownload $(BUILD_DIR)/$(TARGET).bin $(device)

	

# **************************others**************************
.PHONY: clean print
print:
	@echo C_SRC = $(C_SRC)
	@echo S_SRC = $(S_SRC)
	@echo C_OBJ = $(C_OBJ)
	@echo S_OBJ = $(S_OBJ)
	@echo INC = $(INC)


clean:
	rm -rf $(BUILD_DIR) $(TARGET).elf $(TARGET).bin $(TARGET).dis