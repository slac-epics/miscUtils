# Makefile at top of ipac support tree
# Makefile,v 1.3 1999/03/09 20:28:34 anj Exp
TOP = ..
include $(TOP)/configure/CONFIG

DIRS += ipac
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Utils))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *Lib))
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard *App))
#DIRS += ether_ip

include $(TOP)/configure/RULES_TOP
