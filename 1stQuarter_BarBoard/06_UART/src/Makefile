INCLUDE := -I ../include/soc_s3c2440/
INCLUDE += -I ../include/uart/
INCLUDE += -I ../include/led/
INCLUDE += -I ../include/

CFLAGS := $(INCLUDE) 

OBJS := led.o uart.o
DEP_FILES := $(patsubst %.o,.%.d,$(OBJS))
DEP_FILES := $(wildcard $(DEP_FILES))

#注意，非命令语句不要Tab缩进
ifneq ($(DEP_FILES),)
include $(DEP_FILES)
endif

targetNull:$(OBJS)
	touch targetNullFile
	
%.o:%.c
	@echo "compiler $<"
	@arm-linux-gcc -E $< $(CFLAGS) -o $(patsubst %.c,%.i,$<)
	@arm-linux-gcc -c $< $(CFLAGS) -o $@ -MD -MF $(patsubst %.c,.%.d,$<)


.PHONY:clean
clean:
	@rm *.o *.i targetNullFile && pwd && echo "clean success!"
	@rm -f $(DEP_FILES) && pwd && \
	echo "delete depend file success!" || \
	echo "delete depend files failure！"