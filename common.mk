ifndef __COMMON_MK_INCLUDED

override __COMMON_MK_INCLUDED := 1
override PROJECT_ROOT         := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

export __COMMON_MK_INCLUDED
export PROJECT_ROOT

$(info PROJECT_ROOT=$(PROJECT_ROOT))

endif # __COMMON_MK_INCLUDED

#########################################
## DO NOT ADD ANYTHING ABOVE THIS LINE ##
#########################################

override HEAD_CFLAGS := -std=gnu11 -Wall -Wextra
override TAIL_CFLAGS := $(if $(strip $(CFLAGS)),$(CFLAGS),-O3 -flto)

override HEAD_CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic
override TAIL_CXXFLAGS := $(if $(strip $(CXXFLAGS)),$(CXXFLAGS),-O3 -flto)

override IFLAGS := -I. -I$(PROJECT_ROOT)include
override CFLAGS := $(HEAD_CFLAGS) $(TAIL_CFLAGS)
override CXXFLAGS := $(HEAD_CXXFLAGS) $(TAIL_CXXFLAGS)

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
	+$(call __recurse,exit 1,$(strip $(2)),$(strip $(3)))
endef

override define clean-subdirs =
.PHONY: $(strip $(1))
$(strip $(1)):
	+$(call __recurse,true,$(strip $(2)),$(strip $(3)))
endef

override define build-c-obj =
.PHONY: $$(notdir $(1))
$$(notdir $(1)): $(1)

$(1): $(1:.c.o=.c)
	cd "$$(DIR)" && \
	$$(CC) $$(CFLAGS) $(IFLAGS) -c $$(notdir $$^) -o $$(notdir $$@)
endef

override define build-cxx-obj =
.PHONY: $$(notdir $(1))
$$(notdir $(1)): $(1)

$(1): $(1:.cpp.o=.cpp)
	cd "$$(DIR)" && \
	$$(CXX) $$(CXXFLAGS) $(IFLAGS) -c $$(notdir $$^) -o $$(notdir $$@)
endef

export build-subdirs
export clean-subdirs
export build-c-obj
export build-cxx-obj
