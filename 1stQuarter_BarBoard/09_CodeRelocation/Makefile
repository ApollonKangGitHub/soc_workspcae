CC := arm-linux-gcc
#-Tdata 0x10000 
LD := arm-linux-ld -Ttext 0 -Tdata 0x2500 
OBJCOPY := arm-linux-objcopy
OBJDUMP := arm-linux-objdump
COPY := cp -u -v -f

INCLUDE = -I ./include/
INCLUDE += -I ./include/tools/
INCLUDE += -I ./include/uart/
INCLUDE += -I ./include/led/
INCLUDE += -I ./include/soc_s3c2440/
INCLUDE += -I ./include/memCtrl/
INCLUDE += -I ./include/log/
CFLAGS = $(INCLUDE) 

START = start.o
ASMFILE = lib1funcs.o
OBJS = ./soc_s3c2440.o
OBJS += ./src/uart/uart.o
OBJS += ./src/led/led.o
OBJS += ./src/tools/tools.o
OBJS += ./src/log/syslog.o
OBJS += ./src/memCtrl/memCtrl.o

ALLOBJS := $(START)
#ALLOBJS += $(ASMFILE)
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
	

#编译启动的配置汇编文件
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
	@rm -f $(patsubst %.o,%.d,$(ALLOBJS)) && echo "delete depend files succeed!"