/*
 * Miscellaneous utility routines to supplement EPICS base
 */

#include "iocsh.h"
#include "epicsExport.h"

extern void registerPlatformIocsh();

static void miscUtilsRegistrar(void) {
  registerPlatformIocsh();
}
epicsExportRegistrar(miscUtilsRegistrar);
