#Makefile at top of application tree
TOP = ..
include $(TOP)/configure/CONFIG
DIRS := $(DIRS) $(filter-out $(DIRS), $(wildcard miscUtils*))
include $(TOP)/configure/RULES_TOP
