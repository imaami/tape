override DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
include $(DIR)../common.mk

override TARGET := example
override MODULES := grid wait

override C_SRC   := $(wildcard $(DIR)*.c)
override CXX_SRC := $(wildcard $(DIR)*.cpp)

override MOD_C_SRC   := $(wildcard $(MODULES:%=$(DIR)%/*.c))
override MOD_CXX_SRC := $(wildcard $(MODULES:%=$(DIR)%/*.cpp))

override C_OBJ   := $(C_SRC:.c=.c.o)
override CXX_OBJ := $(CXX_SRC:.cpp=.cpp.o)

override MOD_C_OBJ   := $(MOD_C_SRC:.c=.c.o)
override MOD_CXX_OBJ := $(MOD_CXX_SRC:.cpp=.cpp.o)

override OBJ := $(C_OBJ) $(CXX_OBJ)
override MOD_OBJ := $(MOD_C_OBJ) $(MOD_CXX_OBJ)

.PHONY: all clean $(TARGET)

all: $(TARGET)

clean: modules-clean
	rm -fr $(DIR)$(TARGET) $(OBJ)

$(TARGET): modules
	+$(MAKE) $(DIR)$(TARGET)

$(eval $(call build-subdirs, modules, all, $(MODULES)))
$(eval $(call clean-subdirs, modules-clean, clean, $(MODULES)))

$(foreach obj,$(C_OBJ),$(eval $(call build-c-obj,$(obj))))
$(foreach obj,$(CXX_OBJ),$(eval $(call build-cxx-obj,$(obj))))

$(DIR)$(TARGET): $(OBJ) $(MOD_OBJ)
	cd "$(DIR)" && $(CC) $(CFLAGS) $(^:$(DIR)%=%) -o $(notdir $@)
