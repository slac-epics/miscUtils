/* helper routine for parsing a VMEIO link address */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <devBusMapped.h>
#include <basicIoOps.h>

/* just any unique address */
void	*devBusMappedRegistryId = (void*)&devBusMappedRegistryId;

static unsigned inbe32(DevBusMappedPvt pvt) { return in_be32(pvt->addr); }
static unsigned inle32(DevBusMappedPvt pvt) { return in_le32(pvt->addr); }
static unsigned inbe16(DevBusMappedPvt pvt) { return in_be16(pvt->addr) & 0xffff; }
static unsigned inle16(DevBusMappedPvt pvt) { return in_le16(pvt->addr) & 0xffff; }
static unsigned in8(DevBusMappedPvt pvt)	{ return in_8(pvt->addr) & 0xff; }

static void outbe32(DevBusMappedPvt pvt, unsigned v) { out_be32(pvt->addr,v); }
static void outle32(DevBusMappedPvt pvt, unsigned v) { out_le32(pvt->addr,v); }
static void outbe16(DevBusMappedPvt pvt, unsigned v) { out_be16(pvt->addr,v&0xffff); }
static void outle16(DevBusMappedPvt pvt, unsigned v) { out_le16(pvt->addr,v&0xffff); }
static void out8(DevBusMappedPvt pvt, unsigned v)    { out_8(pvt->addr, v&0xff); }

static DevBusMappedAccessRec be32 = { inbe32, outbe32 };
static DevBusMappedAccessRec le32 = { inle32, outle32 };
static DevBusMappedAccessRec be16 = { inbe16, outbe16 };
static DevBusMappedAccessRec le16 = { inle16, outle16 };
static DevBusMappedAccessRec io8  = { in8, out8 };

unsigned long
devBusVmeLinkInit(DBLINK *l, DevBusMappedPvt pvt, dbCommon *prec)
{
char          *sep;
unsigned long offset = 0;
unsigned long rval   = 0;
char          *cp    = 0;

	if ( !pvt ) {
		assert( pvt = malloc( sizeof(*pvt) ) );
		prec->dpvt = pvt;
	}

	pvt->prec = prec;
	pvt->acc = &be32;

    switch (l->type) {

    case (CONSTANT) :
			recGblInitConstantLink(l, DBF_ULONG, &rval);
        break;

    case (VME_IO) :

			cp = malloc(strlen(l->value.vmeio.parm) + 1);
			strcpy(cp,l->value.vmeio.parm);

			if ( (sep=strchr(cp,'+')) ) {
				*sep++=0;
				if ( 1!= sscanf(sep,"%li",&offset) ) {
					recGblRecordError(S_db_badField, (void*)prec,
									  "devXXBus (init_record) Invalid OFFSET string");
					break;
				}
			}
			if ( (rval = (unsigned long)registryFind(devBusMappedRegistryId, cp)) ) {
				rval += l->value.vmeio.card << l->value.vmeio.signal;
			}

			if ( (sep=strchr(sep,',')) ) {
				void *found;
				sep++;
				if ( (found = registryFind(devBusMappedRegistryId, sep)) ) {
					pvt->acc = found;
				} else
				if ( !strcmp(sep,"be32") ) {
					pvt->acc = &be32;
				} else
				if ( !strcmp(sep,"le32") ) {
					pvt->acc = &le32;
				} else
				if ( !strcmp(sep,"be16") ) {
					pvt->acc = &be16;
				} else
				if ( !strcmp(sep,"le16") ) {
					pvt->acc = &le16;
				} else
				if ( !strcmp(sep,"be8") ) {
					pvt->acc = &io8;
				} else {
					recGblRecordError(S_db_badField, (void*)prec,
									  "devXXBus (init_record) Invalid ACCESS string");
				}
			}

			/* fall thru */

    default :
		break;
    }

	free(cp);

	if (rval)
		rval += offset;

	pvt->addr = (volatile void*)rval;

	return !rval;
}


