A = a b c.pl aaea af bag ia j kka n.txt b.txt c.jpg

#过滤符合格式的参数"$(filter va1 var2 var3 ...,list)"#
B = $(filter %a,$(A))

#过滤不符合格式的参数:"$(filter-out va1 var2 var3 ...,list)"#
C = $(filter-out %a %.txt %.jpg %.pl,$(A))

#对列表中的参数执行指定操作:"$(foreach var,list,operator)"#

D_HEAD = $(foreach f,$(C),$(f).h $(f).hpp)
D_SOURCE = $(foreach f,$(C),$(f).c $(f).cpp $(f).i $(f).s $(f).asm $(f).hpp)
D_TARGET = $(foreach f,$(C),$(f).bin $(f).o $(f).obj $(f).exe $(f).pck $(f).sta)
D += $(D_HEAD)
D += $(D_SOURCE)
D += $(D_TARGET)

#获取指定文件下的指定类型文件:"$(wildcard file1 file2 file3...)"#
SRC = $(wildcard *.c *.cpp *.s)
HEAD = $(wildcard *.h *.hpp)
#对指定类型文件名进行修改:"$(patsubst format1 format2 ...,format_trget,list)"#
C_OBJ = $(patsubst %.c,%.o,$(SRC))
CPP_OBJ = $(patsubst %.cpp,%.o,$(SRC))
S_OBJ = $(patsubst %.s %asm,%.o,$(SRC))

first:target_B target_C target_file format_file
	@echo A = $(A)

.PHONY:target_B
target_B:
	@echo B = $(B)	

.PHONY:target_C
target_C:
	@echo C = $(C)

.PHONY:target_file
target_file:
	@echo D = $(D)	
	@touch $(D)

.PHONY:target_choose
target_choose:
	@echo $1 $2 $3
	@touch ${$3}
	
.PHONY:format_file
format_file:
	@echo SRC = $(SRC)
	@echo HEAD = $(HEAD)
	@echo C_OBJ = $(C_OBJ)
	@echo CPP_OBJ = $(CPP_OBJ)
	@echo S_OBJ = $(S_OBJ)

.PHONY:clean
clean:
	rm -f *.*


.PHONY:clean_head
clean_head:
	rm -f *.h *.hpp



.PHONY:clean_source
clean_source:
	rm -f *.c *.cpp *.s *.asm *.i


.PHONY:clean_target
clean_target:
	rm -f *.o *.bin *.exe *.sta *.pck *.obj

