export CC = g++
INCPATH = 
LIBS = -lmysqlcppconn 
#DEFINES = -DAPPRENTISSAGE
DEFINES = 
export FLAGS = -g -std=c++0x -o3 $(INCPATH) $(LIBS) $(DEFINES)
export TOP := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
export OBJ_DIR = $(TOP)/obj

SRC = $(wildcard *.c*)
OBJ = $(SRC:.cpp=.o) $(SRC:*.cpp=.o)

BASE_SUBDIRS = parser_mgf aa_search combinaisons sql

ifeq ($(DEFINES),-DAPPRENTISSAGE)
BASE_SUBDIRS_2 = $(BASE_SUBDIRS) score_evo
export EXEC = Harpe-apprentissage
else
BASE_SUBDIRS_2 = $(BASE_SUBDIRS)
export EXEC = Harpe
endif

SUBDIRS = $(BASE_SUBDIRS_2) obj

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
