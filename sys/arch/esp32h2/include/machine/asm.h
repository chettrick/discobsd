/*
 * asm.h -- RISC-V assembly macros for DiscoBSD/esp32h2.
 */
#ifndef _MACHINE_ASM_H_
#define _MACHINE_ASM_H_

#ifdef __ASSEMBLER__

#define ENTRY(name)             \
    .globl  name              ; \
    .type   name, @function   ; \
    name:

#define END(name)               \
    .size   name, . - name

#endif /* __ASSEMBLER__ */
#endif /* _MACHINE_ASM_H_ */
