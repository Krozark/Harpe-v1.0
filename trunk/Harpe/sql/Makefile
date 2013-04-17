all: main

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

main: main.cpp config.hpp SqlObject.hpp
	$(CC) $^ $(FLAGS) -o $@


clean:
	@rm -f *.o
	@rm -f main
