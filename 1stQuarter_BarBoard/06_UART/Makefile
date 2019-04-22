INCLUDE := -I ./include/uart/
INCLUDE += -I ./include/led/
INCLUDE += -I ./include/
CFLAGS := $(INCLUDE) 

SUBDIRS := ./src/

OBJS = start.o soc_s3c2440.o
ALLOBJS := $(OBJS)
ALLOBJS += led.o uart.o
C_SOURCE = soc_s3c2440.c
DEP_FILES := $(patsubst %.c,.%.d,$(C_SOURCE))
DEP_FILES := $(wildcard $(DEP_FILES))

#注意，非命令语句不要Tab缩进
ifneq ($(DEP_FILES),)
include $(DEP_FILES)
endif

Project:start.o soc_s3c2440.o subMode
	#到操作目录下进行链接并且将目标文件拷贝出来，然后返回当前目录
	@cp -u -v -f ./*.o ./target/
	@cd ./target/ && arm-linux-ld -Ttext 0 $(ALLOBJS) -o $@.elf && \
	arm-linux-objcopy -O binary -S $@.elf $@.bin && \
	arm-linux-objdump -D $@.elf > $@.dis && \
	cp -u -v -f *.bin *.elf *.dis ../ && \
	cd ../

#编译启动的配置汇编文件
start.o:start.s
	@arm-linux-gcc -c -o $@ $<	

#编译主控制程序文件
soc_s3c2440.o:soc_s3c2440.c
	@arm-linux-gcc -E $< $(CFLAGS) -o $(patsubst %.c,%.i,$<)
	@arm-linux-gcc -c $< $(CFLAGS) -o $@ -MD -MF $(patsubst %.c,.%.d,$<)

.PHONY:subMode
subMode:
	#进入模块内部编译，复制文件，并且在文件跟新的情况下为强制覆盖
	@list='$(SUBDIRS)'; for subdir in $$list; do \
		echo "compiler in $$subdir";\
		$(MAKE) -C $$subdir;\
		cp -u -v -f $$subdir/*.o ./target/ ;\
	done
	
.PHONY:clean
clean:
	@rm *.bin *.elf *.dis *.i *.o && pwd && echo "clean success!"
	@rm -f $(DEP_FILES) && pwd && \
	echo "delete depend file success!" || \
	echo "delete depend files failure！"
	@cd ./src/ && make clean &&  cd -

.PHONY:cleanTarget
cleanTarget:
	@cd ./target && rm -f *.* && cd ../