#include <sys/param.h>
#include <sys/systm.h>
#include <sys/map.h>
#include <sys/inode.h>
#include <sys/user.h>
#include <sys/proc.h>
#include <sys/buf.h>
#include <sys/namei.h>
#include <sys/fs.h>
#include <sys/mount.h>
#include <sys/file.h>
#include <sys/resource.h>
#include <sys/exec.h>
#include <sys/dir.h>
#include <sys/uio.h>
#include <machine/debug.h>
#include <machine/frame.h>

/*
 * How memory is set up.
 *
 * var a:
 *__user_data_end:!----------!
 *                ! +P_ssize ! stack
 * p_saddr ->     !----------!
 *
 *                !----------!
 *                ! +P_dsize ! .data + .bss + heap
 * P_daddr ->     !----------!
 *
 * var b:
 *
 *__user_data_end:!--------!
 *                ! +ssize ! stack
 * saddr ->       !--------!
 *                ! +hsize ! heap
 * haddr ->       !--------!
 *                ! +dsize ! .data + .bss
 * daddr ->       !--------!
 *                ! +tsize ! .text
 * taddr ->       !--------!
 * paddr -> +psize
 *
 * var c:
 *                !--------!
 *                ! +tsize ! .text (read only section)
 * taddr ->       !--------!
 *                ! +ssize ! stack
 * saddr ->       !--------!
 *                ! +hsize ! heap
 * haddr ->       !--------!
 *                ! +dsize ! .data + .bss
 * daddr ->       !--------!
 * paddr -> +psize
 */

/*
 * Set up user memory.
 *
 * The following is a key to top of the stack and the variables used.
 *
 *  topp->      [argv]      top word for /bin/ps
 *               <0>
 *                n
 *                g
 *                r
 *                a
 *               ...
 *               <0>
 *                0
 *                g
 *                r
 *  ucp ->        a
 *               [0]
 *              [envn]
 *               ...
 *  envp ->     [env0]
 *               [0]
 *              [argn]      ptr to argn
 *               ...
 *  argp ->     [arg0]      ptr to arg0
 *               []       \
 *               []       | 16 bytes for Mips
 *               []       | 64 bytes for Arm
 *    sp ->      []       /
 *
 */
void exec_setupstack(unsigned entryaddr, struct exec_params *epp)
{
    int nc,i;
    u_int len;
    char *ucp;
    char **argp, **envp, ***topp;

    DEBUG("\texec_setupstack(): start\n");

    /*
     * Set up top of stack structure as above
     * This depends on that kernel and user spaces
     * map to the same addresses.
     */
    topp = (char ***)(epp->stack.vaddr + epp->stack.len - NBPW);            /* Last word of RAM */
    ucp = (char *)((unsigned)topp - roundup(epp->envbc + epp->argbc,NBPW)); /* arg string space */
    envp = (char **)(ucp - (epp->envc+1)*NBPW); /* Make place for envp[...], +1 for the 0 */
    argp = envp - (epp->argc+1)*NBPW;           /* Make place for argv[...] */

#ifdef __mips__
    u.u_frame->tf_sp = (int)(argp-16);
    u.u_frame->tf_r4 = epp->argc;               /* $a0 := argc */
    u.u_frame->tf_r5 = (int)argp;               /* $a1 := argp */
    u.u_frame->tf_r6 = (int)envp;               /* $a2 := env */
#elif __thumb2__ || __thumb__
    u.u_frame->tf_sp = (int)(argp-0x40);        /* 0x40 for svc trap frame. */
    u.u_frame->tf_r0 = epp->argc;               /* $a1 := argc */
    u.u_rval         = epp->argc;               /* $a1 := argc via syscall() */
    u.u_frame->tf_r1 = (int)argp;               /* $a2 := argp */
    u.u_frame->tf_r2 = (int)envp;               /* $a3 := envp */

    u.u_frame->tf_lr  = 0xffffffff;             /* lr, set -1 (reset value) */
    u.u_frame->tf_psr = 0x01000000;             /* psr, Thumb state bit set */
#else
#error "set up top of stack for unknown architecture"
#endif

    *topp = argp;                               /* for /bin/ps */

    /*
     * copy the arguments into the structure
     */
    nc = 0;
    for (i = 0; i < epp->argc; i++) {
        argp[i] = (caddr_t)ucp;
        if (copystr((caddr_t)epp->argp[i], (caddr_t)ucp, (caddr_t)topp-ucp, &len) == 0) {
            nc += len;
            ucp += len;
        }
    }
    argp[epp->argc] = NULL;

    for (i = 0; i < epp->envc; i++) {
        envp[i] = ucp;
        if (copystr((caddr_t)epp->envp[i], (caddr_t)ucp, (caddr_t)topp-ucp, &len) == 0) {
            nc += len;
            ucp += len;
        }
    }
    envp[epp->envc] = NULL;

    ucp = (caddr_t)roundup((unsigned)ucp, NBPW);
    if ((caddr_t)ucp != (caddr_t)topp) {
        DEBUG("\texec_setupstack(): error: copy arg list, ucp = %#x, topp = %#x\n", ucp, topp);
        panic("exec check");
    }

    u.u_frame->tf_pc = entryaddr;
    DEBUG("\texec_setupstack(): new PC = %#x\n", entryaddr);

    /*
     * Remember file name for accounting.
     */
    (void) copystr(argp[0], u.u_comm, MAXCOMLEN, 0);

    DEBUG("\texec_setupstack(): end\n");
}

