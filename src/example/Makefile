CC = g++
CFLAGS = -Wall -g -std=c++11
INCLUDES = -I..
LFLAGS =
LIBS =

SRCS = ../gwRTTI/gwRTTI.cpp $(wildcard *.cpp)

OBJS = $(SRCS:.cpp=.o)
MAIN = example


.PHONY: depend clean

all:    $(MAIN)
		@echo done

$(MAIN): $(OBJS)
		$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)


.cpp.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) ../gwRTTI/*.o *.o *~ $(MAIN)

depend: $(SRCS)
		makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

