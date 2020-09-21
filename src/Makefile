override DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
include $(DIR)../common.mk

override TARGET := test

override C_SRC   := $(wildcard $(DIR)*.c)
override CXX_SRC := $(wildcard $(DIR)*.cpp)

override C_OBJ   := $(C_SRC:.c=.c.o)
override CXX_OBJ := $(CXX_SRC:.cpp=.cpp.o)

override OBJ := $(C_OBJ) $(CXX_OBJ)

.PHONY: all clean $(TARGET)

all: $(TARGET)

clean:
	rm -fr $(DIR)$(TARGET) $(OBJ)

$(TARGET): $(DIR)$(TARGET)

$(foreach obj,$(C_OBJ),$(eval $(call build-c-obj,$(obj))))
$(foreach obj,$(CXX_OBJ),$(eval $(call build-cxx-obj,$(obj))))

$(DIR)$(TARGET): $(OBJ)
	cd "$(DIR)" && $(CXX) $(CXXFLAGS) $(notdir $^) -o $(notdir $@)
