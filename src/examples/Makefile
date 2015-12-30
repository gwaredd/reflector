################################################################################

CC = g++
CCFLAGS = -Wall -g -std=c++11 -I..


# OS variations on shared libs ...

ifdef SystemRoot # SystemRoot is only set on windows

	$(error "Using MSVC on windows ...")

else

	UNAME := $(shell uname -s)

    ifeq ($(UNAME),Darwin) # OSX

		MODULE_EXT = dylib
		MODULE_FLAGS = -dynamiclib -current_version 1.0

	else

		MODULE_EXT = so
		MODULE_FLAGS = -shared
		CCFLAGS += -fPIC
		RPATH = -Wl,-rpath,.

    endif

endif


################################################################################
# build targets

EXAMPLE1 = example1
EXAMPLE2 = example2
MODULE_A = libModuleA.$(MODULE_EXT)
MODULE_B = libModuleB.$(MODULE_EXT)
MODULE_RTTI = libModuleRTTI.$(MODULE_EXT)


.PHONY: clean

all:    $(EXAMPLE1) $(MODULE_RTTI) $(MODULE_A) $(MODULE_B) $(EXAMPLE2)
		@echo done

.cpp.o:
		$(CC) $(CCFLAGS) -c $<  -o $@

clean:
		$(RM) ../gwRTTI/*.o ./Serialisation/*.o ./Modules/*.o $(EXAMPLE1) $(EXAMPLE2) $(MODULE_A) $(MODULE_B) $(MODULE_RTTI) *~



################################################################################
# example 1 - serialisation

EXAMPLE1_SRCS = ../gwRTTI/gwRTTI.cpp $(wildcard ./Serialisation/*.cpp)
EXAMPLE1_OBJS = $(EXAMPLE1_SRCS:.cpp=.o)

$(EXAMPLE1): $(EXAMPLE1_OBJS)
		$(CC) $(EXAMPLE1_OBJS) -o $(EXAMPLE1)


################################################################################
# example 2 - dymamic loading of modules

$(EXAMPLE2): ./Modules/Example2.o
		$(CC) ./Modules/Example2.o $(MODULE_RTTI) $(MODULE_A) -ldl $(RPATH) -o $(EXAMPLE2)

$(MODULE_RTTI): ./Modules/ModuleRTTI.o ../gwRTTI/gwRTTI.o
		$(CC) $(MODULE_FLAGS) ./Modules/ModuleRTTI.o ../gwRTTI/gwRTTI.o -o $(MODULE_RTTI)

$(MODULE_A): ./Modules/ModuleA.o
		$(CC) $(MODULE_FLAGS) ./Modules/ModuleA.o $(MODULE_RTTI) -o $(MODULE_A)

$(MODULE_B): ./Modules/ModuleB.o
		$(CC) $(MODULE_FLAGS) ./Modules/ModuleB.o $(MODULE_RTTI) -o $(MODULE_B)

