INCLUDE := -I ./include/soc_s3c2440/
CFLAGS := $(INCLUDE) 

OBJS = start.o led.o
C_SOURCE = led.c
DEP_FILES := $(patsubst %.c,.%.d,$(C_SOURCE))
DEP_FILES := $(wildcard $(DEP_FILES))

#注意，非命令语句不要Tab缩进
ifneq ($(DEP_FILES),)
include $(DEP_FILES)
endif

clockSet:$(OBJS)
	#链接两个目标文件到一个.elf文件
	#特别注意两个.o文件的顺序，当我把main生成的.o放在前面时其结果是不对的！
	@arm-linux-ld -Ttext 0 $^ -o $@.elf
	#由目标代码生成机器码的bin文件
	@arm-linux-objcopy -O binary -S $@.elf $@.bin
	#由目标代码反汇编得出标准汇编指令（非伪汇编指令）
	@arm-linux-objdump -D $@.elf > $@.dis
	
start.o:start.s
	@arm-linux-gcc -c -o $@ $<	
	
%.o:%.c
	#记录一下预处理结果中对宏的处理，便于观察
	@arm-linux-gcc -E $< $(CFLAGS) -o $(patsubst %.c,%.i,$<)
	#分别编译，生成目标文件，依赖文件存为隐藏文件
	@arm-linux-gcc -c $< $(CFLAGS) -o $@ -MD -MF $(patsubst %.c,.%.d,$<)


.PHONY:clean
clean:
	@rm *.bin *.elf *.o *.dis *.i
	
.PHONY:clean_dep
clean_dep:
	@rm -f $(DEP_FILES) && echo "delete depend file success!" || echo "delete depend files failure！"

