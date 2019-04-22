FILEMAIN=endian
FILESTART=start
all:
	#分别编译
	arm-linux-gcc -c -o ${FILEMAIN}.o ${FILEMAIN}.c
	arm-linux-gcc -c -o ${FILESTART}.o ${FILESTART}.S
	#链接两个.o文件到一个.elf文件
	#特别注意两个.o文件的顺序，当我把main生成的.o放在前面时其结果是不对的！
	arm-linux-ld -Ttext 0 ${FILESTART}.o ${FILEMAIN}.o -o ${FILEMAIN}.elf
	#由目标代码生成机器码的bin文件
	arm-linux-objcopy -O binary -S ${FILEMAIN}.elf ${FILEMAIN}.bin
	#由目标代码反汇编得出标准汇编指令（非伪汇编指令）
	arm-linux-objdump -D ${FILEMAIN}.elf > ${FILEMAIN}.dis
clean:
	rm *.bin *.elf *.o *.dis
