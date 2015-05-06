	.syntax unified
	.thumb

	.section .isr_vectors
	.align
	.long __stack_top
	.long Reset_Handler
	
	.text
	.align

	.thumb_func
	.global Reset_Handler
Reset_Handler:
crt0:
	b main
