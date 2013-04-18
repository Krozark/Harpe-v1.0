all: main.exe

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

main.exe: main.cpp parser.lex.o parser.o
	$(CC) $^ $(FLAGS) -o $@

parser.lex.o : parser.lex.c
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)


parser.lex.c: parser.lex defines.hpp
	flex -t parser.lex > $@


clean:
	@rm -f *.o
	@rm -f main.exe
	@rm -f parser.lex.c
