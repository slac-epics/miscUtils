#ifndef LITTLE_ENDIAN_TO_CPU_CONVERSION_H
#define LITTLE_ENDIAN_TO_CPU_CONVERSION_H

/* $Id: endian2cpu.h,v 1.3 2002/11/19 02:28:08 till Exp $ */

#ifdef __rtems__
#include <libcpu/io.h> /* rtems has these already */
#else

#include <sys/types.h>
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
	int		i;
	char	c[sizeof(int)];
} __EndianTestU;

typedef uint32_t __Endian2Cpu32_t;
typedef uint16_t __Endian2Cpu16_t;

typedef union {
	__Endian2Cpu32_t	ui32;
	unsigned char		c[4];
} __Endian2Cpu32U; 

typedef union {
	__Endian2Cpu16_t	ui16;
	unsigned char		c[2];
} __Endian2Cpu16U; 

#ifdef __GNUC__

#define _INLINE_ __inline__	/* we have inline feature */

/* GCC doesn't optimize a static const variable away :-(
 * we may cast unions, however and this works fine
 */
#if 0
#define ENDIAN_TEST_IS_LITTLE	(((__EndianTestU){1}).c[0])
#else
static __inline__ int
__endian_test_is_little(void)
{
const __EndianTestU u = {(int)1};
	return u.c[0];
}
#define ENDIAN_TEST_IS_LITTLE	__endian_test_is_little()
#endif

#else /* not gcc */

#define _INLINE_	/* has probably no inline feature
					 * NOTE: this may lead to instantiation of these routines
					 *       in every object file which uses this header!
					 *       Using other compilers than gcc is only supported
					 *       for testing...
					 */

static const __EndianTestU endianTester={i:1,};
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
#elif defined(__i386__) | defined(__i386)
/* nothing to do on __i386__ */
#else
#warning "Unknown IO barrier/synchronization for this CPU (add an #ifdef <YourCpu> around this warning if none needed by your CPU)"
#endif

#ifndef __iobarrier
#define __iobarrier do{}while(0)
#endif

static _INLINE_ __Endian2Cpu32_t
in_le32(volatile __Endian2Cpu32_t *pval)
{
register __Endian2Cpu32_t rval;
	if (ENDIAN_TEST_IS_LITTLE) {
			rval = *pval;
	} else {
#ifdef ASSEMBLEPPC
		{
		__asm__ __volatile__("lwbrx %0, 0, %1":"=r"(rval):"r"(pval));
		}
#else
		{
		__Endian2Cpu32U src;
	   	src.ui32	= *pval;
		/* brute force */
		rval = (src.c[0]<<24) | (src.c[1]<<16) | (src.c[2]<<8) | src.c[3];
		}
#endif
	}
	__iobarrier;
	return rval;
}

static _INLINE_ __Endian2Cpu32_t
in_be32(volatile __Endian2Cpu32_t *pval)
{
register __Endian2Cpu32_t rval=*pval;
	__iobarrier;
	return ntohl(rval);
}

static _INLINE_ __Endian2Cpu16_t
in_le16(volatile __Endian2Cpu16_t *pval)
{
register __Endian2Cpu16_t rval;
	if (ENDIAN_TEST_IS_LITTLE) {
		rval = *pval;
	} else {
#ifdef ASSEMBLEPPC
		{
		__asm__ __volatile__("lhbrx %0, 0, %1":"=r"(rval):"r"(pval));
		}
#else
		{
		__Endian2Cpu16U src;
	   	src.ui16	= *pval;
		/* brute force */
		rval = (src.c[0]<<8) | src.c[1];
		}
#endif
	}
	__iobarrier;
	return rval;
}

static _INLINE_ __Endian2Cpu16_t
in_be16(volatile __Endian2Cpu16_t *pval)
{
register __Endian2Cpu16_t rval=*pval;
	__iobarrier;
	return ntohs(rval);
}


static _INLINE_ unsigned char
in_8(volatile unsigned char *pval)
{
register unsigned char rval=*pval;
	__iobarrier;
	return rval;
	
}

static _INLINE_ void
out_le32(volatile __Endian2Cpu32_t *addr, __Endian2Cpu32_t val)
{
	if (ENDIAN_TEST_IS_LITTLE) {
		*addr=val;
	} else {
#ifdef ASSEMBLEPPC
		{
		__asm__ __volatile__("stwbrx %1, 0, %2":"=m"(*addr):"r"(val),"r"(addr));
		}
#else
		{
		/* brute force */
		__Endian2Cpu32_t l;
		l = (((val & 0xff000000) >> 24) | 
		     ((val & 0x00ff0000) >>  8) | 
		     ((val & 0x0000ff00) <<  8) |
		     ((val & 0x000000ff) << 24));
		*addr=l;
		}
#endif
	}
	__iobarrier;
}

static _INLINE_ void
out_be32(volatile __Endian2Cpu32_t *addr, __Endian2Cpu32_t val)
{
	*addr = htonl(val);
	__iobarrier;
}

static _INLINE_ void
out_le16(volatile __Endian2Cpu16_t *addr, __Endian2Cpu16_t val)
{
	if (ENDIAN_TEST_IS_LITTLE) {
		*addr=val;
	} else {
#ifdef ASSEMBLEPPC
		{
		__asm__ __volatile__("sthbrx %1, 0, %2":"=m"(*addr):"r"(val),"r"(addr));
		}
#else
		{
		/* brute force */
			val = ((val & 0xff)<<8) | ((val>>8)&0xff);
			*addr=val;
		}
#endif
	}
	__iobarrier;
}

static _INLINE_ void
out_be16(volatile __Endian2Cpu16_t *addr, __Endian2Cpu16_t val)
{
	*addr = htons(val);
	__iobarrier;
}

static _INLINE_ void
out_8(volatile unsigned char *addr, unsigned char val)
{
		*addr=val;
		__iobarrier;
}

#endif /* defined(__rtems__) */
#endif
