include common.mk

SUBDIRS := src

$(eval $(call build-subdirs, all, $(SUBDIRS)))
$(eval $(call clean-subdirs, clean, $(SUBDIRS)))

.PHONY: all clean
