/*
 * tclVar.c --
 *
 *	This file contains routines that implement Tcl variables
 *	(both scalars and arrays).
 *
 *	The implementation of arrays is modelled after an initial
 *	implementation by Karl Lehenbauer, Mark Diekhans and
 *	Peter da Silva.
 *
 * Copyright 1987-1991 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */
#include "internal.h"
#include <assert.h>

/*
 * The strings below are used to indicate what went wrong when a
 * variable access is denied.
 */

static unsigned char *noSuchVar
	= (unsigned char*) "no such variable";
static unsigned char *isArray
	= (unsigned char*) "variable is array";
static unsigned char *needArray
	= (unsigned char*) "variable isn't array";
static unsigned char *noSuchElement
	= (unsigned char*) "no such element in array";
static unsigned char *traceActive
	= (unsigned char*) "trace is active on variable";

/*
 * Forward references to procedures defined later in this file:
 */

static unsigned char *	CallTraces (Interp *iPtr, Var *arrayPtr,
			    Tcl_HashEntry *hPtr, unsigned char *part1,
			    unsigned char *part2, int flags);
static void		DeleteSearches (Var *arrayVarPtr);
static void		DeleteArray (Interp *iPtr, unsigned char *arrayName,
			    Var *varPtr, int flags);
static Var *		NewVar (int space);
static ArraySearch *	ParseSearchId (Tcl_Interp *interp,
			    Var *varPtr, unsigned char *varName, char *string);
static void		VarErrMsg (Tcl_Interp *interp,
			    unsigned char *part1, unsigned char *part2,
			    unsigned char *operation, unsigned char *reason);

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GetVar --
 *
 *	Return the value of a Tcl variable.
 *
 * Results:
 *	The return value points to the current value of varName.  If
 *	the variable is not defined or can't be read because of a clash
 *	in array usage then a NULL pointer is returned and an error
 *	message is left in interp->result if the TCL_LEAVE_ERR_MSG
 *	flag is set.  Note:  the return value is only valid up until
 *	the next call to Tcl_SetVar or Tcl_SetVar2;  if you depend on
 *	the value lasting longer than that, then make yourself a private
 *	copy.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

