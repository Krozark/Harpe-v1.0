CC = g++
FLAGS = -g 

.cpp.o:
	$(CC) -c $(FLAGS) -o "$@" "$<"

.c.o:
	$(CC) -c $(FLAGS) -o "$@" "$<"

main: main.cpp defines.hpp AA_tab.o AA.o
	$(CC) $(FLAGS) -o $@ $^

clean:
	rm -f *.o
	rm -f main
