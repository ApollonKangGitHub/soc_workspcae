/*
 * format:
 *      SECTIONS {
 *      ...
 *      SECNAME START BLOCK(ALIGN) (NOLOAD) : AT ( LDADR )
 *        { CONTENTS } >REGION :PHDR =FILL
 *      ...
 */

SECTIONS {
	.text 	0 			: { *(.text); }				/* text段地址在0x0开始,应用于所有文件的.text段 */
	.rodata 			: { *(.rodata); }			/* rodata段地址紧接着text段,应用于所有文件的.rodata段 */
	
	.data 	0x30000000	: AT((ADDR(.rodata) + SIZEOF (.rodata))) 				
	{
		_data_start_ = . ;							/* data段在内存中的重定位地址，运行地址 */
		_data_load_ = LOADADDR(.data);				/* data段在bin文件中的地址，加载地址 */
		*(.data);									/* data段地址设置,应用于所有文件的.data段，链接时方可确定 */
		_data_end_ = . ;							/* data段在内存中的结束地址，运行地址，链接时方可确定 */	
	}												/* data段重定位到0x30000000，并且加载地址在0x4000，不加AT的话，实际文件大小会大于0x30000000,即800M */

	_bss_start_ = . ;
	.bss 				: { *(.bss); }
	.common 			: { *(common); }
	_bss_end_ = . ;
}