export CC = g++
INCPATH = 
LIBS = -lmysqlcppconn -lpthread 
#APPRENTISSAGE = -DAPPRENTISSAGE
APPRENTISSAGE = 
export DEFINES = -DCOLORS $(APPRENTISSAGE)
export FLAGS = -g -std=c++0x $(INCPATH) $(LIBS) $(DEFINES)
export TOP := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
export OBJ_DIR = $(TOP)/obj

SRC = $(wildcard *.c*)
OBJ = $(SRC:.cpp=.o) $(SRC:*.cpp=.o)

SUBDIRS = harpe obj

ifeq ($(APPRENTISSAGE),-DAPPRENTISSAGE)
export EXEC = Harpe-apprentissage.exe
else
export EXEC = Harpe.exe
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


#$(EXEC): $(OBJ) $(SUBDIRS)
#	$(CC) $(FLAGS) $^ -o $@


clean: $(CLEANDIRS)

$(CLEANDIRS): 
	$(MAKE) -C $(@:clean-%=%) clean
	@rm -f *.o
	@rm -f $(EXEC)
