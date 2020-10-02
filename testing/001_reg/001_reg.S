	.syntax	unified
	.cpu	cortex-m4
	.thumb
	.text

	.global	Reset_Handler
	.thumb_func

	.word	_estack
	.word	Reset_Handler

	.type	Reset_Handler, %function
Reset_Handler:
	ldr	sp, =_estack

	ldr	r1, =0x00C5C490

forever:
	b	forever
