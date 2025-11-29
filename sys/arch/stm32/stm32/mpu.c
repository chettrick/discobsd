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

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/file.h>
#include <sys/inode.h>
#include <sys/sysctl.h>

#include <machine/mpuvar.h>

#include <stm32/hal/stm32f4xx_ll_cortex.h>

int
mpu_sysctl(int *name, u_int namelen, void *oldp, size_t *oldlenp, void *newp,
    size_t newlen)
{

	/* All sysctl names at this level are terminal. */
	if (namelen != 1)
		return ENOTDIR;

	switch (name[0]) {
	case CPU_MPU_ENABLE:
		return sysctl_rdint(oldp, oldlenp, newp,
		    LL_MPU_IsEnabled());
	case CPU_MPU_CTRL:
		return sysctl_rdint(oldp, oldlenp, newp,
		    LL_MPU_GetCtrl());
	case CPU_MPU_NREGIONS:
		return sysctl_rdint(oldp, oldlenp, newp,
		    LL_MPU_GetNumRegions());
	case CPU_MPU_SEPARATE:
		return sysctl_rdint(oldp, oldlenp, newp,
		    LL_MPU_GetSeparate());
	default:
		return EOPNOTSUPP;
	}
}
