	/* RM0090 Rev 18 Reference Manual */
	/* https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405415-stm32f407417-stm32f427437-and-stm32f429439-advanced-armbased-32bit-mcus-stmicroelectronics.pdf */

	/* PM0214 Rev 10 Programming Manual */
	/* https://www.st.com/resource/en/programming_manual/dm00046982-stm32-cortexm4-mcus-and-mpus-programming-manual-stmicroelectronics.pdf */

	/* UM1472 Rev 6 User Manual: Discovery Kit with STM32F407VG MCU */
	/* https://www.st.com/resource/en/user_manual/dm00039084-discovery-kit-with-stm32f407vg-mcu-stmicroelectronics.pdf */

	/* DS8626 Rev 9 STM32F405xx STM32F407xx Datasheet */
	/* https://www.st.com/resource/en/datasheet/stm32f407vg.pdf */

	/* Blink LEDs by direct register manipulation in Thumb assembly. */
	/* GPIO is specified in Section 8 of the Reference Manual. */
	/* filename: 002_led.S */

	.syntax	unified
	.cpu	cortex-m4
	.thumb

	.word	_estack
	.word	Reset_Handler
	.word	SystemInit
	.word	main

	.text
	.thumb_func
	.global	Reset_Handler
	.type	Reset_Handler, %function
Reset_Handler:
	ldr	sp, =_estack	/* Set stack pointer. */

	ldr	r5, =0x00C5C490	/* Smoke test. */

	bl	SystemInit	/* Standardized clock init. */
	bl	main		/* Branch to application code. */
	bx	lr


	.text
	.thumb_func
	.global SystemInit
	.type SystemInit, %function
SystemInit:
	/* LD3, Orange LED, PD13 */
	/* LD4, Green LED,  PD12 */
	/* LD5, Red LED,    PD14 */
	/* LD6, Blue LED,   PD15 */

	/* Clock enable for GPIO Port D. S6.3.10 */
	ldr	r1, =0x40023830	/* RCC_AHB1ENR periph clock reg. S6.3.10 */
	ldr	r0, [r1]
	orr.w	r0, #0x00000008	/* GPIODEN Set Bit 3 to enable port D clock. S6.3.10 */
	str	r0, [r1]

	/* There are four 32-bit config registers for GPIO. Configure them here. S8.1 */
	/* GPIO-D control registers 0x4002 0c00 to 0x4002 0fff. S2.3 and S6.3.26 */

	/* Set GPIO Port D as general purpose output mode (01). All four pins for four LEDs. S8.4.1 */
	/* Each GPIO pin has two bits. Bits 24 to 31 control pins PD12 to PD15. */
	ldr	r1, =0x40020c00	/* GPIOD_MODER Set mode to GP output. S8.4.1 */
	ldr	r0, [r1]
	orr.w	r0, #0x55000000	/* MODER12 to MODER15 set to (01) for GP output. */
	and.w	r0, #0x55ffffff	/* Clear other bits to 00 for inputs. */
	str	r0, [r1]

	/* Set output type as push-pull. S8.4.2 */
	ldr	r1, =0x40020c04	/* GPIOD_OTYPER Sets output type as either push-pull (0) or open-drain (1). S8.4.2 */
	ldr	r0, [r1]
	and.w	r0, #0xffff0fff	/* OT12 to OT15, so clear (0) bits 12 to 15 in register for push-pull. */
	str	r0, [r1]

	/* Set port output speed to low speed (00). S8.4.3 */
	/* Each GPIO pin has two bits. Bits 24 to 31 control pins PD12 to PD15. */
	ldr	r1, =0x40020c08	/* GPIOD_OSPEEDR Sets output as either low, med, high, or very high speed. S8.4.3 */
	ldr	r0, [r1]
	and.w	r0, #0x00ffffff	/* OSPEEDR12 to OSPEED15, so clear (00) bits 24 to 31 in register for low speed. */
	str	r0, [r1]

	/* Set pull-up/pull-down register to no pull-ups or pull-downs (00). S8.4.4 */
	/* Each GPIO pin has two bits. Bits 24 to 31 control pins PD12 to PD15. */
	ldr	r1, =0x40020c0c	/* GPIOD_PUPDR Sets outputs to use weak pull-ups or pull-downs. S8.4.4 */
	ldr	r0, [r1]
	and.w	r0, #0x00ffffff	/* PUPDR12 to PUPDR15, so clear (00) bits 24 to 31 in register to disable pull-ups and pull-downs. */
	str	r0, [r1]

	bx	lr


	.text
	.thumb_func
	.global main
	.type main, %function
main:
	/* Read/Modify/Write of GPIOs. */
	/* Set outputs high to turn on LEDs. */
	ldr	r1, =0x40020c14	/* GPIOD_ODR Output Data Register S8.2 */
	ldr	r0, [r1]
	orr.w	r0, #0x0000f000	/* ODR12 to ODR15, so set (1) bits 12 to 15 in register to set output high. S8.4.6 */
	str	r0, [r1]

	/* Clear outputs low to turn off LEDs. */
	ldr	r1, =0x40020c14	/* GPIOD_ODR Output Data Register S8.2 */
	ldr	r0, [r1]
	and.w	r0, #0xffff0fff	/* ODR12 to ODR15, so clear (0) bits 12 to 15 in register to clear output low. S8.4.6 */
	str	r0, [r1]


	/* Atomic manipulation of GPIOs. */
	/* Use GPIOD_BSRR register to atomically set and clear GPIO pins. */
	ldr	r0, =0x40020c18	/* GPIOD_BSRR Bit set/reset register. S8.4.7 */
	ldr	r1, =0x0000f000	/* BS12 to BS15, so set (1) bits 12 to 15. */
	ldr	r2, =0xf0000000	/* BR12 to BR15, so set (1) bits 28 to 31. */

blink:
	str	r1, [r0]	/* Atomically set GPIO outputs high to turn on LEDs. */
	str	r2, [r0]	/* Atomically clear GPIO outputs low to turn off LEDs. */

	b	blink

	.end
