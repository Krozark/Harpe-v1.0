all: $(OBJ) main

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<" 
	@cp $@ $(OBJ_DIR)

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

main: analyseur_peptide.o ../combinaisons/src/Combinaison.o
	#$(CC) $^ $(FLAGS) -o $@


clean:
	@rm -f *.o
	@rm -f ../combinaisons/src/*.o
	@rm -f main
