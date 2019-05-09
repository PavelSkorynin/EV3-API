## Copyright (c) 2015 Ahmad Fatoum
CROSS_COMPILE ?= C:/ev3/toolchain/bin/arm-ev3liga-linux-gnueabi-
PREFIX ?= $(CROSS_COMPILE)
RM ?= del
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AR = $(PREFIX)ar
CORE_SRCS = $(wildcard core/*.c)
CORE_OBJS = $(patsubst %.c,out/%.o,$(CORE_SRCS))
CPP_SRCS = $(wildcard *.cpp)
CPP_OBJS = $(patsubst %.cpp,out/%.o,$(CPP_SRCS))
CFLAGS += -fno-strict-aliasing

.DEFAULT: libev3apicore.a
all: libev3apicore.a libev3api.a
libev3apicore.a: $(CORE_OBJS)
	$(AR) rcs $@ $^

libev3api.a: $(CPP_OBJS)
	$(AR) rcs $@ $^

out/core/%.o: core/%.c
	@mkdir $(subst /,\\,$(@D)) || @echo
	$(CC) -Os $(CFLAGS) -isystem. -o $@ -c $<

out/%.o: %.cpp
	@mkdir $(subst /,\\,$(@D)) || @echo
	$(CXX) --std=c++11 -Wall -Os $(CFLAGS) -isystem. -o $@ -c $<

example:
	echo 'int main(void) { return EV3IsInitialized() == 1; }' | $(CC) -xc $(CFLAGS) - -L. -lev3apicore -I. -oexample -include core/ev3.h

#ifeq ($(OS),Windows_NT)
#RM = del /Q
#endif

.PHONY: clean
clean:
	$(RM) *.o *.a *.d example
