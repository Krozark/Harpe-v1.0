all: serveur local main

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<" 
	@cp $@ $(OBJ_DIR)

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

main : local.o serveur.o Socket.o

local: local-main.cpp Socket.o
	$(CC) $^ $(FLAGS) -o $@

serveur: serveur-main.cpp Socket.o
	$(CC) $^ $(FLAGS) -o $@


clean:
	@rm -f *.o
	@rm -f local
	@rm -f serveur
