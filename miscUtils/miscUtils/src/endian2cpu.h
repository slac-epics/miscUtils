#ifndef LITTLE_ENDIAN_TO_CPU_CONVERSION_H
#define LITTLE_ENDIAN_TO_CPU_CONVERSION_H

/* $Id: endian2cpu.h,v 1.1 2002/04/29 20:08:27 till Exp $ */

#ifdef __rtems
#include <libcpu/io.h> /* rtems has these already */
#else

#include <arpa/inet.h>
#include <netinet/in.h>	/* for to/fro bigendian conversion */

/* Routines to convert little endian data
 * to CPU representation and vice versa.
 *
 * NOTE: Using other compilers than gcc is only supported
 *       for testing and may yield non-optimal results...
 */

/* Author: Till Straumann <strauman@slac.stanford.edu>, 4/2002
 *
 * Some ideas are borrowed from Jeff Hill <johill@lanl.gov>'s
 * osiWireFormat.
 */

/* Union type to probe endianness;
 * Access a chunk of data either as a byte stream 
 * or as a int.
 *
 * The idea is to use a constant
 *
 *   EndianTestU endianTester;
 *
 * initialized to
 *
 *   endianTester.i=1;
 * 
 * and subsequentially probing it:
 *
 *   if (endianTester.c[0]) {
 *       its_little_endian();
 *   } else {
 *       its_big_endian();
 *   }
 *
 * Ideally, the compiler performs this check and
 * optimizes out the unused code (including the check itself)
 *
 * I found that this works e.g. with the solaris compiler
 * GCC needs additional help, however.
 */

typedef union {
	int	i;
	char	c[sizeof(int)];
} EndianTestU;

#ifdef __GNUC__

#define _INLINE_ __inline__	/* we have inline feature */

/* GCC doesn't optimize a static const variable away :-(
 * we may cast unions, however and this works fine
 */
#define ENDIAN_TEST_IS_LITTLE	(((EndianTestU){1}).c[0])

#else /* not gcc */

#define _INLINE_	/* has probably no inline feature
					 * NOTE: this may lead to instantiation of these routines
					 *       in every object file which uses this header!
					 *       Using other compilers than gcc is only supported
					 *       for testing...
					 */

static const EndianTestU endianTester={i:1,};
#define ENDIAN_TEST_IS_LITTLE	(endianTester.c[0])

#endif /* if __GNUC__ */


/* gcc allows us to use PPC specific instructions (inline assembly) */
#if  defined(_ARCH_PPC) || defined(__PPC__) || defined(__PPC)
#ifdef __GNUC__
#define ASSEMBLEPPC
#else
#error "You have to find a way to generate inline assembly for this compiler"
#endif
#endif

#ifdef ASSEMBLEPPC
#define __iobarrier	do { __asm__ __volatile__ ("eieio"); } while(0)
#else
#warning "Unknown IO barrier/synchronization for this CPU (add an #ifdef <YourCpu> around this warning if none needed by your CPU)"
#define __iobarrier do{}while(0)
#endif

static _INLINE_ unsigned long
in_le32(volatile void *pval)
{
register unsigned long rval;
	if (ENDIAN_TEST_IS_LITTLE) {
		if (sizeof(unsigned long)==4) {
			rval = *(unsigned long*)pval;
		} else if (sizeof(unsigned int)==4) {
			rval = *(unsigned int*)pval;
		} else {
			/* brute force; should be optimized away */
			int i;
			for (i=3,rval=0; i>=0; i--) {
				rval<<=8;
				rval|=((unsigned char*)pval)[i];
			}
		}
	} else {
#ifdef ASSEMBLEPPC
		{
		register unsigned long rval;
		__asm__ __volatile__("lwbrx %0, 0, %1":"=r"(rval):"r"(pval));
		}
#else
		{
		unsigned char *cp=(unsigned char*)pval;
		/* brute force */
		rval = (cp[0]<<24) | (cp[1]<<16) | (cp[2]<<8) | cp[3];
		}
#endif
	}
	__iobarrier;
	return rval;
}

static _INLINE_ unsigned long
in_be32(volatile void *pval)
{
register unsigned long rval=ntohl(*(volatile unsigned long*)pval);
	__iobarrier;
	return rval;
}