unsigned char *
Tcl_GetVar(interp, varName, flags)
    Tcl_Interp *interp;		/* Command interpreter in which varName is
				 * to be looked up. */
    unsigned char *varName;	/* Name of a variable in interp. */
    int flags;			/* OR-ed combination of TCL_GLOBAL_ONLY
				 * or TCL_LEAVE_ERR_MSG bits. */
{
    register unsigned char *p;

    /*
     * If varName refers to an array (it ends with a parenthesized
     * element name), then handle it specially.
     */

    for (p = varName; *p != '\0'; p++) {
	if (*p == '(') {
	    unsigned char *result;
	    unsigned char *open = p;

	    do {
		p++;
	    } while (*p != '\0');
	    p--;
	    if (*p != ')') {
		goto scalar;
	    }
	    *open = '\0';
	    *p = '\0';
	    result = Tcl_GetVar2(interp, varName, open+1, flags);
	    *open = '(';
	    *p = ')';
	    return result;
	}
    }

    scalar:
    return Tcl_GetVar2(interp, varName, 0, flags);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GetVar2 --
 *
 *	Return the value of a Tcl variable, given a two-part name
 *	consisting of array name and element within array.
 *
 * Results:
 *	The return value points to the current value of the variable
 *	given by part1 and part2.  If the specified variable doesn't
 *	exist, or if there is a clash in array usage, then NULL is
 *	returned and a message will be left in interp->result if the
 *	TCL_LEAVE_ERR_MSG flag is set.  Note:  the return value is
 *	only valid up until the next call to Tcl_SetVar or Tcl_SetVar2;
 *	if you depend on the value lasting longer than that, then make
 *	yourself a private copy.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

unsigned char *
Tcl_GetVar2(interp, part1, part2, flags)
    Tcl_Interp *interp;		/* Command interpreter in which variable is
				 * to be looked up. */
    unsigned char *part1;	/* Name of array (if part2 is NULL) or
				 * name of variable. */
    unsigned char *part2;	/* If non-null, gives name of element in
				 * array. */
    int flags;			/* OR-ed combination of TCL_GLOBAL_ONLY
				 * or TCL_LEAVE_ERR_MSG bits. */
{
    Tcl_HashEntry *hPtr;
    Var *varPtr;
    Interp *iPtr = (Interp *) interp;
    Var *arrayPtr = 0;

    /*
     * Lookup the first name.
     */

    if ((flags & TCL_GLOBAL_ONLY) || (iPtr->varFramePtr == 0)) {
	hPtr = Tcl_FindHashEntry(&iPtr->globalTable, part1);
    } else {
	hPtr = Tcl_FindHashEntry(&iPtr->varFramePtr->varTable, part1);
    }
    if (hPtr == 0) {
	if (flags & TCL_LEAVE_ERR_MSG) {
	    VarErrMsg(interp, part1, part2, (unsigned char*) "read", noSuchVar);
	}
	return 0;
    }
    varPtr = (Var *) Tcl_GetHashValue(hPtr);
    if (varPtr->flags & VAR_UPVAR) {
	hPtr = varPtr->value.upvarPtr;
	varPtr = (Var *) Tcl_GetHashValue(hPtr);
    }

    /*
     * If this is an array reference, then remember the traces on the array
     * and lookup the element within the array.
     */
    if (part2 != 0) {
	if (varPtr->flags & VAR_UNDEFINED) {
	    if (flags & TCL_LEAVE_ERR_MSG) {
		VarErrMsg(interp, part1, part2, (unsigned char*) "read", noSuchVar);
	    }
	    return 0;
	} else if (!(varPtr->flags & VAR_ARRAY)) {
	    if (flags & TCL_LEAVE_ERR_MSG) {
		VarErrMsg(interp, part1, part2, (unsigned char*) "read", needArray);
	    }
	    return 0;
	}
	arrayPtr = varPtr;
	hPtr = Tcl_FindHashEntry(varPtr->value.tablePtr, part2);
	if (hPtr == 0) {
	    if (flags & TCL_LEAVE_ERR_MSG) {
		VarErrMsg(interp, part1, part2, (unsigned char*) "read", noSuchElement);
	    }
	    return 0;
	}
	varPtr = (Var *) Tcl_GetHashValue(hPtr);
    }

    /*
     * Invoke any traces that have been set for the variable.
     */

    if ((varPtr->tracePtr != 0)
	    || ((arrayPtr != 0) && (arrayPtr->tracePtr != 0))) {
	unsigned char *msg;

	msg = CallTraces(iPtr, arrayPtr, hPtr, part1, part2,
		(flags & TCL_GLOBAL_ONLY) | TCL_TRACE_READS);
	if (msg != 0) {
	    VarErrMsg(interp, part1, part2, (unsigned char*) "read", msg);
	    return 0;
	}

	/*
	 * Watch out!  The variable could have gotten re-allocated to
	 * a larger size.  Fortunately the hash table entry will still
	 * be around.
	 */

	varPtr = (Var *) Tcl_GetHashValue(hPtr);
    }
    if (varPtr->flags & (VAR_UNDEFINED|VAR_UPVAR|VAR_ARRAY)) {
	if (flags & TCL_LEAVE_ERR_MSG) {
	    VarErrMsg(interp, part1, part2, (unsigned char*) "read", noSuchVar);
	}
	return 0;
    }
    return varPtr->value.string;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SetVar --
 *
 *	Change the value of a variable.
 *
 * Results:
 *	Returns a pointer to the malloc'ed string holding the new
 *	value of the variable.  The caller should not modify this
 *	string.  If the write operation was disallowed then NULL
 *	is returned;  if the TCL_LEAVE_ERR_MSG flag is set, then
 *	an explanatory message will be left in interp->result.
 *
 * Side effects:
 *	If varName is defined as a local or global variable in interp,
 *	its value is changed to newValue.  If varName isn't currently
 *	defined, then a new global variable by that name is created.
 *
 *----------------------------------------------------------------------
 */

unsigned char *
Tcl_SetVar(interp, varName, newValue, flags)
    Tcl_Interp *interp;		/* Command interpreter in which varName is
				 * to be looked up. */
    unsigned char *varName;	/* Name of a variable in interp. */
    unsigned char *newValue;	/* New value for varName. */
    int flags;			/* Various flags that tell how to set value:
				 * any of TCL_GLOBAL_ONLY, TCL_APPEND_VALUE,
				 * TCL_LIST_ELEMENT, TCL_NO_SPACE, or
				 * TCL_LEAVE_ERR_MSG. */
{
    register unsigned char *p;

    /*
     * If varName refers to an array (it ends with a parenthesized
     * element name), then handle it specially.
     */

    for (p = varName; *p != '\0'; p++) {
	if (*p == '(') {
	    unsigned char *result;
	    unsigned char *open = p;

	    do {
		p++;
	    } while (*p != '\0');
	    p--;
	    if (*p != ')') {
		goto scalar;
	    }
	    *open = '\0';
	    *p = '\0';
	    result = Tcl_SetVar2(interp, varName, open+1, newValue, flags);
	    *open = '(';
	    *p = ')';
	    return result;
	}
    }

    scalar:
    return Tcl_SetVar2(interp, varName, 0, newValue, flags);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SetVar2 --
 *
 *	Given a two-part variable name, which may refer either to a
 *	scalar variable or an element of an array, change the value
 *	of the variable.  If the named scalar or array or element
 *	doesn't exist then create one.
 *
 * Results:
 *	Returns a pointer to the malloc'ed string holding the new
 *	value of the variable.  The caller should not modify this
 *	string.  If the write operation was disallowed because an
 *	array was expected but not found (or vice versa), then NULL
 *	is returned;  if the TCL_LEAVE_ERR_MSG flag is set, then
 *	an explanatory message will be left in interp->result.
 *
 * Side effects:
 *	The value of the given variable is set.  If either the array
 *	or the entry didn't exist then a new one is created.
 *
 *----------------------------------------------------------------------
 */

unsigned char *
Tcl_SetVar2(interp, part1, part2, newValue, flags)
    Tcl_Interp *interp;		/* Command interpreter in which variable is
				 * to be looked up. */
    unsigned char *part1;	/* If part2 is NULL, this is name of scalar
				 * variable.  Otherwise it is name of array. */
    unsigned char *part2;	/* Name of an element within array, or NULL. */
    unsigned char *newValue;	/* New value for variable. */
    int flags;			/* Various flags that tell how to set value:
				 * any of TCL_GLOBAL_ONLY, TCL_APPEND_VALUE,
				 * TCL_LIST_ELEMENT, and TCL_NO_SPACE, or
				 * TCL_LEAVE_ERR_MSG . */
{
    Tcl_HashEntry *hPtr;
    register Var *varPtr = 0;
				/* Initial value only used to stop compiler
				 * from complaining; not really needed. */
    register Interp *iPtr = (Interp *) interp;
    int length, new, listFlags;
    Var *arrayPtr = 0;

    /*
     * Lookup the first name.
     */

    if ((flags & TCL_GLOBAL_ONLY) || (iPtr->varFramePtr == 0)) {
	hPtr = Tcl_CreateHashEntry(&iPtr->globalTable, part1, &new);
    } else {
	hPtr = Tcl_CreateHashEntry(&iPtr->varFramePtr->varTable,
		part1, &new);
    }
    if (!new) {
	varPtr = (Var *) Tcl_GetHashValue(hPtr);
	if (varPtr->flags & VAR_UPVAR) {
	    hPtr = varPtr->value.upvarPtr;
	    varPtr = (Var *) Tcl_GetHashValue(hPtr);
	}
    }

    /*
     * If this is an array reference, then create a new array (if
     * needed), remember any traces on the array, and lookup the
     * element within the array.
     */

    if (part2 != 0) {
	if (new) {
	    varPtr = NewVar (0);
	    Tcl_SetHashValue(hPtr, varPtr);
	    varPtr->flags = VAR_ARRAY;
	    varPtr->value.tablePtr = (Tcl_HashTable*)
		    malloc (sizeof(Tcl_HashTable));
	    Tcl_InitHashTable (varPtr->value.tablePtr, TCL_STRING_KEYS);
	} else {
	    if (varPtr->flags & VAR_UNDEFINED) {
		varPtr->flags = VAR_ARRAY;
		varPtr->value.tablePtr = (Tcl_HashTable*)
			malloc (sizeof(Tcl_HashTable));
		Tcl_InitHashTable (varPtr->value.tablePtr, TCL_STRING_KEYS);
	    } else if (!(varPtr->flags & VAR_ARRAY)) {
		if (flags & TCL_LEAVE_ERR_MSG) {
		    VarErrMsg(interp, part1, part2, (unsigned char*) "set", needArray);
		}
		return 0;
	    }
	    arrayPtr = varPtr;
	}
	hPtr = Tcl_CreateHashEntry(varPtr->value.tablePtr, part2, &new);
    }

    /*
     * Compute how many bytes will be needed for newValue (leave space
     * for a separating space between list elements).
     */

    if (flags & TCL_LIST_ELEMENT) {
	length = Tcl_ScanElement(newValue, &listFlags) + 1;
    } else {
	length = strlen(newValue);
    }

    /*
     * If the variable doesn't exist then create a new one.  If it
     * does exist then clear its current value unless this is an
     * append operation.
     */

    if (new) {
	varPtr = NewVar (length);
	Tcl_SetHashValue(hPtr, varPtr);
	if ((arrayPtr != 0) && (arrayPtr->searchPtr != 0)) {
	    DeleteSearches(arrayPtr);
	}
    } else {
	varPtr = (Var *) Tcl_GetHashValue(hPtr);
	if (varPtr->flags & VAR_ARRAY) {
	    if (flags & TCL_LEAVE_ERR_MSG) {
		VarErrMsg(interp, part1, part2, (unsigned char*) "set", isArray);
	    }
	    return 0;
	}
	if (!(flags & TCL_APPEND_VALUE) || (varPtr->flags & VAR_UNDEFINED)) {
	    varPtr->valueLength = 0;
	}
    }

    /*
     * Make sure there's enough space to hold the variable's
     * new value.  If not, enlarge the variable's space.
     */

    if ((length + varPtr->valueLength) >= varPtr->valueSpace) {
	Var *newVarPtr;
	int newSize;

	newSize = 2*varPtr->valueSpace;
	if (newSize <= (length + varPtr->valueLength)) {
	    newSize += length;
	}
	newVarPtr = NewVar (newSize);
	newVarPtr->valueLength = varPtr->valueLength;
	newVarPtr->upvarUses = varPtr->upvarUses;
	newVarPtr->tracePtr = varPtr->tracePtr;
	newVarPtr->searchPtr = varPtr->searchPtr;
	newVarPtr->flags = varPtr->flags;
	strcpy(newVarPtr->value.string, varPtr->value.string);
	Tcl_SetHashValue(hPtr, newVarPtr);
	free (varPtr);
	varPtr = newVarPtr;
    }

    /*
     * Append the new value to the variable, either as a list
     * element or as a string.
     */

    if (flags & TCL_LIST_ELEMENT) {
	if ((varPtr->valueLength > 0) && !(flags & TCL_NO_SPACE)) {
	    varPtr->value.string[varPtr->valueLength] = ' ';
	    varPtr->valueLength++;
	}
	varPtr->valueLength += Tcl_ConvertElement(newValue,
		varPtr->value.string + varPtr->valueLength, listFlags);
	varPtr->value.string[varPtr->valueLength] = 0;
    } else {
	strcpy(varPtr->value.string + varPtr->valueLength, newValue);
	varPtr->valueLength += length;
    }
    varPtr->flags &= ~VAR_UNDEFINED;

    /*
     * Invoke any write traces for the variable.
     */

    if ((varPtr->tracePtr != 0)
	    || ((arrayPtr != 0) && (arrayPtr->tracePtr != 0))) {
	unsigned char *msg;

	msg = CallTraces(iPtr, arrayPtr, hPtr, part1, part2,
		(flags & TCL_GLOBAL_ONLY) | TCL_TRACE_WRITES);
	if (msg != 0) {
	    VarErrMsg(interp, part1, part2, (unsigned char*) "set", msg);
	    return 0;
	}

	/*
	 * Watch out!  The variable could have gotten re-allocated to
	 * a larger size.  Fortunately the hash table entry will still
	 * be around.
	 */

	varPtr = (Var *) Tcl_GetHashValue(hPtr);
    }
    return varPtr->value.string;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_UnsetVar --
 *
 *	Delete a variable, so that it may not be accessed anymore.
 *
 * Results:
 *	Returns 0 if the variable was successfully deleted, -1
 *	if the variable can't be unset.  In the event of an error,
 *	if the TCL_LEAVE_ERR_MSG flag is set then an error message
 *	is left in interp->result.
 *
 * Side effects:
 *	If varName is defined as a local or global variable in interp,
 *	it is deleted.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_UnsetVar(interp, varName, flags)
    Tcl_Interp *interp;		/* Command interpreter in which varName is
				 * to be looked up. */
    unsigned char *varName;	/* Name of a variable in interp.  May be
				 * either a scalar name or an array name
				 * or an element in an array. */
    int flags;			/* OR-ed combination of any of
				 * TCL_GLOBAL_ONLY or TCL_LEAVE_ERR_MSG. */
{
    register unsigned char *p;
    int result;

    /*
     * Figure out whether this is an array reference, then call
     * Tcl_UnsetVar2 to do all the real work.
     */

    for (p = varName; *p != '\0'; p++) {
	if (*p == '(') {
	    unsigned char *open = p;

	    do {
		p++;
	    } while (*p != '\0');
	    p--;
	    if (*p != ')') {
		goto scalar;
	    }
	    *open = '\0';
	    *p = '\0';
	    result = Tcl_UnsetVar2(interp, varName, open+1, flags);
	    *open = '(';
	    *p = ')';
	    return result;
	}
    }

    scalar:
    return Tcl_UnsetVar2(interp, varName, 0, flags);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_UnsetVar2 --
 *
 *	Delete a variable, given a 2-part name.
 *
 * Results:
 *	Returns 0 if the variable was successfully deleted, -1
 *	if the variable can't be unset.  In the event of an error,
 *	if the TCL_LEAVE_ERR_MSG flag is set then an error message
 *	is left in interp->result.
 *
 * Side effects:
 *	If part1 and part2 indicate a local or global variable in interp,
 *	it is deleted.  If part1 is an array name and part2 is NULL, then
 *	the whole array is deleted.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_UnsetVar2(interp, part1, part2, flags)
    Tcl_Interp *interp;		/* Command interpreter in which varName is
				 * to be looked up. */
    unsigned char *part1;	/* Name of variable or array. */
    unsigned char *part2;	/* Name of element within array or NULL. */
    int flags;			/* OR-ed combination of any of
				 * TCL_GLOBAL_ONLY or TCL_LEAVE_ERR_MSG. */
{
    Tcl_HashEntry *hPtr, dummyEntry;
    Var *varPtr, dummyVar;
    Interp *iPtr = (Interp *) interp;
    Var *arrayPtr = 0;

    if ((flags & TCL_GLOBAL_ONLY) || (iPtr->varFramePtr == 0)) {
	hPtr = Tcl_FindHashEntry(&iPtr->globalTable, part1);
    } else {
	hPtr = Tcl_FindHashEntry(&iPtr->varFramePtr->varTable, part1);
    }
    if (hPtr == 0) {
	if (flags & TCL_LEAVE_ERR_MSG) {
	    VarErrMsg(interp, part1, part2, (unsigned char*) "unset", noSuchVar);
	}
	return -1;
    }
    varPtr = (Var *) Tcl_GetHashValue(hPtr);

    /*
     * For global variables referenced in procedures, leave the procedure's
     * reference variable in place, but unset the global variable.  Can't
     * decrement the actual variable's use count, since we didn't delete
     * the reference variable.
     */

    if (varPtr->flags & VAR_UPVAR) {
	hPtr = varPtr->value.upvarPtr;
	varPtr = (Var *) Tcl_GetHashValue(hPtr);
    }

    /*
     * If the variable being deleted is an element of an array, then
     * remember trace procedures on the overall array and find the
     * element to delete.
     */

    if (part2 != 0) {
	if (!(varPtr->flags & VAR_ARRAY)) {
	    if (flags & TCL_LEAVE_ERR_MSG) {
		VarErrMsg(interp, part1, part2, (unsigned char*) "unset", needArray);
	    }
	    return -1;
	}
	if (varPtr->searchPtr != 0) {
	    DeleteSearches(varPtr);
	}
	arrayPtr = varPtr;
	hPtr = Tcl_FindHashEntry(varPtr->value.tablePtr, part2);
	if (hPtr == 0) {
	    if (flags & TCL_LEAVE_ERR_MSG) {
		VarErrMsg(interp, part1, part2, (unsigned char*) "unset", noSuchElement);
	    }
	    return -1;
	}
	varPtr = (Var *) Tcl_GetHashValue(hPtr);
    }

    /*
     * If there is a trace active on this variable or if the variable
     * is already being deleted then don't delete the variable:  it
     * isn't safe, since there are procedures higher up on the stack
     * that will use pointers to the variable.  Also don't delete an
     * array if there are traces active on any of its elements.
     */

    if (varPtr->flags &
	    (VAR_TRACE_ACTIVE|VAR_ELEMENT_ACTIVE)) {
	if (flags & TCL_LEAVE_ERR_MSG) {
	    VarErrMsg(interp, part1, part2, (unsigned char*) "unset", traceActive);
	}
	return -1;
    }

    /*
     * The code below is tricky, because of the possibility that
     * a trace procedure might try to access a variable being
     * deleted.  To handle this situation gracefully, copy the
     * contents of the variable and its hash table entry to
     * dummy variables, then clean up the actual variable so that
     * it's been completely deleted before the traces are called.
     * Then call the traces, and finally clean up the variable's
     * storage using the dummy copies.
     */

    dummyVar = *varPtr;
    Tcl_SetHashValue(&dummyEntry, &dummyVar);
    if (varPtr->upvarUses == 0) {
	Tcl_DeleteHashEntry(hPtr);
	free (varPtr);
    } else {
	varPtr->flags = VAR_UNDEFINED;
	varPtr->tracePtr = 0;
    }

    /*
     * Call trace procedures for the variable being deleted and delete
     * its traces.
     */

    if ((dummyVar.tracePtr != 0)
	    || ((arrayPtr != 0) && (arrayPtr->tracePtr != 0))) {
	(void) CallTraces(iPtr, arrayPtr, &dummyEntry, part1, part2,
		(flags & TCL_GLOBAL_ONLY) | TCL_TRACE_UNSETS);
	while (dummyVar.tracePtr != 0) {
	    VarTrace *tracePtr = dummyVar.tracePtr;
	    dummyVar.tracePtr = tracePtr->nextPtr;
	    free (tracePtr);
	}
    }

    /*
     * If the variable is an array, delete all of its elements.  This
     * must be done after calling the traces on the array, above (that's
     * the way traces are defined).
     */

    if (dummyVar.flags & VAR_ARRAY) {
	DeleteArray(iPtr, part1, &dummyVar,
	    (flags & TCL_GLOBAL_ONLY) | TCL_TRACE_UNSETS);
    }
    if (dummyVar.flags & VAR_UNDEFINED) {
	if (flags & TCL_LEAVE_ERR_MSG) {
	    VarErrMsg(interp, part1, part2, (unsigned char*) "unset",
		    (part2 == 0) ? noSuchVar : noSuchElement);
	}
	return -1;
    }
    return 0;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_TraceVar --
 *
 *	Arrange for reads and/or writes to a variable to cause a
 *	procedure to be invoked, which can monitor the operations
 *	and/or change their actions.
 *
 * Results:
 *	A standard Tcl return value.
 *
 * Side effects:
 *	A trace is set up on the variable given by varName, such that
 *	future references to the variable will be intermediated by
 *	proc.  See the manual entry for complete details on the calling
 *	sequence for proc.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_TraceVar(interp, varName, flags, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter in which variable is
				 * to be traced. */
    unsigned char *varName;	/* Name of variable;  may end with "(index)"
				 * to signify an array reference. */
    int flags;			/* OR-ed collection of bits, including any
				 * of TCL_TRACE_READS, TCL_TRACE_WRITES,
				 * TCL_TRACE_UNSETS, and TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure to call when specified ops are
				 * invoked upon varName. */
    void *clientData;		/* Arbitrary argument to pass to proc. */
{
    register unsigned char *p;

    /*
     * If varName refers to an array (it ends with a parenthesized
     * element name), then handle it specially.
     */

    for (p = varName; *p != '\0'; p++) {
	if (*p == '(') {
	    int result;
	    unsigned char *open = p;

	    do {
		p++;
	    } while (*p != '\0');
	    p--;
	    if (*p != ')') {
		goto scalar;
	    }
	    *open = '\0';
	    *p = '\0';
	    result = Tcl_TraceVar2(interp, varName, open+1, flags,
		    proc, clientData);
	    *open = '(';
	    *p = ')';
	    return result;
	}
    }

    scalar:
    return Tcl_TraceVar2(interp, varName, 0, flags, proc, clientData);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_TraceVar2 --
 *
 *	Arrange for reads and/or writes to a variable to cause a
 *	procedure to be invoked, which can monitor the operations
 *	and/or change their actions.
 *
 * Results:
 *	A standard Tcl return value.
 *
 * Side effects:
 *	A trace is set up on the variable given by part1 and part2, such
 *	that future references to the variable will be intermediated by
 *	proc.  See the manual entry for complete details on the calling
 *	sequence for proc.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_TraceVar2(interp, part1, part2, flags, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter in which variable is
				 * to be traced. */
    unsigned char *part1;	/* Name of scalar variable or array. */
    unsigned char *part2;	/* Name of element within array;  NULL means
				 * trace applies to scalar variable or array
				 * as-a-whole. */
    int flags;			/* OR-ed collection of bits, including any
				 * of TCL_TRACE_READS, TCL_TRACE_WRITES,
				 * TCL_TRACE_UNSETS, and TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure to call when specified ops are
				 * invoked upon varName. */
    void *clientData;		/* Arbitrary argument to pass to proc. */
{
    Tcl_HashEntry *hPtr;
    Var *varPtr = 0;		/* Initial value only used to stop compiler
				 * from complaining; not really needed. */
    Interp *iPtr = (Interp *) interp;
    register VarTrace *tracePtr;
    int new;

    /*
     * Locate the variable, making a new (undefined) one if necessary.
     */

    if ((flags & TCL_GLOBAL_ONLY) || (iPtr->varFramePtr == 0)) {
	hPtr = Tcl_CreateHashEntry(&iPtr->globalTable, part1, &new);
    } else {
	hPtr = Tcl_CreateHashEntry(&iPtr->varFramePtr->varTable, part1, &new);
    }
    if (!new) {
	varPtr = (Var *) Tcl_GetHashValue(hPtr);
	if (varPtr->flags & VAR_UPVAR) {
	    hPtr = varPtr->value.upvarPtr;
	    varPtr = (Var *) Tcl_GetHashValue(hPtr);
	}
    }

    /*
     * If the trace is to be on an array element, make sure that the
     * variable is an array variable.  If the variable doesn't exist
     * then define it as an empty array.  Then find the specific
     * array element.
     */

    if (part2 != 0) {
	if (new) {
	    varPtr = NewVar (0);
	    Tcl_SetHashValue(hPtr, varPtr);
	    varPtr->flags = VAR_ARRAY;
	    varPtr->value.tablePtr = (Tcl_HashTable*)
		malloc (sizeof(Tcl_HashTable));
	    Tcl_InitHashTable (varPtr->value.tablePtr, TCL_STRING_KEYS);
	} else {
	    if (varPtr->flags & VAR_UNDEFINED) {
		varPtr->flags = VAR_ARRAY;
		varPtr->value.tablePtr = (Tcl_HashTable*)
		    malloc (sizeof(Tcl_HashTable));
		Tcl_InitHashTable (varPtr->value.tablePtr, TCL_STRING_KEYS);
	    } else if (!(varPtr->flags & VAR_ARRAY)) {
		iPtr->result = needArray;
		return TCL_ERROR;
	    }
	}
	hPtr = Tcl_CreateHashEntry(varPtr->value.tablePtr, part2, &new);
    }

    if (new) {
	if ((part2 != 0) && (varPtr->searchPtr != 0)) {
	    DeleteSearches(varPtr);
	}
	varPtr = NewVar (0);
	varPtr->flags = VAR_UNDEFINED;
	Tcl_SetHashValue(hPtr, varPtr);
    } else {
	varPtr = (Var *) Tcl_GetHashValue(hPtr);
    }

    /*
     * Set up trace information.
     */

    tracePtr = (VarTrace*) malloc (sizeof(VarTrace));
    tracePtr->traceProc = proc;
    tracePtr->clientData = clientData;
    tracePtr->flags = flags &
	    (TCL_TRACE_READS|TCL_TRACE_WRITES|TCL_TRACE_UNSETS);
    tracePtr->nextPtr = varPtr->tracePtr;
    varPtr->tracePtr = tracePtr;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_UntraceVar --
 *
 *	Remove a previously-created trace for a variable.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	If there exists a trace for the variable given by varName
 *	with the given flags, proc, and clientData, then that trace
 *	is removed.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_UntraceVar(interp, varName, flags, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter containing traced variable. */
    unsigned char *varName;	/* Name of variable;  may end with "(index)"
				 * to signify an array reference. */
    int flags;			/* OR-ed collection of bits describing
				 * current trace, including any of
				 * TCL_TRACE_READS, TCL_TRACE_WRITES,
				 * TCL_TRACE_UNSETS, and TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure assocated with trace. */
    void *clientData;		/* Arbitrary argument to pass to proc. */
{
    register unsigned char *p;

    /*
     * If varName refers to an array (it ends with a parenthesized
     * element name), then handle it specially.
     */

    for (p = varName; *p != '\0'; p++) {
	if (*p == '(') {
	    unsigned char *open = p;

	    do {
		p++;
	    } while (*p != '\0');
	    p--;
	    if (*p != ')') {
		goto scalar;
	    }
	    *open = '\0';
	    *p = '\0';
	    Tcl_UntraceVar2(interp, varName, open+1, flags, proc, clientData);
	    *open = '(';
	    *p = ')';
	    return;
	}
    }

    scalar:
    Tcl_UntraceVar2(interp, varName, 0, flags, proc, clientData);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_UntraceVar2 --
 *
 *	Remove a previously-created trace for a variable.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	If there exists a trace for the variable given by part1
 *	and part2 with the given flags, proc, and clientData, then
 *	that trace is removed.
 *
 *----------------------------------------------------------------------
 */

void
Tcl_UntraceVar2(interp, part1, part2, flags, proc, clientData)
    Tcl_Interp *interp;		/* Interpreter containing traced variable. */
    unsigned char *part1;	/* Name of variable or array. */
    unsigned char *part2;	/* Name of element within array;  NULL means
				 * trace applies to scalar variable or array
				 * as-a-whole. */
    int flags;			/* OR-ed collection of bits describing
				 * current trace, including any of
				 * TCL_TRACE_READS, TCL_TRACE_WRITES,
				 * TCL_TRACE_UNSETS, and TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure assocated with trace. */
    void *clientData;		/* Arbitrary argument to pass to proc. */
{
    register VarTrace *tracePtr;
    VarTrace *prevPtr;
    Var *varPtr;
    Interp *iPtr = (Interp *) interp;
    Tcl_HashEntry *hPtr;
    ActiveVarTrace *activePtr;

    /*
     * First, lookup the variable.
     */

    if ((flags & TCL_GLOBAL_ONLY) || (iPtr->varFramePtr == 0)) {
	hPtr = Tcl_FindHashEntry(&iPtr->globalTable, part1);
    } else {
	hPtr = Tcl_FindHashEntry(&iPtr->varFramePtr->varTable, part1);
    }
    if (hPtr == 0) {
	return;
    }
    varPtr = (Var *) Tcl_GetHashValue(hPtr);
    if (varPtr->flags & VAR_UPVAR) {
	hPtr = varPtr->value.upvarPtr;
	varPtr = (Var *) Tcl_GetHashValue(hPtr);
    }
    if (part2 != 0) {
	if (!(varPtr->flags & VAR_ARRAY)) {
	    return;
	}
	hPtr = Tcl_FindHashEntry(varPtr->value.tablePtr, part2);
	if (hPtr == 0) {
	    return;
	}
	varPtr = (Var *) Tcl_GetHashValue(hPtr);
    }

    flags &= (TCL_TRACE_READS | TCL_TRACE_WRITES | TCL_TRACE_UNSETS);
    for (tracePtr = varPtr->tracePtr, prevPtr = 0; ;
	    prevPtr = tracePtr, tracePtr = tracePtr->nextPtr) {
	if (tracePtr == 0) {
	    return;
	}
	if ((tracePtr->traceProc == proc) && (tracePtr->flags == flags)
		&& (tracePtr->clientData == clientData)) {
	    break;
	}
    }

    /*
     * The code below makes it possible to delete traces while traces
     * are active:  it makes sure that the deleted trace won't be
     * processed by CallTraces.
     */

    for (activePtr = iPtr->activeTracePtr; activePtr != 0;
	    activePtr = activePtr->nextPtr) {
	if (activePtr->nextTracePtr == tracePtr) {
	    activePtr->nextTracePtr = tracePtr->nextPtr;
	}
    }
    if (prevPtr == 0) {
	varPtr->tracePtr = tracePtr->nextPtr;
    } else {
	prevPtr->nextPtr = tracePtr->nextPtr;
    }
    free (tracePtr);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_VarTraceInfo --
 *
 *	Return the clientData value associated with a trace on a
 *	variable.  This procedure can also be used to step through
 *	all of the traces on a particular variable that have the
 *	same trace procedure.
 *
 * Results:
 *	The return value is the clientData value associated with
 *	a trace on the given variable.  Information will only be
 *	returned for a trace with proc as trace procedure.  If
 *	the clientData argument is NULL then the first such trace is
 *	returned;  otherwise, the next relevant one after the one
 *	given by clientData will be returned.  If the variable
 *	doesn't exist, or if there are no (more) traces for it,
 *	then NULL is returned.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

void *
Tcl_VarTraceInfo(interp, varName, flags, proc, prevClientData)
    Tcl_Interp *interp;		/* Interpreter containing variable. */
    unsigned char *varName;	/* Name of variable;  may end with "(index)"
				 * to signify an array reference. */
    int flags;			/* 0 or TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure assocated with trace. */
    void *prevClientData;	/* If non-NULL, gives last value returned
				 * by this procedure, so this call will
				 * return the next trace after that one.
				 * If NULL, this call will return the
				 * first trace. */
{
    register unsigned char *p;

    /*
     * If varName refers to an array (it ends with a parenthesized
     * element name), then handle it specially.
     */

    for (p = varName; *p != '\0'; p++) {
	if (*p == '(') {
	    void *result;
	    unsigned char *open = p;

	    do {
		p++;
	    } while (*p != '\0');
	    p--;
	    if (*p != ')') {
		goto scalar;
	    }
	    *open = '\0';
	    *p = '\0';
	    result = Tcl_VarTraceInfo2(interp, varName, open+1, flags, proc,
		prevClientData);
	    *open = '(';
	    *p = ')';
	    return result;
	}
    }

    scalar:
    return Tcl_VarTraceInfo2(interp, varName, 0, flags, proc, prevClientData);
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_VarTraceInfo2 --
 *
 *	Same as Tcl_VarTraceInfo, except takes name in two pieces
 *	instead of one.
 *
 * Results:
 *	Same as Tcl_VarTraceInfo.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

void *
Tcl_VarTraceInfo2(interp, part1, part2, flags, proc, prevClientData)
    Tcl_Interp *interp;		/* Interpreter containing variable. */
    unsigned char *part1;	/* Name of variable or array. */
    unsigned char *part2;	/* Name of element within array;  NULL means
				 * trace applies to scalar variable or array
				 * as-a-whole. */
    int flags;			/* 0 or TCL_GLOBAL_ONLY. */
    Tcl_VarTraceProc *proc;	/* Procedure assocated with trace. */
    void *prevClientData;	/* If non-NULL, gives last value returned
				 * by this procedure, so this call will
				 * return the next trace after that one.
				 * If NULL, this call will return the
				 * first trace. */
{
    register VarTrace *tracePtr;
    Var *varPtr;
    Interp *iPtr = (Interp *) interp;
    Tcl_HashEntry *hPtr;

    /*
     * First, lookup the variable.
     */
    if ((flags & TCL_GLOBAL_ONLY) || (iPtr->varFramePtr == 0)) {
	hPtr = Tcl_FindHashEntry(&iPtr->globalTable, part1);
    } else {
	hPtr = Tcl_FindHashEntry(&iPtr->varFramePtr->varTable, part1);
    }
    if (hPtr == 0) {
	return 0;
    }
    varPtr = (Var *) Tcl_GetHashValue(hPtr);
    if (varPtr->flags & VAR_UPVAR) {
	hPtr = varPtr->value.upvarPtr;
	varPtr = (Var *) Tcl_GetHashValue(hPtr);
    }
    if (part2 != 0) {
	if (!(varPtr->flags & VAR_ARRAY)) {
	    return 0;
	}
	hPtr = Tcl_FindHashEntry(varPtr->value.tablePtr, part2);
	if (hPtr == 0) {
	    return 0;
	}
	varPtr = (Var *) Tcl_GetHashValue(hPtr);
    }

    /*
     * Find the relevant trace, if any, and return its clientData.
     */

    tracePtr = varPtr->tracePtr;
    if (prevClientData != 0) {
	for ( ; tracePtr != 0; tracePtr = tracePtr->nextPtr) {
	    if ((tracePtr->clientData == prevClientData)
		    && (tracePtr->traceProc == proc)) {
		tracePtr = tracePtr->nextPtr;
		break;
	    }
	}
    }
    for ( ; tracePtr != 0; tracePtr = tracePtr->nextPtr) {
	if (tracePtr->traceProc == proc) {
	    return tracePtr->clientData;
	}
    }
    return 0;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_SetCmd --
 *
 *	This procedure is invoked to process the "set" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	A variable's value may be changed.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_SetCmd(dummy, interp, argc, argv)
    void *dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    unsigned char **argv;		/* Argument strings. */
{
    if (argc == 2) {
	unsigned char *value;

	value = Tcl_GetVar(interp, argv[1], TCL_LEAVE_ERR_MSG);
	if (value == 0) {
	    return TCL_ERROR;
	}
	interp->result = value;
	return TCL_OK;
    } else if (argc == 3) {
	unsigned char *result;

	result = Tcl_SetVar(interp, argv[1], argv[2], TCL_LEAVE_ERR_MSG);
	if (result == 0) {
	    return TCL_ERROR;
	}
	interp->result = result;
	return TCL_OK;
    } else {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
		argv[0], " varName ?newValue?\"", 0);
	return TCL_ERROR;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_UnsetCmd --
 *
 *	This procedure is invoked to process the "unset" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_UnsetCmd(dummy, interp, argc, argv)
    void *dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    unsigned char **argv;		/* Argument strings. */
{
    int i;

    if (argc < 2) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
		argv[0], " varName ?varName ...?\"", 0);
	return TCL_ERROR;
    }
    for (i = 1; i < argc; i++) {
	if (Tcl_UnsetVar(interp, argv[i], TCL_LEAVE_ERR_MSG) != 0) {
	    return TCL_ERROR;
	}
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_AppendCmd --
 *
 *	This procedure is invoked to process the "append" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	A variable's value may be changed.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_AppendCmd(dummy, interp, argc, argv)
    void *dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    unsigned char **argv;		/* Argument strings. */
{
    int i;
    unsigned char *result = 0;		/* (Initialization only needed to keep
					 * the compiler from complaining) */

    if (argc < 3) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
		argv[0], " varName value ?value ...?\"", 0);
	return TCL_ERROR;
    }

    for (i = 2; i < argc; i++) {
	result = Tcl_SetVar(interp, argv[1], argv[i],
		TCL_APPEND_VALUE|TCL_LEAVE_ERR_MSG);
	if (result == 0) {
	    return TCL_ERROR;
	}
    }
    interp->result = result;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_LappendCmd --
 *
 *	This procedure is invoked to process the "lappend" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	A variable's value may be changed.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_LappendCmd(dummy, interp, argc, argv)
    void *dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    unsigned char **argv;		/* Argument strings. */
{
    int i;
    unsigned char *result = 0;		/* (Initialization only needed to keep
					 * the compiler from complaining) */

    if (argc < 3) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
		argv[0], " varName value ?value ...?\"", 0);
	return TCL_ERROR;
    }

    for (i = 2; i < argc; i++) {
	result = Tcl_SetVar(interp, argv[1], argv[i],
		TCL_APPEND_VALUE|TCL_LIST_ELEMENT|TCL_LEAVE_ERR_MSG);
	if (result == 0) {
	    return TCL_ERROR;
	}
    }
    interp->result = result;
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_ArrayCmd --
 *
 *	This procedure is invoked to process the "array" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_ArrayCmd(dummy, interp, argc, argv)
    void *dummy;			/* Not used. */
    register Tcl_Interp *interp;	/* Current interpreter. */
    int argc;				/* Number of arguments. */
    unsigned char **argv;		/* Argument strings. */
{
    int length;
    char c;
    Var *varPtr;
    Tcl_HashEntry *hPtr;
    Interp *iPtr = (Interp *) interp;

    if (argc < 3) {
	Tcl_AppendResult(interp, "wrong # args: should be \"",
		argv[0], " option arrayName ?arg ...?\"", 0);
	return TCL_ERROR;
    }

    /*
     * Locate the array variable (and it better be an array).
     */

    if (iPtr->varFramePtr == 0) {
	hPtr = Tcl_FindHashEntry(&iPtr->globalTable, argv[2]);
    } else {
	hPtr = Tcl_FindHashEntry(&iPtr->varFramePtr->varTable, argv[2]);
    }
    if (hPtr == 0) {
	notArray:
	Tcl_AppendResult(interp, "\"", argv[2], "\" isn't an array", 0);
	return TCL_ERROR;
    }
    varPtr = (Var *) Tcl_GetHashValue(hPtr);
    if (varPtr->flags & VAR_UPVAR) {
	varPtr = (Var *) Tcl_GetHashValue(varPtr->value.upvarPtr);
    }
    if (!(varPtr->flags & VAR_ARRAY)) {
	goto notArray;
    }

    /*
     * Dispatch based on the option.
     */

    c = argv[1][0];
    length = strlen(argv[1]);
    if ((c == 'a') && (strncmp(argv[1], (unsigned char*) "anymore", length) == 0)) {
	ArraySearch *searchPtr;

	if (argc != 4) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " anymore arrayName searchId\"", 0);
	    return TCL_ERROR;
	}
	searchPtr = ParseSearchId(interp, varPtr, argv[2], argv[3]);
	if (searchPtr == 0) {
	    return TCL_ERROR;
	}
	while (1) {
	    Var *varPtr2;

	    if (searchPtr->nextEntry != 0) {
		varPtr2 = (Var *) Tcl_GetHashValue(searchPtr->nextEntry);
		if (!(varPtr2->flags & VAR_UNDEFINED)) {
		    break;
		}
	    }
	    searchPtr->nextEntry = Tcl_NextHashEntry(&searchPtr->search);
	    if (searchPtr->nextEntry == 0) {
		interp->result = (unsigned char*) "0";
		return TCL_OK;
	    }
	}
	interp->result = (unsigned char*) "1";
	return TCL_OK;
    } else if ((c == 'd') && (strncmp(argv[1], (unsigned char*) "donesearch", length) == 0)) {
	ArraySearch *searchPtr, *prevPtr;

	if (argc != 4) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " donesearch arrayName searchId\"", 0);
	    return TCL_ERROR;
	}
	searchPtr = ParseSearchId(interp, varPtr, argv[2], argv[3]);
	if (searchPtr == 0) {
	    return TCL_ERROR;
	}
	if (varPtr->searchPtr == searchPtr) {
	    varPtr->searchPtr = searchPtr->nextPtr;
	} else {
	    for (prevPtr = varPtr->searchPtr; ; prevPtr = prevPtr->nextPtr) {
		if (prevPtr->nextPtr == searchPtr) {
		    prevPtr->nextPtr = searchPtr->nextPtr;
		    break;
		}
	    }
	}
	free (searchPtr);
    } else if ((c == 'n') && (strncmp(argv[1], (unsigned char*) "names", length) == 0)
	    && (length >= 2)) {
	Tcl_HashSearch search;
	Var *varPtr2;

	if (argc != 3) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " names arrayName\"", 0);
	    return TCL_ERROR;
	}
	for (hPtr = Tcl_FirstHashEntry(varPtr->value.tablePtr, &search);
		hPtr != 0; hPtr = Tcl_NextHashEntry(&search)) {
	    varPtr2 = (Var *) Tcl_GetHashValue(hPtr);
	    if (varPtr2->flags & VAR_UNDEFINED) {
		continue;
	    }
	    Tcl_AppendElement(interp,
		    Tcl_GetHashKey(varPtr->value.tablePtr, hPtr), 0);
	}
    } else if ((c == 'n') && (strncmp(argv[1], (unsigned char*) "nextelement", length) == 0)
	    && (length >= 2)) {
	ArraySearch *searchPtr;
	Tcl_HashEntry *hPtr;

	if (argc != 4) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " nextelement arrayName searchId\"", 0);
	    return TCL_ERROR;
	}
	searchPtr = ParseSearchId(interp, varPtr, argv[2], argv[3]);
	if (searchPtr == 0) {
	    return TCL_ERROR;
	}
	while (1) {
	    Var *varPtr2;

	    hPtr = searchPtr->nextEntry;
	    if (hPtr == 0) {
		hPtr = Tcl_NextHashEntry(&searchPtr->search);
		if (hPtr == 0) {
		    return TCL_OK;
		}
	    } else {
		searchPtr->nextEntry = 0;
	    }
	    varPtr2 = (Var *) Tcl_GetHashValue(hPtr);
	    if (!(varPtr2->flags & VAR_UNDEFINED)) {
		break;
	    }
	}
	interp->result = Tcl_GetHashKey(varPtr->value.tablePtr, hPtr);
    } else if ((c == 's') && (strncmp(argv[1], (unsigned char*) "size", length) == 0)
	    && (length >= 2)) {
	Tcl_HashSearch search;
	Var *varPtr2;
	int size;

	if (argc != 3) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " size arrayName\"", 0);
	    return TCL_ERROR;
	}
	size = 0;
	for (hPtr = Tcl_FirstHashEntry(varPtr->value.tablePtr, &search);
		hPtr != 0; hPtr = Tcl_NextHashEntry(&search)) {
	    varPtr2 = (Var *) Tcl_GetHashValue(hPtr);
	    if (varPtr2->flags & VAR_UNDEFINED) {
		continue;
	    }
	    size++;
	}
	sprintf(interp->result, "%d", size);
    } else if ((c == 's') && (strncmp(argv[1], (unsigned char*) "startsearch", length) == 0)
	    && (length >= 2)) {
	ArraySearch *searchPtr;

	if (argc != 3) {
	    Tcl_AppendResult(interp, "wrong # args: should be \"",
		    argv[0], " startsearch arrayName\"", 0);
	    return TCL_ERROR;
	}
	searchPtr = (ArraySearch*) malloc (sizeof(ArraySearch));
	if (varPtr->searchPtr == 0) {
	    searchPtr->id = 1;
	    Tcl_AppendResult(interp, "s-1-", argv[2], 0);
	} else {
	    unsigned char string[20];

	    searchPtr->id = varPtr->searchPtr->id + 1;
	    sprintf(string, "%u", searchPtr->id);
	    Tcl_AppendResult(interp, "s-", string, "-", argv[2], 0);
	}
	searchPtr->varPtr = varPtr;
	searchPtr->nextEntry = Tcl_FirstHashEntry(varPtr->value.tablePtr,
		&searchPtr->search);
	searchPtr->nextPtr = varPtr->searchPtr;
	varPtr->searchPtr = searchPtr;
    } else {
	Tcl_AppendResult(interp, "bad option \"", argv[1],
		"\": should be anymore, donesearch, names, nextelement, ",
		"size, or startsearch", 0);
	return TCL_ERROR;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_GlobalCmd --
 *
 *	This procedure is invoked to process the "global" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_GlobalCmd(dummy, interp, argc, argv)
    void *dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    unsigned char **argv;		/* Argument strings. */
{
    Var *varPtr, *gVarPtr;
    register Interp *iPtr = (Interp *) interp;
    Tcl_HashEntry *hPtr, *hPtr2;
    int new;

    if (argc < 2) {
	Tcl_AppendResult((Tcl_Interp *) iPtr, "wrong # args: should be \"",
		argv[0], " varName ?varName ...?\"", 0);
	return TCL_ERROR;
    }
    if (iPtr->varFramePtr == 0) {
	return TCL_OK;
    }

    for (argc--, argv++; argc > 0; argc--, argv++) {
	hPtr = Tcl_CreateHashEntry(&iPtr->globalTable, *argv, &new);
	if (new) {
	    gVarPtr = NewVar (0);
	    gVarPtr->flags |= VAR_UNDEFINED;
	    Tcl_SetHashValue(hPtr, gVarPtr);
	} else {
	    gVarPtr = (Var *) Tcl_GetHashValue(hPtr);
	}
	hPtr2 = Tcl_CreateHashEntry(&iPtr->varFramePtr->varTable, *argv, &new);
	if (!new) {
	    Var *varPtr;
	    varPtr = (Var *) Tcl_GetHashValue(hPtr2);
	    if (varPtr->flags & VAR_UPVAR) {
		continue;
	    } else {
		Tcl_AppendResult((Tcl_Interp *) iPtr, "variable \"", *argv,
		    "\" already exists", 0);
		return TCL_ERROR;
	    }
	}
	varPtr = NewVar (0);
	varPtr->flags |= VAR_UPVAR;
	varPtr->value.upvarPtr = hPtr;
	gVarPtr->upvarUses++;
	Tcl_SetHashValue(hPtr2, varPtr);
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_UpvarCmd --
 *
 *	This procedure is invoked to process the "upvar" Tcl command.
 *	See the user documentation for details on what it does.
 *
 * Results:
 *	A standard Tcl result value.
 *
 * Side effects:
 *	See the user documentation.
 *
 *----------------------------------------------------------------------
 */

	/* ARGSUSED */
int
Tcl_UpvarCmd(dummy, interp, argc, argv)
    void *dummy;			/* Not used. */
    Tcl_Interp *interp;			/* Current interpreter. */
    int argc;				/* Number of arguments. */
    unsigned char **argv;		/* Argument strings. */
{
    register Interp *iPtr = (Interp *) interp;
    int result;
    CallFrame *framePtr;
    Var *varPtr = 0;
    Tcl_HashTable *upVarTablePtr;
    Tcl_HashEntry *hPtr, *hPtr2;
    int new;
    Var *upVarPtr;

    if (argc < 3) {
	upvarSyntax:
	Tcl_AppendResult(interp, "wrong # args: should be \"", argv[0],
		" ?level? otherVar localVar ?otherVar localVar ...?\"", 0);
	return TCL_ERROR;
    }

    /*
     * Find the hash table containing the variable being referenced.
     */

    result = TclGetFrame(interp, argv[1], &framePtr);
    if (result == -1) {
	return TCL_ERROR;
    }
    argc -= result+1;
    argv += result+1;
    if (framePtr == 0) {
	upVarTablePtr = &iPtr->globalTable;
    } else {
	upVarTablePtr = &framePtr->varTable;
    }

    if ((argc & 1) != 0) {
	goto upvarSyntax;
    }

    /*
     * Iterate over all the pairs of (local variable, other variable)
     * names.  For each pair, create a hash table entry in the upper
     * context (if the name wasn't there already), then associate it
     * with a new local variable.
     */

    while (argc > 0) {
        hPtr = Tcl_CreateHashEntry(upVarTablePtr, argv[0], &new);
        if (new) {
            upVarPtr = NewVar (0);
            upVarPtr->flags |= VAR_UNDEFINED;
            Tcl_SetHashValue(hPtr, upVarPtr);
        } else {
            upVarPtr = (Var *) Tcl_GetHashValue(hPtr);
	    if (upVarPtr->flags & VAR_UPVAR) {
		hPtr = upVarPtr->value.upvarPtr;
		upVarPtr = (Var *) Tcl_GetHashValue(hPtr);
	    }
        }

        hPtr2 = Tcl_CreateHashEntry(&iPtr->varFramePtr->varTable,
                    argv[1], &new);
        if (!new) {
            Tcl_AppendResult((Tcl_Interp *) iPtr, "variable \"", argv[1],
                "\" already exists", 0);
            return TCL_ERROR;
        }
        varPtr = NewVar (0);
        varPtr->flags |= VAR_UPVAR;
        varPtr->value.upvarPtr = hPtr;
        upVarPtr->upvarUses++;
        Tcl_SetHashValue(hPtr2, varPtr);

        argc -= 2;
        argv += 2;
    }
    return TCL_OK;
}

/*
 *----------------------------------------------------------------------
 *
 * TclDeleteVars --
 *
 *	This procedure is called to recycle all the storage space
 *	associated with a table of variables.  For this procedure
 *	to work correctly, it must not be possible for any of the
 *	variable in the table to be accessed from Tcl commands
 *	(e.g. from trace procedures).
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Variables are deleted and trace procedures are invoked, if
 *	any are declared.
 *
 *----------------------------------------------------------------------
 */

void
TclDeleteVars(iPtr, tablePtr)
    Interp *iPtr;		/* Interpreter to which variables belong. */
    Tcl_HashTable *tablePtr;	/* Hash table containing variables to
				 * delete. */
{
    Tcl_HashSearch search;
    Tcl_HashEntry *hPtr;
    register Var *varPtr;
    int flags, globalFlag;

    flags = TCL_TRACE_UNSETS;
    if (tablePtr == &iPtr->globalTable) {
	flags |= TCL_INTERP_DESTROYED | TCL_GLOBAL_ONLY;
    }
    for (hPtr = Tcl_FirstHashEntry(tablePtr, &search); hPtr != 0;
	    hPtr = Tcl_NextHashEntry(&search)) {
	varPtr = (Var *) Tcl_GetHashValue(hPtr);

	/*
	 * For global/upvar variables referenced in procedures, free up the
	 * local space and then decrement the reference count on the
	 * variable referred to.  If there are no more references to the
	 * global/upvar and it is undefined and has no traces set, then
	 * follow on and delete the referenced variable too.
	 */

	globalFlag = 0;
	if (varPtr->flags & VAR_UPVAR) {
	    hPtr = varPtr->value.upvarPtr;
	    free (varPtr);
	    varPtr = (Var *) Tcl_GetHashValue(hPtr);
	    varPtr->upvarUses--;
	    if ((varPtr->upvarUses != 0) || !(varPtr->flags & VAR_UNDEFINED)
		    || (varPtr->tracePtr != 0)) {
		continue;
	    }
	    globalFlag = TCL_GLOBAL_ONLY;
	}

	/*
	 * Invoke traces on the variable that is being deleted, then
	 * free up the variable's space (no need to free the hash entry
	 * here, unless we're dealing with a global variable:  the
	 * hash entries will be deleted automatically when the whole
	 * table is deleted).
	 */

	if (varPtr->tracePtr != 0) {
	    (void) CallTraces(iPtr, (Var *) 0, hPtr,
		    Tcl_GetHashKey(tablePtr, hPtr), 0,
		    flags | globalFlag);
	    while (varPtr->tracePtr != 0) {
		VarTrace *tracePtr = varPtr->tracePtr;
		varPtr->tracePtr = tracePtr->nextPtr;
		free (tracePtr);
	    }
	}
	if (varPtr->flags & VAR_ARRAY) {
	    DeleteArray(iPtr, Tcl_GetHashKey(tablePtr, hPtr), varPtr,
		    flags | globalFlag);
	}
	if (globalFlag) {
	    Tcl_DeleteHashEntry(hPtr);
	}
	free (varPtr);
    }
    Tcl_DeleteHashTable(tablePtr);
}

/*
 *----------------------------------------------------------------------
 *
 * CallTraces --
 *
 *	This procedure is invoked to find and invoke relevant
 *	trace procedures associated with a particular operation on
 *	a variable.  This procedure invokes traces both on the
 *	variable and on its containing array (where relevant).
 *
 * Results:
 *	The return value is 0 if no trace procedures were invoked, or
 *	if all the invoked trace procedures returned successfully.
 *	The return value is non-zero if a trace procedure returned an
 *	error (in this case no more trace procedures were invoked after
 *	the error was returned).  In this case the return value is a
 *	pointer to a static string describing the error.
 *
 * Side effects:
 *	Almost anything can happen, depending on trace;  this procedure
 *	itself doesn't have any side effects.
 *
 *----------------------------------------------------------------------
 */

static unsigned char *
CallTraces (iPtr, arrayPtr, hPtr, part1, part2, flags)
    Interp *iPtr;			/* Interpreter containing variable. */
    register Var *arrayPtr;		/* Pointer to array variable that
					 * contains the variable, or 0 if
					 * the variable isn't an element of an
					 * array. */
    Tcl_HashEntry *hPtr;		/* Hash table entry corresponding to
					 * variable whose traces are to be
					 * invoked. */
    unsigned char *part1, *part2;	/* Variable's two-part name. */
    int flags;				/* Flags to pass to trace procedures:
					 * indicates what's happening to
					 * variable, plus other stuff like
					 * TCL_GLOBAL_ONLY and
					 * TCL_INTERP_DESTROYED. */
{
    Var *varPtr;
    register VarTrace *tracePtr;
    ActiveVarTrace active;
    unsigned char *result;
    int savedArrayFlags = 0;		/* (Initialization not needed except
					 * to prevent compiler warning) */

    /*
     * If there are already similar trace procedures active for the
     * variable, don't call them again.
     */

    varPtr = (Var *) Tcl_GetHashValue(hPtr);
    if (varPtr->flags & VAR_TRACE_ACTIVE) {
	return 0;
    }
    varPtr->flags |= VAR_TRACE_ACTIVE;

    /*
     * Invoke traces on the array containing the variable, if relevant.
     */

    result = 0;
    active.nextPtr = iPtr->activeTracePtr;
    iPtr->activeTracePtr = &active;
    if (arrayPtr != 0) {
	savedArrayFlags = arrayPtr->flags;
	arrayPtr->flags |= VAR_ELEMENT_ACTIVE;
	for (tracePtr = arrayPtr->tracePtr;  tracePtr != 0;
		tracePtr = active.nextTracePtr) {
	    active.nextTracePtr = tracePtr->nextPtr;
	    if (!(tracePtr->flags & flags)) {
		continue;
	    }
	    result = (*tracePtr->traceProc)(tracePtr->clientData,
		    (Tcl_Interp *) iPtr, part1, part2, flags);
	    if (result != 0) {
		if (flags & TCL_TRACE_UNSETS) {
		    result = 0;
		} else {
		    goto done;
		}
	    }
	}
    }

    /*
     * Invoke traces on the variable itself.
     */

    if (flags & TCL_TRACE_UNSETS) {
	flags |= TCL_TRACE_DESTROYED;
    }
    for (tracePtr = varPtr->tracePtr; tracePtr != 0;
	    tracePtr = active.nextTracePtr) {
	active.nextTracePtr = tracePtr->nextPtr;
	if (!(tracePtr->flags & flags)) {
	    continue;
	}
	result = (*tracePtr->traceProc)(tracePtr->clientData,
		(Tcl_Interp *) iPtr, part1, part2, flags);
	if (result != 0) {
	    if (flags & TCL_TRACE_UNSETS) {
		result = 0;
	    } else {
		goto done;
	    }
	}
    }

    /*
     * Restore the variable's flags, remove the record of our active
     * traces, and then return.  Remember that the variable could have
     * been re-allocated during the traces, but its hash entry won't
     * change.
     */

    done:
    if (arrayPtr != 0) {
	arrayPtr->flags = savedArrayFlags;
    }
    varPtr = (Var *) Tcl_GetHashValue(hPtr);
    varPtr->flags &= ~VAR_TRACE_ACTIVE;
    iPtr->activeTracePtr = active.nextPtr;
    return result;
}

/*
 *----------------------------------------------------------------------
 *
 * NewVar --
 *
 *	Create a new variable with a given initial value.
 *
 * Results:
 *	The return value is a pointer to the new variable structure.
 *	The variable will not be part of any hash table yet, and its
 *	upvarUses count is initialized to 0.  Its initial value will
 *	be empty, but "space" bytes will be available in the value
 *	area.
 *
 * Side effects:
 *	Storage gets allocated.
 *
 *----------------------------------------------------------------------
 */

static Var *
NewVar (int space)		/* Minimum amount of space to allocate
				 * for variable's value. */
{
    int extra;
    register Var *varPtr;

    extra = space - sizeof(varPtr->value);
    if (extra < 0) {
	extra = 0;
	space = sizeof(varPtr->value);
    }
    varPtr = (Var*) malloc ((unsigned) (sizeof(Var) + extra));
    varPtr->valueLength = 0;
    varPtr->valueSpace = space;
    varPtr->upvarUses = 0;
    varPtr->tracePtr = 0;
    varPtr->searchPtr = 0;
    varPtr->flags = 0;
    varPtr->value.string[0] = 0;
    return varPtr;
}

/*
 *----------------------------------------------------------------------
 *
 * ParseSearchId --
 *
 *	This procedure translates from a string to a pointer to an
 *	active array search (if there is one that matches the string).
 *
 * Results:
 *	The return value is a pointer to the array search indicated
 *	by string, or 0 if there isn't one.  If 0 is returned,
 *	interp->result contains an error message.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

static ArraySearch *
ParseSearchId(interp, varPtr, varName, string)
    Tcl_Interp *interp;		/* Interpreter containing variable. */
    Var *varPtr;		/* Array variable search is for. */
    unsigned char *varName;	/* Name of array variable that search is
				 * supposed to be for. */
    char *string;		/* String containing id of search.  Must have
				 * form "search-num-var" where "num" is a
				 * decimal number and "var" is a variable
				 * name. */
{
    char *end;
    int id;
    ArraySearch *searchPtr;

    /*
     * Parse the id into the three parts separated by dashes.
     */

    if ((string[0] != 's') || (string[1] != '-')) {
	syntax:
	Tcl_AppendResult(interp, "illegal search identifier \"", string,
		"\"", 0);
	return 0;
    }
    id = strtoul(string+2, &end, 10);
    if ((end == (string+2)) || (*end != '-')) {
	goto syntax;
    }
    if (strcmp(end+1, varName) != 0) {
	Tcl_AppendResult(interp, "search identifier \"", string,
		"\" isn't for variable \"", varName, "\"", 0);
	return 0;
    }

    /*
     * Search through the list of active searches on the interpreter
     * to see if the desired one exists.
     */

    for (searchPtr = varPtr->searchPtr; searchPtr != 0;
	    searchPtr = searchPtr->nextPtr) {
	if (searchPtr->id == id) {
	    return searchPtr;
	}
    }
    Tcl_AppendResult(interp, "couldn't find search \"", string, "\"", 0);
    return 0;
}

/*
 *----------------------------------------------------------------------
 *
 * DeleteSearches --
 *
 *	This procedure is called to free up all of the searches
 *	associated with an array variable.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Memory is released to the storage allocator.
 *
 *----------------------------------------------------------------------
 */

static void
DeleteSearches(arrayVarPtr)
    register Var *arrayVarPtr;		/* Variable whose searches are
					 * to be deleted. */
{
    ArraySearch *searchPtr;

    while (arrayVarPtr->searchPtr != 0) {
	searchPtr = arrayVarPtr->searchPtr;
	arrayVarPtr->searchPtr = searchPtr->nextPtr;
	free (searchPtr);
    }
}

/*
 *----------------------------------------------------------------------
 *
 * DeleteArray --
 *
 *	This procedure is called to free up everything in an array
 *	variable.  It's the caller's responsibility to make sure
 *	that the array is no longer accessible before this procedure
 *	is called.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	All storage associated with varPtr's array elements is deleted
 *	(including the hash table).  Any delete trace procedures for
 *	array elements are invoked.
 *
 *----------------------------------------------------------------------
 */
static void
DeleteArray(iPtr, arrayName, varPtr, flags)
    Interp *iPtr;			/* Interpreter containing array. */
    unsigned char *arrayName;		/* Name of array (used for trace
					 * callbacks). */
    Var *varPtr;			/* Pointer to variable structure. */
    int flags;				/* Flags to pass to CallTraces:
					 * TCL_TRACE_UNSETS and sometimes
					 * TCL_INTERP_DESTROYED and/or
					 * TCL_GLOBAL_ONLY. */
{
    Tcl_HashSearch search;
    register Tcl_HashEntry *hPtr;
    register Var *elPtr;

    DeleteSearches(varPtr);
    for (hPtr = Tcl_FirstHashEntry(varPtr->value.tablePtr, &search);
	    hPtr != 0; hPtr = Tcl_NextHashEntry(&search)) {
	elPtr = (Var *) Tcl_GetHashValue(hPtr);
	if (elPtr->tracePtr != 0) {
	    (void) CallTraces(iPtr, (Var *) 0, hPtr, arrayName,
		    Tcl_GetHashKey(varPtr->value.tablePtr, hPtr), flags);
	    while (elPtr->tracePtr != 0) {
		VarTrace *tracePtr = elPtr->tracePtr;
		elPtr->tracePtr = tracePtr->nextPtr;
		free (tracePtr);
	    }
	}
	assert ((elPtr->flags & VAR_SEARCHES_POSSIBLE) == 0);

	free (elPtr);
    }
    Tcl_DeleteHashTable(varPtr->value.tablePtr);
    free (varPtr->value.tablePtr);
}

/*
 *----------------------------------------------------------------------
 *
 * VarErrMsg --
 *
 *	Generate a reasonable error message describing why a variable
 *	operation failed.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Interp->result is reset to hold a message identifying the
 *	variable given by part1 and part2 and describing why the
 *	variable operation failed.
 *
 *----------------------------------------------------------------------
 */

static void
VarErrMsg(interp, part1, part2, operation, reason)
    Tcl_Interp *interp;		/* Interpreter in which to record message. */
    unsigned char *part1, *part2; /* Variable's two-part name. */
    unsigned char *operation;	/* String describing operation that failed,
				 * e.g. "read", "set", or "unset". */
    unsigned char *reason;	/* String describing why operation failed. */
{
    Tcl_ResetResult(interp);
    Tcl_AppendResult(interp, "can't ", operation, " \"", part1, 0);
    if (part2 != 0) {
	Tcl_AppendResult(interp, "(", part2, ")", 0);
    }
    Tcl_AppendResult(interp, "\": ", reason, 0);
}
