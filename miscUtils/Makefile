# Makefile at top of ipac support tree
# Makefile,v 1.3 1999/03/09 20:28:34 anj Exp
TOP = ..
include $(TOP)/configure/CONFIG

DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Utils))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard drv*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard dev*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Lib))
DIRS += seq ipac gtr asyn ether_ip restore motor timestampRecord
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *App))
#DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Gpib))
# xycomIpac first since it installs xipIo.h used by acromagIpac.
DIRS += xycomIpac
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Ipac))
#DIRS += Hy8002 Hy8401

include $(TOP)/configure/RULES_TOP
