#ifndef _SETJMP_H
#define _SETJMP_H

#include <machine/setjmp.h>

typedef int jmp_buf[_JBLEN];
typedef jmp_buf sigjmp_buf;

/*
 * Save and restore only CPU state.
 * Signal mask is not saved.
 */
int _setjmp (jmp_buf env);
void _longjmp (jmp_buf env, int val);

/*
 * Save and restore CPU state and signal mask.
 */
int setjmp (jmp_buf env);
void longjmp (jmp_buf env, int val);

/*
 * Save and restore CPU state and optionally a signal mask.
 * Signal mask is saved only when savesigs is nonzero.
 */
int sigsetjmp (sigjmp_buf env, int savesigs);
void siglongjmp (sigjmp_buf env, int val);

#endif /* !_SETJMP_H */
