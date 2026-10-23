/*
 * md - memory disk: a root filesystem image and a swap area in memory
 * (see dev/md.c).
 */
#ifndef _SYS_MD_H_
#define _SYS_MD_H_

#ifdef KERNEL
#define MD_MAJOR        6               /* block major of md, all ports */

int     mdopen(dev_t dev, int flag, int mode);
int     mdclose(dev_t dev, int flag, int mode);
daddr_t mdsize(dev_t dev);
void    mdstrategy(struct buf *bp);
int     mdioctl(dev_t dev, u_int cmd, caddr_t addr, int flag);
int     md_rootrom(dev_t dev);

#ifdef MD_MACHDEP
/*
 * options MD_MACHDEP: the port supplies these in place of md.c's plain
 * defaults.
 */
void    md_machdep_begin(void);         /* before a transfer */
void    md_machdep_end(void);           /* after it */
void    md_machdep_write(caddr_t dst, caddr_t src, u_int len);
int     md_machdep_rootrw(void);        /* root region writable? (MD_ROOT_RW) */
void    md_machdep_swap(caddr_t *startp, caddr_t *endp); /* move swap? */
#endif
#endif /* KERNEL */

#endif /* !_SYS_MD_H_ */
