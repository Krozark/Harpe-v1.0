export SYSTEM = LINUX
#export SYSTEM = WIN

export APPRENTISSAGE = -DAPPRENTISSAGE
#export APPRENTISSAGE = 

#STATIC = 0


ifeq ($(SYSTEM),LINUX)
export CC = g++
export EXTENTION =
export INCPATH =
else
export CC = i586-mingw32msvc-g++
export EXTENTION = .exe
export INCPATH = -I/usr/include/ -I/usr/include/i386-linux-gnu/
endif

LIBS = -lmysqlcppconn -lpthread 

#ifeq ($(STATIC),0)
#SFLAGS = 
#else
#SFLAGS = -static  -Ldir_path -lmysqlclient
#endif

export DEFINES = -DCOLORS $(APPRENTISSAGE) 
export FLAGS = -g -std=c++0x $(SFLAGS) $(INCPATH) $(LIBS) $(DEFINES)
export TOP := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
export OBJ_DIR = $(TOP)/obj

SRC = $(wildcard *.c*)
OBJ = $(SRC:.cpp=.o) $(SRC:*.cpp=.o)

SUBDIRS = harpe obj

ifeq ($(APPRENTISSAGE),-DAPPRENTISSAGE)
export EXEC = Harpe-apprentissage$(EXTENTION)
else
export EXEC = Harpe$(EXTENTION)
endif

CLEANDIRS = $(SUBDIRS:%=clean-%)

.PHONY: subdirs $(SUBDIRS)
.PHONY: subdirs $(CLEANDIRS)

all: $(OBJ) subdirs

subdirs: $(SUBDIRS)
     
$(SUBDIRS):
	$(MAKE) -C $@


.cpp.o:
	$(CC) $(FLAGS) -o $@ -c $^;
	@mv $@ obj

.c.o :
	$(CC) $(FLAGS) -o $@ -c $^;
	@mv $@ obj


clean: $(CLEANDIRS)

$(CLEANDIRS): 
	$(MAKE) -C $(@:clean-%=%) clean
	@rm -f *.o
	@rm -f $(EXEC)
