SRC = $(wildcard *.c*)
OBJ = $(SRC:.cpp=.o) $(SRC:*.cpp=.o)

BASE_SUBDIRS = parser_mgf aa_search sql analyseur execution

ifeq ($(APPRENTISSAGE),-DAPPRENTISSAGE)
SUBDIRS = $(BASE_SUBDIRS) score_evo
else
SUBDIRS = $(BASE_SUBDIRS)
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
	@cp $@ $(OBJ_DIR)

.c.o :
	$(CC) $(FLAGS) -o $@ -c $^;
	@cp $@ $(OBJ_DIR)


#$(EXEC): $(OBJ) $(SUBDIRS)
#	$(CC) $(FLAGS) $^ -o $@


clean: $(CLEANDIRS)

$(CLEANDIRS): 
	$(MAKE) -C $(@:clean-%=%) clean
	@rm -f *.o
	@rm -f $(EXEC)