/*
 * A simple memory allocator used within exec code using file buffers as storage.
 * Will return NULL if allocation is not possible.
 * Total max memory allocatable is MAXALLOCBUF*MAXBSIZE
 * Max size of allocatable chunk is MAXBSIZE
 *
 * All memory allocated with this function will be freed by a call
 * to exec_alloc_freeall()
 */
void *exec_alloc(int size, int ru, struct exec_params *epp)
{
    char *cp;
    int i;

    for (i = 0; i < MAXALLOCBUF; i++)
        if (MAXBSIZE - (ru<=1?epp->alloc[i].fill:roundup(epp->alloc[i].fill,ru)) >= size)
            break;
    if (i == MAXALLOCBUF)
        return NULL;
    if (epp->alloc[i].bp == NULL) {
        if ((epp->alloc[i].bp = geteblk()) == NULL) {
            DEBUG("\texec_alloc(): no buf\n");
            return NULL;
        }
    }
    if (ru > 1)
        epp->alloc[i].fill = roundup(epp->alloc[i].fill, ru);
    cp = epp->alloc[i].bp->b_addr + epp->alloc[i].fill;
    epp->alloc[i].fill += size;
    bzero (cp, size);
    return cp;
}

/*
 * this will deallocate all memory allocated by exec_alloc
 */
void exec_alloc_freeall(struct exec_params *epp)
{
    int i;
    for (i = 0; i < MAXALLOCBUF; i++) {
        if (epp->alloc[i].bp) {
            brelse(epp->alloc[i].bp);
            epp->alloc[i].bp = NULL;
            epp->alloc[i].fill = 0;
        }
    }
}

/*
 * Establish memory for the image based on the
 * values picked up from the executable file and stored
 * in the exec params block.
 */
int exec_estab(struct exec_params *epp)
{
    DEBUG("\texec_estab(): start\n");

    DEBUG("\texec_estab(): text  = %#x..%#x, len = %d\n", epp->text.vaddr,
      epp->text.vaddr + epp->text.len, epp->text.len);
    DEBUG("\texec_estab(): data  = %#x..%#x, len = %d\n", epp->data.vaddr,
      epp->data.vaddr + epp->data.len, epp->data.len);
    DEBUG("\texec_estab(): bss   = %#x..%#x, len = %d\n", epp->bss.vaddr,
      epp->bss.vaddr + epp->bss.len, epp->bss.len);
    DEBUG("\texec_estab(): heap  = %#x..%#x, len = %d\n", epp->heap.vaddr,
      epp->heap.vaddr + epp->heap.len, epp->heap.len);
    DEBUG("\texec_estab(): stack = %#x..%#x, len = %d\n", epp->stack.vaddr,
      epp->stack.vaddr + epp->stack.len, epp->stack.len);

    /*
     * Right now we can only handle the simple original a.out
     * case, so we double check for that case here.
     */
    if (epp->text.vaddr != NO_ADDR || epp->data.vaddr == NO_ADDR ||
      epp->data.vaddr != (caddr_t)__user_data_start ||
      epp->stack.vaddr != (caddr_t)__user_data_end - epp->stack.len) {
        DEBUG("\texec_estab(): error: not an a.out executable\n");
        return ENOMEM;
    }

    /*
     * Try out for overflow
     */
    if (epp->text.len + epp->data.len + epp->heap.len + epp->stack.len > MAXMEM) {
        DEBUG("\texec_estab(): error: memory overflow\n");
        return ENOMEM;
    }

    if (roundup((unsigned)epp->data.vaddr + epp->data.len, NBPW) != roundup((unsigned)epp->bss.vaddr, NBPW)) {
        DEBUG("\texec_estab(): error: .bss does not follow .data\n");
        return ENOMEM;
    }

    /*
     * Allocate core at this point, committed to the new image.
     */
    u.u_prof.pr_scale = 0;
    if (u.u_procp->p_flag & SVFORK)
        endvfork();
    u.u_procp->p_dsize = epp->data.len + epp->bss.len;
    u.u_procp->p_daddr = (size_t)epp->data.vaddr;
    u.u_procp->p_ssize = epp->stack.len;
    u.u_procp->p_saddr = (size_t)epp->stack.vaddr;

    DEBUG("\texec_estab(): core allocation\n");
    DEBUG("\texec_estab(): daddr = %#x..%#x, len = %d\n", u.u_procp->p_daddr,
      u.u_procp->p_daddr + u.u_procp->p_dsize, u.u_procp->p_dsize);
    DEBUG("\texec_estab(): saddr = %#x..%#x, len = %d\n", u.u_procp->p_saddr,
      u.u_procp->p_saddr + u.u_procp->p_ssize, u.u_procp->p_ssize);

    DEBUG("\texec_estab(): end\n");

    return 0;
}


