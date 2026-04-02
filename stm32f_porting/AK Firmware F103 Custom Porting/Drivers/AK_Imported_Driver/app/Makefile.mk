include app/screens/Makefile.mk

CFLAGS		+= -I./app
CPPFLAGS	+= -I./app

VPATH += app

# CPP source files
SOURCES_CPP += app/task_system.cpp
SOURCES_CPP += app/app.cpp
SOURCES_CPP += app/app_data.cpp
SOURCES_CPP += app/app_non_clear_ram.cpp
SOURCES_CPP += app/app_bsp.cpp
SOURCES_CPP += app/app_modbus_pull.cpp
SOURCES_CPP += app/shell.cpp

SOURCES_CPP += app/task_shell.cpp
SOURCES_CPP += app/task_life.cpp
SOURCES_CPP += app/task_fw.cpp
SOURCES_CPP += app/task_list.cpp
SOURCES_CPP += app/task_display.cpp

ifeq ($(TASK_ZIGBEE_OPTION),-DTASK_ZIGBEE_EN)
SOURCES_CPP += app/task_zigbee.cpp
endif

SOURCES_CPP += app/task_if.cpp
SOURCES_CPP += app/task_rf24_if.cpp
SOURCES_CPP += app/task_uart_if.cpp
SOURCES_CPP += app/task_dbg.cpp
