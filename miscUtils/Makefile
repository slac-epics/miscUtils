# Makefile at top of ipac support tree
# Makefile,v 1.3 1999/03/09 20:28:34 anj Exp
TOP = ..
include $(TOP)/configure/CONFIG

DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Utils))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard drv*))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Lib))
DIRS += seq ipac gtr gpib ether_ip
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *App))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Gpib))
# xycomIpac first since it installs xipIo.h used by acromagIpac.
DIRS := xycomIpac acromagIpac
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Ipac))

include $(TOP)/configure/RULES_TOP
