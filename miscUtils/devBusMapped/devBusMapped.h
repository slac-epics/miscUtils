#ifndef DEV_BUS_MAPPED_SUPPORT_H
#define DEV_BUS_MAPPED_SUPPORT_H
/* $Id: devBusMapped.h,v 1.3 2003/08/13 23:19:18 till Exp $ */

/* Unified device support for simple, bus-mapped device registers */

/* A helper routine to parse the INP link field. Since there
 * are not too many link types, we must use VME for everything :-(
 * The idea is as follows:
 * 
 *  The INP/OUT link field is set to something like:
 *  #C<inst> S<shift> @<theDevice>+<offset>,<method>
 *
 *  <theDevice> is a device name string that is looked-up in the
 *  registry, i.e. the device driver must have stored it there
 *  (using the 'devBusMappedRegister()' wrapper).
 *
 *  The device may support multiple (identical) 'subdevices' or
 *  'channels' which can be found at
 *
 *   <base address> + (<inst> << <shift>)
 *
 *  Finally, the <offset> is added to calculate the real register
 *  address.
 *
 *  <method> may be any of 'be8[s]','be16[s]','be32','le16[s]','le32' to 
 *  indicate the access that should be used (big/little endian, data
 *  width; the optional 's' [e.g. le16s] indicates that the short
 *  little endian data are signed). If no <method> is specified,
 *  "be32" is assumed.
 *
 *  In addition, user supported access methods are supported. A driver
 *  can simply register its DevBusMappedAccessRec with a call to
 *  'devBusMappedRegisterIO()'.
 */

#include <dbCommon.h>
#include <dbAccess.h>
#include <recGbl.h>
#include <epicsMutex.h>


typedef struct DevBusMappedPvtRec_ *DevBusMappedPvt;
typedef struct DevBusMappedAccessRec_ *DevBusMappedAccess;

/* Read and write methods which are used by the device support 'read' and 'write'
 * routines.
 */
typedef int (*DevBusMappedRead)(DevBusMappedPvt pvt, unsigned *pvalue, dbCommon *prec);
typedef int (*DevBusMappedWrite)(DevBusMappedPvt pvt, unsigned value, dbCommon *prec);

typedef struct DevBusMappedAccessRec_ {
	DevBusMappedRead	rd;		/* read access routine				 */
	DevBusMappedWrite	wr;		/* read access routine				 */
} DevBusMappedAccessRec;

/* "per-device" information kept in the registry */
typedef struct DevBusMappedDevRec_ {
	volatile void *baseAddr;
	epicsMutexId  mutex;		/* any other driver/devSup sharing registers
								 * with devBusMapped MUST LOCK THIS MUTEX when
                                 * performing modifications or non-atomical reads.
								 */
	const char    name[1];		/* space for the terminating NULL; the entire string
								 * is appended here, however.
								 */
} DevBusMappedDevRec, *DevBusMappedDev;

/* Data "private" to the 'devBusMapped' device support. This goes
 * into a struct to be attached to the record's 'DPVT' field.
 * (In most cases, it's the only thing to be attached there - that's
 * what happens if you call 'devBusVmeLinkInit()' below with a
 * NULL 'pvt' argument. However, special device support could
 * make the 'DevBusMappedPvtRec' part of a bigger struct and pass
 * a pointer to the 'DevBusMappedPvtRec' subpart to 'devBusVmeLinkInit()'.)
 */
typedef struct DevBusMappedPvtRec_ {
	dbCommon			*prec;	/* record this devsup is attached to */
	DevBusMappedAccess	acc;	/* pointer to access methods         */
	DevBusMappedDev		dev;	/* per-device info                   */
	void				*udata;	/* private data for access methods   */
	volatile void		*addr;	/* reg. address (offset from base)   */
} DevBusMappedPvtRec;

/* Parse the link in *l and setup the pvt structure; the
 * caller may pass a preallocated pvt struct.
 * If she passes 'pvt==NULL' a PvtRec is allocated and attached
 * to prec->dpvt. If pvt is non-NULL, it is _not_ attached.
 *
 * (Reason for passing 'l' is that we don't know whether prec has
 * 'inp' or 'out'...)
 *
 * RETURNS: 0 on success nonzero on error (no base address found, parsing
 *          error etc.)
 */

unsigned long
devBusVmeLinkInit(DBLINK *l, DevBusMappedPvt pvt, dbCommon *prec);

/* Register a device's base address and return a pointer to a
 * freshly allocated and registered 'DevBusMappedDev' struct
 * or NULL on failure.
 * 'baseAddress' is the device's base address as seen by the
 * CPU, i.e. VME or PCI base addresses must be properly translated
 * prior to submitting them to this routine.
 */
DevBusMappedDev
devBusMappedRegister(char *name, volatile void * baseAddress);

/* Register an IO access method; returns 0 on success, nonzero on failure */
int
devBusMappedRegisterIO(char *name, DevBusMappedAccess accessMethods);

/* Find the 'devBusMappedDev' of a registered device by name */
DevBusMappedDev
devBusMappedFind(char *name);

#endif
