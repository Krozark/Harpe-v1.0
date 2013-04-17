CC = g++
FLAGS = -g -std=c++0x

.cpp.o:
	$(CC) $(FLAGS) -o $@ -c $^

main: main.o
	$(CC) $(FLAGS) $^ -o $@

clean : 
	@rm main
	@rm *.o
