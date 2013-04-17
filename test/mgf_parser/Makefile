CC = g++
INCPATH = 
LIBS = 
FLAGS = -g $(INCPATH) $(LIBS)


main: main.cpp parser.o parser.lex.c
	$(CC) $(FLAGS) -o $@ $^

parser.lex.c: parser.lex defines.hpp
	flex -t parser.lex > $@

.cpp.o:
	$(CC) -c $(FLAGS) -o "$@" "$<"

.c.o:
	$(CC) -c $(FLAGS) -o "$@" "$<"

clean:
	rm -f *.o
	rm -f main
	rm -f parser.lex.c
