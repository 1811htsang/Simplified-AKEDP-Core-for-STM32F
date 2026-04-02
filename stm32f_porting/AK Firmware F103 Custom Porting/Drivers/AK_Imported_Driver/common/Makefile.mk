include common/container/Makefile.mk
#include sources/common/ctreemap/Makefile.mk

CFLAGS		+= -I./common
CPPFLAGS	+= -I./common

VPATH += common

SOURCES_CPP += common/view_item.cpp
SOURCES_CPP += common/view_render.cpp
SOURCES_CPP += common/screen_manager.cpp

SOURCES += common/utils.c
SOURCES += common/xprintf.c
SOURCES += common/cmd_line.c
