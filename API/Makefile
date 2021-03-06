ifdef OS
   RM = rmdir /S /Q
	 COPY = copy
   FixPath = $(subst /,\,$1)
	 MKDIR = @mkdir
	 CROSS_COMPILE ?= ../toolchain/arm-ev3liga-linux-gnueabi-windows/bin/arm-ev3liga-linux-gnueabi-
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
CPP_SRCS = $(wildcard *.cpp) $(wildcard processes/*.cpp)
CPP_INCLUDE = $(patsubst %,out/include/%,$(wildcard *.h)) $(patsubst %,out/include/%,$(wildcard processes/*.hpp))
CPP_OBJS = $(patsubst %,out/obj/%.o,$(CPP_SRCS))
CFLAGS += -fno-strict-aliasing
CFLAGS += -Wall -Wextra -Wpointer-sign -Wno-unused-parameter -O4
CXXFLAGS += -fno-strict-aliasing --std=c++17 -Wall -O4

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
	$(CC) $(CFLAGS) -isystem. -o $@ -c $<

out/obj/%.o: %
	$(MKDIR) $(call FixPath,$(@D)) || @echo
	$(CXX) $(CXXFLAGS) -isystem. -o $@ -c $<
	
out/include/%.h: %.h
	$(MKDIR) $(call FixPath,$(@D)) || @echo
	$(COPY) $(call FixPath,$<) $(call FixPath,$@)

out/include/%.hpp: %.hpp
	$(MKDIR) $(call FixPath,$(@D)) || @echo
	$(COPY) $(call FixPath,$<) $(call FixPath,$@)

.PHONY: clean
clean:
	$(RM) out || @echo
	$(RM) example || @echo