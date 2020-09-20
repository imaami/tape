override DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
include $(DIR)/../common.mk

override TARGET := test
override IFLAGS := -I$(DIR) -I$(PROJECT_ROOT)include

override C_SRC   := $(wildcard $(DIR)*.c)
override CXX_SRC := $(wildcard $(DIR)*.cpp)

override C_OBJ   := $(C_SRC:.c=.c.o)
override CXX_OBJ := $(CXX_SRC:.cpp=.cpp.o)

override OBJ := $(C_OBJ) $(CXX_OBJ)

override HEAD_CFLAGS := -std=gnu11 -Wall -Wextra
override TAIL_CFLAGS := $(if $(strip $(CFLAGS)),$(CFLAGS),-O3 -flto)

override HEAD_CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic
override TAIL_CXXFLAGS := $(if $(strip $(CXXFLAGS)),$(CXXFLAGS),-O3 -flto)

override CFLAGS := $(HEAD_CFLAGS) $(TAIL_CFLAGS) $(IFLAGS)
override CXXFLAGS := $(HEAD_CXXFLAGS) $(TAIL_CXXFLAGS) $(IFLAGS)

CC := gcc
CXX := g++

.PHONY: all clean $(TARGET)

all: $(TARGET)

clean:
	rm -fr $(DIR)$(TARGET) $(OBJ)

$(TARGET): $(DIR)$(TARGET)

override define build-c-obj =
.PHONY: $$(notdir $(1))
$$(notdir $(1)): $(1)

$(1): $(1:.c.o=.c)
	cd "$$(DIR)" && \
	$$(CC) $$(CFLAGS) -c $$(notdir $$^) -o $$(notdir $$@)
endef

override define build-cxx-obj =
.PHONY: $$(notdir $(1))
$$(notdir $(1)): $(1)

$(1): $(1:.cpp.o=.cpp)
	cd "$$(DIR)" && \
	$$(CXX) $$(CXXFLAGS) -c $$(notdir $$^) -o $$(notdir $$@)
endef

$(foreach obj,$(C_OBJ),$(eval $(call build-c-obj,$(obj))))
$(foreach obj,$(CXX_OBJ),$(eval $(call build-cxx-obj,$(obj))))

$(DIR)$(TARGET): $(OBJ)
	cd "$(DIR)" && $(CXX) $(CXXFLAGS) $(notdir $^) -o $(notdir $@)
