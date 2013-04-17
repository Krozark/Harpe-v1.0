all: combinatoire

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

combinatoire : main.cpp Combinaison.o
	$(CC) $^ $(FLAGS) -o $@


clean:
	@rm -f *.o
	@rm -f main
	@rm -f combinatoire
