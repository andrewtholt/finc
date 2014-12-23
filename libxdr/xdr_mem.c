/* $Id: xdr_mem.c,v 1.3 2004/12/30 22:18:18 alex Exp $ */

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
static char *rcsid = "$OpenBSD: xdr_mem.c,v 1.9 2002/12/11 22:55:39 millert Exp $";
#endif /* LIBC_SCCS and not lint */

/*
 * xdr_mem.h, XDR implementation using memory buffers.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 *
 * If you have some data to be interpreted as external data representation
 * or to be converted to external data representation in a memory buffer,
 * then this is the package for you.
 *
 */

#include <string.h>

#include <rpc/types.h>
#include <rpc/xdr.h>

#if defined (_WIN32)
#    include  <winsock.h>			/* Windows socket definitions. */
#elif defined (__palmos__)
#    include  <Unix/netinet_in.h>		/* htonl(3), ntohl(3), etc. */
#else
#    include  <netinet/in.h>			/* htonl(3), ntohl(3), etc. */
#endif

static bool_t	xdrmem_getlong_aligned(XDR *, long *)  OCD ("libxdr") ;
static bool_t	xdrmem_putlong_aligned(XDR *, long *)  OCD ("libxdr") ;
static bool_t	xdrmem_getlong_unaligned(XDR *, long *)  OCD ("libxdr") ;
static bool_t	xdrmem_putlong_unaligned(XDR *, long *)  OCD ("libxdr") ;
static bool_t	xdrmem_getbytes(XDR *, caddr_t, unsigned int)  OCD ("libxdr") ;
static bool_t	xdrmem_putbytes(XDR *, caddr_t, unsigned int)  OCD ("libxdr") ;
static unsigned int	xdrmem_getpos(XDR *)  OCD ("libxdr") ; /* XXX w/64-bit pointers, unsigned int not enough! */
static bool_t	xdrmem_setpos(XDR *, unsigned int)  OCD ("libxdr") ;
static int32_t *xdrmem_inline_aligned(XDR *, unsigned int)  OCD ("libxdr") ;
static int32_t *xdrmem_inline_unaligned(XDR *, unsigned int)  OCD ("libxdr") ;
static void	xdrmem_destroy(XDR *)  OCD ("libxdr") ;

static struct	xdr_ops xdrmem_ops_aligned = {
	xdrmem_getlong_aligned,
	xdrmem_putlong_aligned,
	xdrmem_getbytes,
	xdrmem_putbytes,
	xdrmem_getpos,
	xdrmem_setpos,
	xdrmem_inline_aligned,
	xdrmem_destroy
};

static struct	xdr_ops xdrmem_ops_unaligned = {
	xdrmem_getlong_unaligned,
	xdrmem_putlong_unaligned,
	xdrmem_getbytes,
	xdrmem_putbytes,
	xdrmem_getpos,
	xdrmem_setpos,
	xdrmem_inline_unaligned,
	xdrmem_destroy
};

/*
 * The procedure xdrmem_create initializes a stream descriptor for a
 * memory buffer.
 */
void
xdrmem_create(xdrs, addr, size, op)
	XDR *xdrs;
	caddr_t addr;
	unsigned int size;
	enum xdr_op op;
{

	xdrs->x_op = op;
	xdrs->x_ops = ((size_t)addr & (sizeof(int32_t) - 1))
	    ? &xdrmem_ops_unaligned : &xdrmem_ops_aligned;
	xdrs->x_private = xdrs->x_base = addr;
	xdrs->x_handy = size;
}

static void
xdrmem_destroy(XDR *xdrs)

{

}

static bool_t
xdrmem_getlong_aligned(xdrs, lp)
	XDR *xdrs;
	long *lp;
{

	if (xdrs->x_handy < sizeof(int32_t))
		return (FALSE);
	xdrs->x_handy -= sizeof(int32_t);
	*lp = ntohl(*(int32_t *)xdrs->x_private);
	xdrs->x_private += sizeof(int32_t);
	return (TRUE);
}

static bool_t
xdrmem_putlong_aligned(xdrs, lp)
	XDR *xdrs;
	long *lp;
{

	if (xdrs->x_handy < sizeof(int32_t))
		return (FALSE);
	xdrs->x_handy -= sizeof(int32_t);
	*(int32_t *)xdrs->x_private = htonl((uint32_t)*lp);
	xdrs->x_private += sizeof(int32_t);
	return (TRUE);
}

static bool_t
xdrmem_getlong_unaligned(xdrs, lp)
	XDR *xdrs;
	long *lp;
{
	int32_t l;

	if (xdrs->x_handy < sizeof(int32_t))
		return (FALSE);
	xdrs->x_handy -= sizeof(int32_t);
	memcpy(&l, xdrs->x_private, sizeof(int32_t));
	*lp = ntohl(l);
	xdrs->x_private += sizeof(int32_t);
	return (TRUE);
}

static bool_t
xdrmem_putlong_unaligned(xdrs, lp)
	XDR *xdrs;
	long *lp;
{
	int32_t l;

	if (xdrs->x_handy < sizeof(int32_t))
		return (FALSE);
	xdrs->x_handy -= sizeof(int32_t);
	l = htonl((uint32_t)*lp);
	(void) memcpy(xdrs->x_private, &l, sizeof(int32_t));
	xdrs->x_private += sizeof(int32_t);
	return (TRUE);
}

static bool_t
xdrmem_getbytes(xdrs, addr, len)
	XDR *xdrs;
	caddr_t addr;
	unsigned int len;
{

	if (xdrs->x_handy < len)
		return (FALSE);
	xdrs->x_handy -= len;
	(void) memcpy(addr, xdrs->x_private, len);
	xdrs->x_private += len;
	return (TRUE);
}

static bool_t
xdrmem_putbytes(xdrs, addr, len)
	XDR *xdrs;
	caddr_t addr;
	unsigned int len;
{

	if (xdrs->x_handy < len)
		return (FALSE);
	xdrs->x_handy -= len;
	(void) memcpy(xdrs->x_private, addr, len);
	xdrs->x_private += len;
	return (TRUE);
}

static unsigned int
xdrmem_getpos(xdrs)
	XDR *xdrs;
{

	/* XXX w/64-bit pointers, unsigned int not enough! */
	return ((unsigned long)xdrs->x_private - (unsigned long)xdrs->x_base);
}

static bool_t
xdrmem_setpos(xdrs, pos)
	XDR *xdrs;
	unsigned int pos;
{
	caddr_t newaddr = xdrs->x_base + pos;
	caddr_t lastaddr = xdrs->x_private + xdrs->x_handy;

	if (newaddr > lastaddr)
		return (FALSE);
	xdrs->x_private = newaddr;
	xdrs->x_handy = (unsigned int)(lastaddr - newaddr);	/* XXX w/64-bit pointers, unsigned int not enough! */
	return (TRUE);
}

static int32_t *
xdrmem_inline_aligned(xdrs, len)
	XDR *xdrs;
	unsigned int len;
{
	int32_t *buf = 0;

	if (xdrs->x_handy >= len) {
		xdrs->x_handy -= len;
		buf = (int32_t *)xdrs->x_private;
		xdrs->x_private += len;
	}
	return (buf);
}

/* ARGSUSED */
static int32_t *
xdrmem_inline_unaligned(xdrs, len)
	XDR *xdrs;
	unsigned int len;
{

	return (0);
}