/*
 * Save argv[] and envp[]
 */
void exec_save_args(struct exec_params *epp)
{
    unsigned len;
    caddr_t cp;
    int argc, i, l;
    char **argp, *ap;

    DEBUG("\texec_save_args(): start\n");

    epp->argc = epp->envc = 0;
    epp->argbc = epp->envbc = 0;

    argc = 0;
    if ((argp = epp->userargp) != NULL)
        while (argp[argc])
            argc++;
    if (epp->sh.interpreted) {
        argc++;
        if (epp->sh.interparg[0])
            argc++;
    }
    if (argc != 0) {
        if ((epp->argp = (char **)exec_alloc(argc * sizeof(char *), NBPW, epp)) == NULL)
            return;
        for (;;) {
            /*
             * For a interpreter script, the arg list is changed to
             * #! <interpreter name> <interpreter arg>
             * arg[0] - the interpreter executable name (path)
             * arg[1] - interpreter arg (optional)
             * arg[2 or 1] - script name
             * arg[3 or 2...] - script arg[1...]
             */
            if (argp)
                ap = *argp++;
            else
                ap = NULL;

            if (epp->sh.interpreted) {
                if (epp->argc == 0)
                    ap = epp->sh.interpname;
                else if (epp->argc == 1 && epp->sh.interparg[0]) {
                    ap = epp->sh.interparg;
                    --argp;
                } else if ((epp->argc == 1 || (epp->argc == 2 && epp->sh.interparg[0]))) {
                    ap = epp->userfname;
                    --argp;
                }
            }
            if (ap == 0)
                break;
            l = strlen(ap)+1;
            if ((cp = exec_alloc(l, 1, epp)) == NULL)
                return;
            if (copystr(ap, cp, l, &len) != 0)
                return;
            epp->argp[epp->argc++] = cp;
            epp->argbc += len;;
        }
    }
    argc = 0;
    if ((argp = epp->userenvp) != NULL)
        while (argp[argc])
            argc++;
    epp->envc = 0;
    epp->envbc = 0;
    if (argc != 0) {
        if ((epp->envp = (char **)exec_alloc(argc * sizeof(char *), NBPW, epp)) == NULL)
            return;
        for (;;) {
            if (argp)
                ap = *argp++;
            else
                ap = NULL;
            if (ap == 0)
                break;
            l = strlen(ap)+1;
            if ((cp = exec_alloc(l, 1, epp)) == NULL)
                return;
            if (copystr(ap, cp, l, &len) != 0)
                return;
            epp->envp[epp->envc++] = cp;
            epp->envbc += len;
        }
    }

    for (i = 0; i < epp->argc; i++)
        DEBUG("\texec_save_args(): arg[%d] = \"%s\"\n", i, epp->argp[i]);

    for (i = 0; i < epp->envc; i++)
        DEBUG("\texec_save_args(): env[%d] = \"%s\"\n", i, epp->envp[i]);

    DEBUG("\texec_save_args(): end\n");
}

