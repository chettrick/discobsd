/*
 * Copyright (c) 1986 Regents of the University of California.
 * All rights reserved.  The Berkeley software License Agreement
 * specifies the terms and conditions for redistribution.
 *
 *	@(#)cpu.h	1.5 (2.11BSD GTE) 1998/4/3
 */

/*
 * CTL_MACHDEP definitions.
 */
#define	CPU_CONSDEV		1	/* dev_t: console terminal device */
#define	CPU_ERRMSG		2	/* get error message by errno */
#define	CPU_NLIST		3	/* get name address */
#define	CPU_FREQ_KHZ		4	/* processor clock in kHz */
#define	CPU_BUS_KHZ		5	/* i/o bus clock in kHz */
#define	CPU_MAXID		6	/* number of valid machdep ids */

#ifndef	KERNEL
#define	CTL_MACHDEP_NAMES { \
	{ 0, 0 }, \
	{ "console_device", CTLTYPE_STRUCT }, \
	{ 0, 0 }, \
	{ 0, 0 }, \
	{ "cpu_khz", CTLTYPE_INT }, \
	{ "bus_khz", CTLTYPE_INT }, \
}
#endif	/* !KERNEL */
