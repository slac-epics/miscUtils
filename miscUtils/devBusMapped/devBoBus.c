/* devBoBus.c */

/* based on devBoSoftRaw.c -
 * modified by Till Straumann <strauman@slac.stanford.edu>, 2002/11/11
 */

/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/*
 *      Original Author:		Janet Anderson
 *      Date:		3-28-92
 */


#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>

#include	"alarm.h"
#include	"dbDefs.h"
#include	"dbAccess.h"
#include	"recGbl.h"
#include    "recSup.h"
#include	"devSup.h"
#include	"boRecord.h"

#define DEV_BUS_MAPPED_PVT
#include	"devBusMapped.h"

/* added for Channel Access Links */
static long init_record();

/* Create the dset for devBoBus */
static long write_bo();

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	write_bo;
}devBoBus={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	write_bo};



static long init_record(boRecord *prec)
{
long rval = 2; /* don't convert */
	if ( devBusVmeLinkInit(&prec->out, 0, (dbCommon*)prec) ) {
		recGblRecordError(S_db_badField,(void *)prec,
			"devBoBus (init_record) Illegal OUT field");
		return(S_db_badField);
	}

	if (!prec->pini) {
		DevBusMappedPvt pvt = prec->dpvt;
		unsigned		v;
		rval = pvt->acc->rd(pvt, &v, (dbCommon*)prec);
		if ( prec->mask ) {
			v &= prec->mask;
		}
		prec->rval = v;
		recGblResetAlarms(prec);
	}

    return(rval);
} /* end init_record() */

static long write_bo(boRecord *pbo)
{
long			rval;
DevBusMappedPvt pvt = pbo->dpvt;
unsigned 		v;

epicsMutexLock(devBusMappedMutex);
	if ( pbo->mask ) {
		if ( (rval = pvt->acc->rd(pvt, &v, (dbCommon*)pbo) ) < 0 )
			goto leave;
		v &= ~pbo->mask;
		v |= pbo->rval & pbo->mask;
	} else {
		v = pbo->rval;
	}
	rval =  pvt->acc->wr(pvt, v, (dbCommon*)pbo);

leave:
epicsMutexUnlock(devBusMappedMutex);
	return rval;
}
