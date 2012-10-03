# Makefile at top of site support tree
# Makefile,v 1.3 1999/03/09 20:28:34 anj Exp
TOP = ..
include $(TOP)/configure/CONFIG

ifneq ($(EPICS_HOST_ARCH),solaris-sparc-gnu)
DIRS += miscUtils
DIRS += drvRTEMSDma
DIRS += devBusMapped devGenVar
DIRS += iocMonLib
DIRS += gtr
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
DIRS += acromagIpac
DIRS += caenADCV965 CaenADCV965_8 ssi
# sscan before std and calc, calc before optics and mca
DIRS += sscan busy
DIRS += std calc
DIRS += optics
DIRS += mca
DIRS += event
DIRS += s7plcTcp
DIRS += modbus
# vmeCardRecord before vsam
DIRS += vmeCardRecord
DIRS += vsam
else
# for solaris and vxWorks, build only a subset of site
DIRS += miscUtils
DIRS += drvRTEMSDma
DIRS += devBusMapped
DIRS += iocMonLib
DIRS += Hp53181AGpib
DIRS += Hp5385Gpib
DIRS += K2001Gpib
DIRS += PTS500Gpib
DIRS += SRSdg535Gpib
#DIRS += Hy8002 Hy8401
#DIRS += Hp3324Gpib
#DIRS += Hp8664Gpib
#DIRS += K2701Gpib
# sscan before std and calc, calc before optics
DIRS += sscan busy
DIRS += std calc
DIRS += optics
DIRS += s7plcTcp
DIRS += modbus
endif

include $(TOP)/configure/RULES_TOP
