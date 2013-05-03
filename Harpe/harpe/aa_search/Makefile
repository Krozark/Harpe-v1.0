all: $(OBJ) aa_search.exe

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

aa_search.exe: main.cpp defines.hpp AA_tab.o AA.o Enzyme.o AA_model.o Enzyme_model.o
	$(CC) $^ $(FLAGS) -o $@


clean:
	@rm -f *.o
	@rm -f main
	@rm -f aa_search.exe
