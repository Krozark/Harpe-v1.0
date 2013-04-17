all: serveur local

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<" 
	@cp $@ $(OBJ_DIR)

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

local: local.o local.hpp 
	#$(CC) $^ $(FLAGS) -o $@

serveur: serveur.o serveur.hpp Socket.o
	#$(CC) $^ $(FLAGS) -o $@


clean:
	@rm -f *.o
	@rm -f main
