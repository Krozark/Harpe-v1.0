all: $(OBJ) serveur.exe local.exe

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<" 
	@cp $@ $(OBJ_DIR)

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

local.exe: local-main.cpp Socket.o Serializer.o
	$(CC) $^ $(FLAGS) -o $@

serveur.exe: serveur-main.cpp Socket.o Serializer.o
	$(CC) $^ $(FLAGS) -o $@

Serializer.o : Serializer.cpp Serializer.hpp
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

Socket.o: Socket.cpp Socket.hpp
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

clean:
	@rm -f *.o
	@rm -f *.exe