static _INLINE_ unsigned short
in_le16(volatile unsigned short *pval)
{
register unsigned short rval;
		if (ENDIAN_TEST_IS_LITTLE) {
				rval = *pval;
		} else {
#ifdef ASSEMBLEPPC
		{
		register unsigned short rval;
		__asm__ __volatile__("lhbrx %0, 0, %1":"=r"(rval):"r"(pval));
		}
#else
		{
		unsigned char *cp=(unsigned char*)pval;
		/* brute force */
		rval = (cp[0]<<8) | cp[1];
		}
#endif
		}
		__iobarrier;
		return rval;
}

static _INLINE_ unsigned short
in_be16(volatile void *pval)
{
register unsigned short rval=ntohs(*(volatile unsigned short*)pval);
	__iobarrier;
	return rval;
}


static _INLINE_ unsigned char
in_8(volatile unsigned char *pval)
{
register unsigned char rval=*pval;
	__iobarrier;
	return rval;
	
}

static _INLINE_ void
out_le32(volatile void *addr, unsigned long val)
{
		if (ENDIAN_TEST_IS_LITTLE) {
			if (sizeof(unsigned long)==4) {
					*(unsigned long*)addr=val;
			} else if (sizeof(unsigned int)==4) {
					*(unsigned int*)addr=(unsigned int)val;
			} else {
					/* brute force; hopefully optimized away */
				int i;
				for (i=0; i<4; i++,val>>=8) {
					((unsigned char*)addr)[i] = (unsigned char)(val & 0xff);
				}
			}
		} else {
#ifdef ASSEMBLEPPC
		{
		register unsigned short rval;
		__asm__ __volatile__("stwbrx %1, 0, %2":"=m"(*(unsigned long *)addr):"r"(val),"r"(addr));
		}
#else
		{
			/* brute force; unused branches are optimized away */
			if (sizeof(unsigned long)==4) {
					unsigned long l;
					l = (((val & 0xff000000) >> 24) | 
					     ((val & 0x00ff0000) >>  8) | 
					     ((val & 0x0000ff00) <<  8) |
					     ((val & 0x000000ff) << 24));
					*(volatile unsigned long*)addr=l;
			} else if (sizeof(unsigned int)==4) {
					unsigned int  i;
					i = (((val & 0xff000000) >> 24) | 
					     ((val & 0x00ff0000) >>  8) | 
					     ((val & 0x0000ff00) <<  8) |
					     ((val & 0x000000ff) << 24));
					*(volatile unsigned int*)addr=i;
			} else {
				volatile unsigned char *cp=addr;
				*cp++=(unsigned char)(val&0xff); val >>= 8;
				*cp++=(unsigned char)(val&0xff); val >>= 8;
				*cp++=(unsigned char)(val&0xff); val >>= 8;
				*cp++=(unsigned char)(val&0xff); val >>= 8;
			}
		}
#endif
		}
		__iobarrier;
}

static _INLINE_ void
out_be32(volatile void *addr, unsigned long val)
{
	*(volatile unsigned long*)addr = htonl(val);
	__iobarrier;
}

static _INLINE_ void
out_le16(volatile void *addr, unsigned short val)
{
		if (ENDIAN_TEST_IS_LITTLE) {
				*(volatile unsigned short*)addr=val;
		} else {
#ifdef ASSEMBLEPPC
		{
		register unsigned short rval;
		__asm__ __volatile__("sthbrx %1, 0, %2":"=m"(*(unsigned short*)addr):"r"(val),"r"(addr));
		}
#else
		{
		/* brute force */
			val = ((val & 0xff)<<8) | ((val>>8)&0xff);
			*(volatile unsigned short*)addr=val;
		}
#endif
		}
		__iobarrier;
}

static _INLINE_ void
out_be16(volatile void *addr, unsigned short val)
{
	*(volatile unsigned short*)addr = htons(val);
	__iobarrier;
}

static _INLINE_ void
out_8(volatile void *addr, unsigned char val)
{
		*(volatile unsigned char*)addr=val;
		__iobarrier;
}

#endif /* defined(__rtems) */
#endif
