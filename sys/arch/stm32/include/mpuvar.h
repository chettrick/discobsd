/*
 * Copyright (c) 2025 Christopher Hettrick <chris@structfoo.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef	_MACHINE_MPUVAR_H_
#define	_MACHINE_MPUVAR_H_

int	mpu_sysctl(int *, u_int, void *, size_t *, void *, size_t);

/*
 * CTL_MPU definitions.
 */
#define	CPU_MPU_ENABLE		1
#define	CPU_MPU_CTRL		2
#define	CPU_MPU_NREGIONS	3
#define	CPU_MPU_SEPARATE	4
#define	CPU_MPU_MAXID		5

#ifndef	KERNEL
#define	CTL_MPU_NAMES { \
	{ 0, 0 }, \
	{ "enable", CTLTYPE_INT }, \
	{ "ctrl", CTLTYPE_INT }, \
	{ "nregions", CTLTYPE_INT }, \
	{ "separate", CTLTYPE_INT }, \
}
#endif	/* !KERNEL */

#endif	/* !_MACHINE_MPUVAR_H_ */
