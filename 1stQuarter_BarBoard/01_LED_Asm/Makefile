#FILENAME=light_led
FILENAME=light_led_all
all:
	#预处理和编译
	arm-linux-gcc -c -o ${FILENAME}.o ${FILENAME}.S
	#连接生成带符号表等信息的目标代码
	arm-linux-ld -Ttext 0 ${FILENAME}.o -o ${FILENAME}.elf
	#由目标代码生成机器码的bin文件
	arm-linux-objcopy -O binary -S ${FILENAME}.elf ${FILENAME}.bin
	#由目标代码反汇编得出标准汇编指令（非伪汇编指令）
	arm-linux-objdump -D ${FILENAME}.elf > ${FILENAME}.dis
clean:
	rm *.bin *.elf *.o *.dis >> /dev/null
