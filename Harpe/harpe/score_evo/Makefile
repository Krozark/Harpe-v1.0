all: main.exe

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<" 
	@cp $@ $(OBJ_DIR)

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

main.exe: main.cpp IndividuTree.o IndividuLinear.hpp Engine/src/GeneticEngine.hpp Engine/src/GeneticThread.hpp
	#$(CC) $^ $(FLAGS) -o $@


clean:
	@rm -f *.o
	@rm -f Engine/src/*.o
	@rm -f main.exe
