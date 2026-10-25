/*
 * UNIX shell
 *
 * Bell Telephone Laboratories
 */

/* To use stack as temporary workspace across
 * possible storage allocation (eg name lookup)
 * a) get ptr from `relstak'
 * b) can now use `pushstak'
 * c) then reset with `setstak'
 * d) `absstak' gives real address if needed
 */
#define relstak()	(staktop-stakbot)
#define absstak(x)	(stakbot+Rcheat(x))
#define setstak(x)	(staktop=absstak(x))
#define pushstak(c)	(GROWSTAK(staktop), *staktop++=(c))
#define zerostak()	(GROWSTAK(staktop), *staktop=0)

/* Before storing at p in the stack: make sure the break covers it.
 * Writing past the break used to be caught as SIGSEGV (fault()) and
 * the break grown then; without an MMU nothing faults, and the next
 * brk() zero-fills what was written.
 */
#define GROWSTAK(p)	((p) >= brkend ? growstak(p) : 0)
extern int		growstak();

/* Used to address an item left on the top of
 * the stack (very temporary)
 */
#define curstak()	(staktop)

/* `usestak' before `pushstak' then `fixstak'
 * These routines are safe against heap
 * being allocated.
 */
#define usestak()	{locstak();}

/* for local use only since it hands
 * out a real address for the stack top
 */
extern char		*locstak();

/* Will allocate the item being used and return its
 * address (safe now).
 */
#define fixstak()	endstak(staktop)

/* For use after `locstak' to hand back
 * new stack top and then allocate item
 */
extern char		*endstak();

/* Copy a string onto the stack and
 * allocate the space.
 */
extern char		*cpystak();

/* Allocate given ammount of stack space */
extern char		*getstak();

/* A chain of ptrs of stack blocks that
 * have become covered by heap allocation.
 * `tdystak' will return them to the heap.
 */
extern struct blk	*stakbsy;

/* Base of the entire stack */
extern char		*stakbas;

/* Top of entire stack */
extern char		*brkend;

/* Base of current item */
extern char		*stakbot;

/* Top of current item */
extern char		*staktop;

/* Used with tdystak */
extern char		*savstak();
