TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

INC += basicIoOps.h
INC += copyright_SLAC.h
INC += debugPrint.h

LIBRARY_IOC = miscUtils
DBD         = miscUtils.dbd

LIBSRCS += miscUtils.c

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