void exec_clear(struct exec_params *epp)
{
    char *cp;
    int cc;

    DEBUG("\texec_clear(): start\n");

    /* clear BSS  */
    if (epp->bss.len > 0)
        bzero((void *)epp->bss.vaddr, epp->bss.len);
    if (epp->heap.len > 0)
        bzero((void *)epp->heap.vaddr, epp->heap.len);
    /* Clear stack */
    bzero((void *)epp->stack.vaddr, epp->stack.len);

    /*
     * set SUID/SGID protections, if no tracing
     */
    if ((u.u_procp->p_flag & P_TRACED) == 0) {
        u.u_uid = epp->uid;
        u.u_procp->p_uid = epp->uid;
        u.u_groups[0] = epp->gid;
    } else
        psignal (u.u_procp, SIGTRAP);
    u.u_svuid = u.u_uid;
    u.u_svgid = u.u_groups[0];

    u.u_tsize = epp->text.len;
    u.u_dsize = epp->data.len + epp->bss.len;
    u.u_ssize = epp->stack.len;

    /*
     * Clear registers.
     */
#ifdef __mips__
    u.u_frame->tf_r1  = 0;              /* $at */
    u.u_frame->tf_r2  = 0;              /* $v0 */
    u.u_frame->tf_r3  = 0;              /* $v1 */
    u.u_frame->tf_r7  = 0;              /* $a3 */
    u.u_frame->tf_r8  = 0;              /* $t0 */
    u.u_frame->tf_r9  = 0;              /* $t1 */
    u.u_frame->tf_r10 = 0;              /* $t2 */
    u.u_frame->tf_r11 = 0;              /* $t3 */
    u.u_frame->tf_r12 = 0;              /* $t4 */
    u.u_frame->tf_r13 = 0;              /* $t5 */
    u.u_frame->tf_r14 = 0;              /* $t6 */
    u.u_frame->tf_r15 = 0;              /* $t7 */
    u.u_frame->tf_r16 = 0;              /* $s0 */
    u.u_frame->tf_r17 = 0;              /* $s1 */
    u.u_frame->tf_r18 = 0;              /* $s2 */
    u.u_frame->tf_r19 = 0;              /* $s3 */
    u.u_frame->tf_r20 = 0;              /* $s4 */
    u.u_frame->tf_r21 = 0;              /* $s5 */
    u.u_frame->tf_r22 = 0;              /* $s6 */
    u.u_frame->tf_r23 = 0;              /* $s7 */
    u.u_frame->tf_r24 = 0;              /* $t8 */
    u.u_frame->tf_r25 = 0;              /* $t9 */
    u.u_frame->tf_fp  = 0;
    u.u_frame->tf_ra  = 0;
    u.u_frame->tf_lo  = 0;
    u.u_frame->tf_hi  = 0;
    u.u_frame->tf_gp  = 0;
#elif __thumb2__ || __thumb__
    u.u_frame->tf_r0  = 0;              /* a1 */
    u.u_frame->tf_r1  = 0;              /* a2 */
    u.u_frame->tf_r2  = 0;              /* a3 */
    u.u_frame->tf_r3  = 0;              /* a4 */
    u.u_frame->tf_ip  = 0;              /* sp, as passed as ip */
    u.u_frame->tf_lr  = 0;              /* lr */
    u.u_frame->tf_pc  = 0;              /* pc */
    u.u_frame->tf_psr = 0;              /* psr */
    u.u_frame->tf_r4  = 0;              /* v1 */
    u.u_frame->tf_r5  = 0;              /* v2 */
    u.u_frame->tf_r6  = 0;              /* v3 */
    u.u_frame->tf_r7  = 0;              /* v4 */
    u.u_frame->tf_r8  = 0;              /* v5 */
    u.u_frame->tf_r9  = 0;              /* v6 */
    u.u_frame->tf_r10 = 0;              /* v7 */
    u.u_frame->tf_r11 = 0;              /* v8 */
#else
#error "clear trap frame registers for unknown architecture"
#endif

    execsigs (u.u_procp);

    /*
     * Clear (close) files
     */
    for (cp = u.u_pofile, cc = 0; cc <= u.u_lastfile; cc++, cp++) {
        if (*cp & UF_EXCLOSE) {
            (void) closef (u.u_ofile [cc]);
            u.u_ofile [cc] = NULL;
            *cp = 0;
        }
    }
    while (u.u_lastfile >= 0 && u.u_ofile [u.u_lastfile] == NULL)
        u.u_lastfile--;

    DEBUG("\texec_clear(): end\n");
}
