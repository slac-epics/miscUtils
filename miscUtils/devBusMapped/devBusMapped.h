#ifndef DEV_BUS_MAPPED_SUPPORT_H
#define DEV_BUS_MAPPED_SUPPORT_H
/* $Id$ */

/* Unified device support for simple, bus-mapped device registers */

/* A helper routine to parse the INP link field. Since there
 * are not too many link types, we must use VME for everything :-(
 * The idea is as follows:
 * 
 *  The INP/OUT link field is set to something like:
 *  #C<inst> S<shift> @<theDevice>+<offset>,<method>
 *
 *  <theDevice> is a device name string that is looked-up in the
 *  registry, i.e. the device driver must have stored it there.
 *  This device support assumes that the value retrieved from the
 *  registry is the device base address as seen from the CPU.
 *  The device may support multiple (identical) 'subdevices' or
 *  'channels' which can be found at
 *
 *   <base address> + (<inst> << <shift>)
 *
 *  Finally, the <offset> is added to calculate the real register
 *  address.
 *
 *  <method> may be any of 'be8','be16','be32','le16','le32' to 
 *  indicate the access that should be used (big/little endian, data
 *  width). If no <method> is specified, "be32" is assumed.
 *
 *  In addition, user supported access methods are supported. A driver
 *  can simply add an entry to the devBusMappedRegistryId providing
 *  the address of its DevBusMappedAccessRec.
 */

#include "registry.h"
#include "dbCommon.h"
#include "dbAccess.h"
#include "recGbl.h"


typedef struct DevBusMappedPvtRec_ *DevBusMappedPvt;
typedef struct DevBusMappedAccessRec_ *DevBusMappedAccess;

typedef unsigned (*DevBusMappedRead)(DevBusMappedPvt pvt);
typedef void (*DevBusMappedWrite)(DevBusMappedPvt pvt, unsigned value);



typedef struct DevBusMappedAccessRec_ {
	DevBusMappedRead	rd;		/* read access routine				 */
	DevBusMappedWrite	wr;		/* read access routine				 */
} DevBusMappedAccessRec;

typedef struct DevBusMappedPvtRec_ {
	dbCommon			*prec;	/* record this devsup is attached to */
	DevBusMappedAccess	acc;	/* pointer to access methods         */
	volatile void		*addr;
} DevBusMappedPvtRec;

/* parse the link in *l and setup the pvt structure; the
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

/* registry for drivers to enter their base addresses */
extern void	*devBusMappedRegistryId;

#endif
