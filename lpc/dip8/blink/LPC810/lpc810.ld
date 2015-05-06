ENTRY(Reset_Handler);
MEMORY {
    FLASH(rx) : ORIGIN = 0x0, LENGTH = 4K
    RAM(rwx) : ORIGIN = 0x10000000, LENGTH = 1K
}

SECTIONS {
    . = ORIGIN(FLASH);
    .text : {
        KEEP(* (.isr_vectors));
	. = ALIGN(4);
	. = 0xC4;		/* The end of vectors. Just for tutorial 0 */
	__vec_end__ = .;
	* (.text);
	. = ALIGN(4);
	__end_text__ = .;
    } > FLASH
    __stack_top = ORIGIN(RAM) + LENGTH(RAM);
}

__end = .;
PROVIDE(etext = __end_text__);
PROVIDE(end = .);
