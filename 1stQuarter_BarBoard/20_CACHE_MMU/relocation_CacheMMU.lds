/*
 * format:
 *      SECTIONS {
 *      ...
 *      SECNAME START BLOCK(ALIGN) (NOLOAD) : AT ( LDADR )
 *        { CONTENTS } >REGION :PHDR =FILL
 *      ...
 */

SECTIONS {
	/* 
	 * 代码段数据段均重定位到SDRAM，不需要分开链接（代码段在Nor flash， data段在SDRAM）
	 * 其他section紧接着前一个section，且保证每个section开始地址四字节对齐
	 * 这里设置代码重定位地址不在SDRAM上，因此重定位之前就得进行MMU页表映射与使能MMU
	 */
	. = 0XB0000000;
	_code_start_ = .;
	
	. = ALIGN(4);
	.text			:	{ *(.text); }
	
	. = ALIGN(4);
	.rodata			:	{ *(.rodata); }
	
	. = ALIGN(4);
	.data 			:	{ *(.data); }

	. = ALIGN(4);						
	_bss_start_ = .;
	.bss			:	{ *(.bss); }
	
	. = ALIGN(4);							
	.common			:	{ *(common); }
	_bss_end_ = .;
}