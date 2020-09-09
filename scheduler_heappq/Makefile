
# make clean/all CONFIG=Debug/Release ARCH=linux64/raspi ECHO=0/1

# the parrent dir of "arch" dir
ROOT_DIR = ../../..

#PROJ_SRCS = $(shell xmlstarlet select -t -v //Unit/@filename xx.cbp | grep -e '\.c')
PROJ_SRCS = sched_hpq.c sched_hpq_test.c task.c

# list of directories containing header files
PROJ_INC_DIR = $(ROOT_DIR)/ds/heap $(ROOT_DIR)/ds/priqueue/heap_priq $(ROOT_DIR)/ds/uid $(ROOT_DIR)/ds/vector

# the executable file
PROJ_OUT = sched_hpq_test.out

# list of compiler flags (prepended with "-" sign)
#PROJ_CFLAGS = -Wunused-but-set-variable
PROJ_CFLAGS =

#PROJ_MACROS = _BSD_SOURCE
PROJ_MACROS =

# additional object files (created by other projects)
PROJ_ADD_OBJS = heap.o heap_PQ.o UID.o vector.o

# additional libraries
PROJ_LIBS =

# linker flags
PROJ_LDFLAGS =

# list of projects (makefiles) this project depends on
PROJ_DEPENDS  = $(ROOT_DIR)/ds/uid/Makefile $(ROOT_DIR)/ds/priqueue/heap_priq/Makefile

include $(ROOT_DIR)/Make/Project.mak
