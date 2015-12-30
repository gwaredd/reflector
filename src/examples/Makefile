################################################################################

CC = g++ -Wall -g -std=c++11 -I..

EXAMPLE1 = example1
EXAMPLE2 = example2
MODULE_A = libModuleA.dylib
MODULE_B = libModuleB.dylib
MODULE_RTTI = libModuleRTTI.dylib

.PHONY: clean

all:    $(EXAMPLE1) $(MODULE_RTTI) $(MODULE_A) $(MODULE_B) $(EXAMPLE2)
		@echo done

.cpp.o:
		$(CC) -c $<  -o $@

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
		$(CC) ./Modules/Example2.o libModuleRTTI.dylib libModuleA.dylib -o $(EXAMPLE2)

$(MODULE_RTTI): ./Modules/ModuleRTTI.o ../gwRTTI/gwRTTI.o
		$(CC) -dynamiclib -current_version 1.0 ./Modules/ModuleRTTI.o ../gwRTTI/gwRTTI.o -o $(MODULE_RTTI)

$(MODULE_A): ./Modules/ModuleA.o
		$(CC) -dynamiclib -current_version 1.0 ./Modules/ModuleA.o libModuleRTTI.dylib -o $(MODULE_A)

$(MODULE_B): ./Modules/ModuleB.o
		$(CC) -dynamiclib -current_version 1.0 ./Modules/ModuleB.o libModuleRTTI.dylib -o $(MODULE_B)

