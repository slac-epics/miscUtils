TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#USR_CFLAGS += 

ca_DIR           = $(EPICS_BASE_LIB)
Com_DIR          = $(EPICS_BASE_LIB)


#=============================

INC = basicIoOps.h savresUtil.h

LIBRARY_IOC = ssrlMiscUtil

ssrlMiscUtil_SRCS += savres.c

ssrlMiscUtil_LIBS += $(EPICS_BASE_IOC_LIBS)

# xxxRecord.h will be created from xxxRecord.dbd
#DBDINC += xxxRecord

# <name>.dbd will be created from <name>Include.dbd
#DBD += example.dbd

#=============================

#PROD_IOC = example

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

