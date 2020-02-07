CC := arm-linux-gcc -O0

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
INCLUDE += -I ./include/log/
INCLUDE += -I ./include/test/

CFLAGS = $(INCLUDE) 

#测试THUMB指令集，C文件编译为thumb指令"-mthumb"，start.S编译为arm指令集
#THUMB指令的优点是指令长度小，生成bin文件小，但是thumb指令对于/和%的支持不好，
#并且thumb指令集有时需要arm指令集配合才能实现复杂功能，因此还是默认arm指令编译
THUMB_INSTRUCTION_TEST = FALSE
#未定义指令异常测试
UNDF_INSTRUCTION_EXCEPTION = TRUE

ifeq ($(THUMB_INSTRUCTION_TEST), TRUE) 
	CC += -D SOC_S3C2440_THUMB_INSTRUCTION_TEST=TRUE
	CFLAGS += -mthumb
else 
	CC += -D SOC_S3C2440_THUMB_INSTRUCTION_TEST=FALSE
	CFLAGS += -marm
endif 

ifeq ($(UNDF_INSTRUCTION_EXCEPTION), TRUE) 
	CC += -D SOC_S3C2440_UNDEFINED_INSTRUCTION_TEST=TRUE
else 
	CC += -D SOC_S3C2440_UNDEFINED_INSTRUCTION_TEST=FALSE
endif 

START = start.o
#ASMFILE = lib1funcs.o
OBJS = ./soc_s3c2440.o
OBJS += ./src/uart/uart.o
OBJS += ./src/led/led.o
OBJS += ./src/tools/tools.o
OBJS += ./src/log/syslog.o
OBJS += ./src/memCtrl/memCtrl.o
OBJS += ./src/test/test.o
OBJS += ./src/exception/exception.o

ALLOBJS := $(START)
ALLOBJS += $(ASMFILE)
ALLOBJS += $(OBJS)

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
