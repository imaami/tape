override DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
include $(DIR)/common.mk

SUBDIRS := src

$(eval $(call build-subdirs, all, all, $(SUBDIRS)))
$(eval $(call clean-subdirs, clean, clean, $(SUBDIRS)))

.PHONY: all clean
