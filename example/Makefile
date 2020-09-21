override DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
include $(DIR)../common.mk

override TARGET := example
override SUBDIRS := grid wait

override C_SRC   := $(wildcard $(DIR)*.c)
override CXX_SRC := $(wildcard $(DIR)*.cpp)

override C_OBJ   := $(C_SRC:.c=.c.o)
override CXX_OBJ := $(CXX_SRC:.cpp=.cpp.o)

override OBJ := $(C_OBJ) $(CXX_OBJ)
override MOD_OBJ := grid/grid.c.o wait/wait.c.o

.PHONY: all clean $(TARGET)

all: $(TARGET)

clean: modules-clean
	rm -fr $(DIR)$(TARGET) $(OBJ)

$(TARGET): modules
	+$(MAKE) $(DIR)$(TARGET)

$(eval $(call build-subdirs, modules, all, $(SUBDIRS)))
$(eval $(call clean-subdirs, modules-clean, clean, $(SUBDIRS)))

$(foreach obj,$(C_OBJ),$(eval $(call build-c-obj,$(obj))))
$(foreach obj,$(CXX_OBJ),$(eval $(call build-cxx-obj,$(obj))))

$(DIR)$(TARGET): $(OBJ)
	cd "$(DIR)" && $(CC) $(CFLAGS) $(notdir $^) $(MOD_OBJ) -o $(notdir $@)
