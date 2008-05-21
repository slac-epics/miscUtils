# Makefile at top of ipac support tree
# Makefile,v 1.3 1999/03/09 20:28:34 anj Exp
TOP = ..
include $(TOP)/configure/CONFIG

DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Utils))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard drv*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard dev*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Lib))
DIRS += seq ipac gtr asyn ether_ip restore motor sSubRecord iocAdmin
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *App))
# Not all Gpib in the repository has been converted to asyn yet.
# Just these:
DIRS += Hp53181AGpib
DIRS += Hp5385Gpib
DIRS += K2001Gpib
DIRS += PTS500Gpib
DIRS += SRSdg535Gpib
#DIRS += Hy8002 Hy8401
#DIRS += Hp3324Gpib
#DIRS += Hp8664Gpib
#DIRS += K2701Gpib
# xycomIpac first since it installs xipIo.h used by acromagIpac.
DIRS += xycomIpac
DIRS += acromagIpac
DIRS += caenADCV965 CaenADCV965_8 ssi
# genSub and sscan before std and calc, calc before optics and stream
DIRS += genSub sscan
DIRS += std calc
DIRS += optics stream
ifneq ($(EPICS_HOST_ARCH),solaris-sparc-gnu)
DIRS += snmp
endif

include $(TOP)/configure/RULES_TOP
