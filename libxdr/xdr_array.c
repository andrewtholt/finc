/* $Id: xdr_array.c,v 1.1 2003/09/16 23:02:02 alex Exp $ */

/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user.
 *
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char *rcsid = "$OpenBSD: xdr_array.c,v 1.6 2002/08/01 01:05:24 deraadt Exp $";
#endif /* LIBC_SCCS and not lint */

/*
 * xdr_array.c, Generic XDR routines impelmentation.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 *
 * These are the "non-trivial" xdr primitives used to serialize and de-serialize
 * arrays.  See xdr.h for more info on the interface to xdr.
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <rpc/types.h>
#include <rpc/xdr.h>

/*
 * XDR an array of arbitrary elements
 * *addrp is a pointer to the array, *sizep is the number of elements.
 * If addrp is NULL (*sizep * elsize) bytes are allocated.
 * elsize is the size (in bytes) of each element, and elproc is the
 * xdr procedure to call to handle each element of the array.
 */
bool_t
xdr_array(xdrs, addrp, sizep, maxsize, elsize, elproc)
	XDR *xdrs;
	caddr_t *addrp;		/* array pointer */
	unsigned int *sizep;	/* number of elements */
	unsigned int maxsize;	/* max numberof elements */
	unsigned int elsize;	/* size in bytes of each element */
	xdrproc_t elproc;	/* xdr routine to handle each element */
{
	caddr_t target = *addrp;
	unsigned int nodesize, c, i;
	bool_t stat = TRUE;

	/* like strings, arrays are really counted arrays */
	if (!xdr_u_int(xdrs, sizep))
		return (FALSE);

	c = *sizep;
	if ((c > maxsize || c > UINT_MAX/elsize) &&
	    xdrs->x_op != XDR_FREE)
		return (FALSE);
	nodesize = c * elsize;

	/*
	 * if we are deserializing, we may need to allocate an array.
	 * We also save time by checking for a null array if we are freeing.
	 */
	if (target == NULL) {
		switch (xdrs->x_op) {
		case XDR_DECODE:
			if (c == 0)
				return (TRUE);
			*addrp = target = mem_alloc(nodesize);
			if (target == NULL) {
				(void) fprintf(stderr,
				    "xdr_array: out of memory\n");
				return (FALSE);
			}
			memset(target, 0, nodesize);
			break;
		case XDR_FREE:
			return (TRUE);
		default:
			break;
		}
	}

	/*
	 * now we xdr each element of array
	 */
	for (i = 0; (i < c) && stat; i++) {
		stat = (*elproc)(xdrs, target);
		target += elsize;
	}

	/*
	 * the array may need freeing
	 */
	if (xdrs->x_op == XDR_FREE) {
		mem_free(*addrp, nodesize);
		*addrp = NULL;
	}
	return (stat);
}

/*
 * xdr_vector():
 *
 * XDR a fixed length array. Unlike variable-length arrays,
 * the storage of fixed length arrays is static and unfreeable.
 * > basep: base of the array
 * > size: size of the array
 * > elemsize: size of each element
 * > xdr_elem: routine to XDR each element
 */
bool_t
xdr_vector(xdrs, basep, nelem, elemsize, xdr_elem)
	XDR *xdrs;
	char *basep;
	unsigned int nelem;
	unsigned int elemsize;
	xdrproc_t xdr_elem;
{
	char *elptr;
	unsigned int i;

	elptr = basep;
	for (i = 0; i < nelem; i++) {
		if (!(*xdr_elem)(xdrs, elptr))
			return(FALSE);
		elptr += elemsize;
	}
	return(TRUE);
}