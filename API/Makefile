ifdef OS
   RM = rmdir /S /Q
	 COPY = copy
   FixPath = $(subst /,\,$1)
	 MKDIR = @mkdir
	 CROSS_COMPILE ?= C:/ev3/toolchain/bin/arm-ev3liga-linux-gnueabi-
else
   RM = rm -rf
	 COPY = cp -r
   FixPath = $1
	 MKDIR = mkdir -p
	 CROSS_COMPILE ?= $(HOME)/ev3/toolchain/bin/arm-c4ev3-linux-gnueabi-
endif

PREFIX ?= $(CROSS_COMPILE)
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
AR = $(PREFIX)ar
CORE_SRCS = $(wildcard core/*.c)
CORE_INCLUDE = $(patsubst %,out/include/%,$(wildcard core/*.h))
CORE_OBJS = $(patsubst %,out/obj/%.o,$(CORE_SRCS))
CPP_SRCS = $(wildcard *.cpp)
CPP_INCLUDE = $(patsubst %,out/include/%,$(wildcard *.h))
CPP_OBJS = $(patsubst %,out/obj/%.o,$(CPP_SRCS))
CFLAGS += -fno-strict-aliasing
CFLAGS += -Wall -Wextra -Wpointer-sign -Wno-unused-parameter
CXXFLAGS += -fno-strict-aliasing --std=c++17 -Wall

.DEFAULT: all
all: out/lib/libev3apicore.a out/lib/libev3api.a
out/lib/libev3apicore.a: $(CORE_OBJS) $(CORE_INCLUDE)
	$(MKDIR) $(call FixPath,$(@D)) || @echo
	$(AR) rcs $@ $(CORE_OBJS)

out/lib/libev3api.a: $(CORE_OBJS) $(CPP_OBJS) $(CORE_INCLUDE) $(CPP_INCLUDE)
	$(MKDIR) $(call FixPath,$(@D)) || @echo
	$(AR) rcs $@ $(CORE_OBJS) $(CPP_OBJS)

out/obj/core/%.o: core/%
	$(MKDIR) $(call FixPath,$(@D)) || @echo
	$(CC) -Os $(CFLAGS) -isystem. -o $@ -c $<

out/obj/%.o: %
	$(MKDIR) $(call FixPath,$(@D)) || @echo
	$(CXX) -Os $(CXXFLAGS) -isystem. -o $@ -c $<
	
out/include/%.h: %.h
	$(MKDIR) $(call FixPath,$(@D)) || @echo
	$(COPY) $(call FixPath,$<) $(call FixPath,$@)

example:
	echo 'int main(void) { return EV3IsInitialized() == 1; }' | $(CC) -xc $(CFLAGS) - -L. -lev3apicore -I. -oexample -include core/ev3.h

.PHONY: clean
clean:
	$(RM) out || @echo
	$(RM) example || @echo