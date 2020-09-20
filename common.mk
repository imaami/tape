ifndef __COMMON_MK_INCLUDED

override __COMMON_MK_INCLUDED := 1
override PROJECT_ROOT         := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

export __COMMON_MK_INCLUDED
export PROJECT_ROOT

$(info PROJECT_ROOT=$(PROJECT_ROOT))

#########################################
## DO NOT ADD ANYTHING ABOVE THIS LINE ##
#########################################

override define __recurse =
	target=$(strip $(2)); \
	for subdir in $(3); do \
		echo "Making $$$$target in $$$$subdir"; \
		cd "$$(DIR)$$$$subdir" || $(1); \
		$(MAKE) $$$$target || $(1); \
		cd ..; \
	done
endef

override define build-subdirs =
.PHONY: $(strip $(1))
$(strip $(1)):
	+$(call __recurse,exit 1,$(strip $(1)),$(strip $(2)))
endef

export build-subdirs

override define clean-subdirs =
.PHONY: $(strip $(1))
$(strip $(1)):
	+$(call __recurse,true,$(strip $(1)),$(strip $(2)))
endef

export clean-subdirs

endif # __COMMON_MK_INCLUDED
