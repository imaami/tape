override DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
include $(DIR)../../common.mk

override IFLAGS += -I..

override C_SRC   := $(wildcard $(DIR)*.c)
override CXX_SRC := $(wildcard $(DIR)*.cpp)

override C_OBJ   := $(C_SRC:.c=.c.o)
override CXX_OBJ := $(CXX_SRC:.cpp=.cpp.o)

override OBJ := $(C_OBJ) $(CXX_OBJ)

.PHONY: all clean

all: $(OBJ)

clean:
	rm -fr $(OBJ)

$(foreach obj,$(C_OBJ),$(eval $(call build-c-obj,$(obj))))
$(foreach obj,$(CXX_OBJ),$(eval $(call build-cxx-obj,$(obj))))
