override DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
include $(DIR)/common.mk

SUBDIRS := src

$(eval $(call build-subdirs, all, $(SUBDIRS)))
$(eval $(call clean-subdirs, clean, $(SUBDIRS)))

.PHONY: all clean
