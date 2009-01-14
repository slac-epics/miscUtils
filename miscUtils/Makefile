# Makefile at top of ipac support tree
# Makefile,v 1.3 1999/03/09 20:28:34 anj Exp
TOP = ..
include $(TOP)/configure/CONFIG

ifneq ($(EPICS_HOST_ARCH),solaris-sparc-gnu)
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
DIRS += snmp
else
# for solaris and vxWorks, build only a subset of site
DIRS += seq asyn ether_ip restore sSubRecord iocAdmin
DIRS += Hp53181AGpib
DIRS += Hp5385Gpib
DIRS += K2001Gpib
DIRS += PTS500Gpib
DIRS += SRSdg535Gpib
#DIRS += Hy8002 Hy8401
#DIRS += Hp3324Gpib
#DIRS += Hp8664Gpib
#DIRS += K2701Gpib
# genSub and sscan before std and calc, calc before optics
DIRS += genSub sscan
DIRS += std calc
DIRS += optics
DIRS += epvxi
endif

include $(TOP)/configure/RULES_TOP
