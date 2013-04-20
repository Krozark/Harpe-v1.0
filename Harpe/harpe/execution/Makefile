all: serveur.exe local.exe main

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<" 
	@cp $@ $(OBJ_DIR)

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

main : local.o serveur.o Socket.o Serializer.o

local.exe: local-main.cpp Socket.o Serializer.o
	$(CC) $^ $(FLAGS) -o $@

serveur.exe: serveur-main.cpp Socket.o Serializer.o
	$(CC) $^ $(FLAGS) -o $@


clean:
	@rm -f *.o
	@rm -f *.exe
