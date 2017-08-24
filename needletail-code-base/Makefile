CC = gcc
CXX = clang++
ECHO = echo
OFLAGS = -O3
MACROS = -D_FILE_OFFSET_BITS=64
INCLUDES = -IEWAHBoolArray/headers -Iyaml-cpp/include
CFLAGS = -c -g -Wall $(INCLUDES) $(MACROS) $(OFLAGS)
CXXFLAGS = -c -g -Wall -std=c++0x $(INCLUDES) $(MACROS) $(OFLAGS)
STATIC_LIBS = yaml-cpp/build/libyaml-cpp.a
LDFLAGS = -g -Wall $(OFLAGS) -pthread -Lyaml-cpp/build $(STATIC_LIBS)

ifeq ($(shell which clang++),)
	CXX = g++
endif

C_SRCS := $(wildcard *.c)
C_OBJS := $(C_SRCS:.c=.c.o)
C_DEPS := $(C_SRCS:%.c=.%.c.d)

CXX_SRCS := $(wildcard *.cc)
CXX_OBJS := $(CXX_SRCS:.cc=.cc.o)
CXX_DEPS := $(CXX_SRCS:%.cc=.%.cc.d)

MAIN := needletail
ALL := $(MAIN)

all : $(ALL)

$(MAIN) : $(C_OBJS) $(CXX_OBJS) $(STATIC_LIBS)
	$(CXX) $(C_OBJS) -o $@ $(CXX_OBJS) $(LDFLAGS)

%.c.o : %.c Makefile
	$(CC) $(CFLAGS) -MD -MP -MF .$<.d -MT $<.o -o $@ $<

%.cc.o : %.cc Makefile
	$(CXX) $(CXXFLAGS) -MD -MP -MF .$<.d -MT $<.o -o $@ $<

yaml-cpp/build/libyaml-cpp.a :
	mkdir -p yaml-cpp/build
	cd yaml-cpp/build; cmake ..; make

.ycm_flags : Makefile
	$(ECHO) $(CXXFLAGS) > $@

-include $(C_DEPS) $(CXX_DEPS)

.PHONY : clean

clean :
	rm -rf $(C_OBJS) $(CXX_OBJS) $(C_DEPS) $(CXX_DEPS) $(ALL)
