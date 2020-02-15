CC := arm-linux-gcc
CC += -O0

#即使加了volatile，uint16的读写操作（strh/ldrh）也可能被分成两次读写uint8的指令(ldrb/strb)
#使用ARM v4版本的指令编译，保证不会发生以上问题，如果发生可能导致数据访问不同步，
CC += -march=armv4

#直接-Tdata 0x30000000重定位到SDRAM是行不通的，
#因为code段默认从0地址开始，data段和代码段之间就存在一大片未使用的地址
#大致bin文件达到800多兆

#LD := arm-linux-ld -Ttext 0 -Tdata 0x30000000
LD := arm-linux-ld -T relocation.lds
OBJCOPY := arm-linux-objcopy
OBJDUMP := arm-linux-objdump
COPY := cp -u -v -f

INCLUDE = -I ./include/
INCLUDE += -I ./include/tools/
INCLUDE += -I ./include/uart/
INCLUDE += -I ./include/led/
INCLUDE += -I ./include/soc_s3c2440/
INCLUDE += -I ./include/memCtrl/
INCLUDE += -I ./include/exception/
INCLUDE += -I ./include/timer/
INCLUDE += -I ./include/log/
INCLUDE += -I ./include/nor_flash/
INCLUDE += -I ./include/nand_flash/
INCLUDE += -I ./include/test/

CFLAGS = $(INCLUDE) 

#测试THUMB指令集，C文件编译为thumb指令"-mthumb"，start.S编译为arm指令集
#THUMB指令的优点是指令长度小，生成bin文件小，但是thumb指令对于/和%的支持不好，
#并且thumb指令集有时需要arm指令集配合才能实现复杂功能，因此还是默认arm指令编译
THUMB_INSTRUCTION_TEST = FALSE
#未定义指令异常测试
UNDF_INSTRUCTION_EXCEPTION = FALSE
#软件终端异常
SWI_EXCEPTION = FALSE
#Nand 启动测试，需要保证代码重定位的code、SDRAM初始化的code、NorFlash的code等，
#其编译后在前4K位置，以确保在代码重定位之前可以正常初始化完毕，以及随后功能正常
#中断的执行代码不放在前4K，只要确保不会有中断打断初始化过程即可，init很快，因此
#正常来讲内部中断、外部中断也只是在初始化完成后才开始初始化中断并使用，
#但是如果在初始化代码重定位完成前，真的有中断产生，中断处理的代码还必须放在前4K
NAND_BOOT_TEST = TRUE

ifeq ($(NAND_BOOT_TEST), TRUE) 
	CC += -D TEST_NAND_COMMENT_USELESS_CODE=TRUE
else 
	CC += -D TEST_NAND_COMMENT_USELESS_CODE=FALSE
endif 

ifeq ($(THUMB_INSTRUCTION_TEST), TRUE) 
	CC += -D SOC_S3C2440_THUMB_INSTRUCTION_TEST=TRUE
	CFLAGS += -mthumb
else 
	CC += -D SOC_S3C2440_THUMB_INSTRUCTION_TEST=FALSE
	CFLAGS += -marm
endif 

ifeq ($(UNDF_INSTRUCTION_EXCEPTION), TRUE) 
	CC += -D SOC_S3C2440_UND_INSTRUCTION_TEST=TRUE
else 
	CC += -D SOC_S3C2440_UND_INSTRUCTION_TEST=FALSE
endif 

ifeq ($(SWI_EXCEPTION), TRUE) 
	CC += -D SOC_S3C2440_SWI_INSTRUCTION_TEST=TRUE
else 
	CC += -D SOC_S3C2440_SWI_INSTRUCTION_TEST=FALSE
endif 

START = start.o
#ASMFILE = lib1funcs.o

OBJS += ./src/memCtrl/memCtrl.o
OBJS += ./src/nand_flash/nand_flash.o
OBJS += ./src/soc_s3c2440_init.o
OBJS += ./src/uart/uart.o
OBJS += ./src/exception/interrupt.o
OBJS += ./src/led/led.o
OBJS += ./src/tools/tools.o
OBJS += ./src/log/syslog.o
OBJS += ./src/test/test.o
OBJS += ./src/exception/exception.o
OBJS += ./src/timer/timer.o
OBJS += ./src/nor_flash/nor_flash.o
OBJS += ./soc_s3c2440.o

ALLOBJS := 
ALLOBJS += $(START)
ALLOBJS += $(OBJS)
ALLOBJS += $(ASMFILE)

DEP_FILES := $(patsubst %.o,%.d,$(OBJS))
DEP_FILES := $(wildcard $(DEP_FILES))

TARDIR := ./target/
TARGETP_ROJECT := S3C2440
#注意，非命令语句不要Tab缩进
ifneq ($(DEP_FILES),)
include $(DEP_FILES)
endif

$(TARGETP_ROJECT):$(ALLOBJS)
	@echo "commpile project $(TARGETP_ROJECT) is running..." && \
	$(LD) $(START) $(OBJS) -o $@.elf && \
	$(OBJCOPY) -O binary -S $@.elf $@.bin && \
	$(OBJDUMP) -D $@.elf > $@.dis && \
	echo "link all files to target file $(TARGETP_ROJECT).bin succeed!" || \
	echo "link all files to target file $(TARGETP_ROJECT).bin failure!"
	

#编译启动的配置汇编文件,汇编文件使用ARM指令
$(START):$(patsubst %.o,%.s,$(START))
	@echo "$< compile running..." && \
	$(CC) -c -o $@ $<	&& \
	echo "$< compile succeed!" || \
	echo "$< compile failure!"
$(ASMFILE):$(patsubst %.o,%.s,$(ASMFILE))
	@echo "$< compile running..." && \
	$(CC) -c -o $@ $<	&& \
	echo "$< compile succeed!" || \
	echo "$< compile failure!"
	
#编译C程序文件
%.o:%.c
	@echo "$< compile running..." && \
	$(CC) -E $< $(CFLAGS) -o $(patsubst %.c,%.i,$<) && \
	$(CC) -c $< $(CFLAGS) -o $@ -MD -MF $(patsubst %.c,%.d,$<) && \
	echo "$< compile succeed!" || \
	echo "$< compile failure!"
	
.PHONY:clean
clean:
	@rm -f $(ALLOBJS) && \
	rm -f $(TARGETP_ROJECT).elf $(TARGETP_ROJECT).dis $(TARGETP_ROJECT).bin && \
	rm -f $(patsubst %.o,%.i,$(ALLOBJS)) && \
	echo "delete module files succeed!"
	
.PHONY:cleanDepend
cleanDepend:
	@rm -f $(ALLOBJS) && \
	rm -f $(TARGETP_ROJECT).elf $(TARGETP_ROJECT).dis $(TARGETP_ROJECT).bin && \
	rm -f $(patsubst %.o,%.i,$(ALLOBJS)) && \
	echo "delete all module files succeed!"
	@rm -f $(patsubst %.o,%.d,$(ALLOBJS)) && echo "delete all depend files succeed!"
