all: $(OBJ) serveur$(EXTENTION) local$(EXTENTION) main

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<" 
	@cp $@ $(OBJ_DIR)

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

main : local.o serveur.o Socket/src/Socket.o Socket/src/Serializer.o Socket/src/SocketSerialized.o Socket/src/SelectManager.o

local$(EXTENTION): local-main.cpp Socket/src/Socket.o Socket/src/Serializer.o Socket/src/SocketSerialized.o Socket/src/SelectManager.o
	$(CC) $^ $(FLAGS) -o $@

serveur$(EXTENTION): serveur-main.cpp Socket/src/Socket.o Socket/src/Serializer.o Socket/src/SocketSerialized.o Socket/src/SelectManager.o
	$(CC) $^ $(FLAGS) -o $@

clean:
	@rm -f *.o
	@rm -f Socket/src/*.o
	@rm -f local$(EXTENTION)
	@rm -f serveur$(EXTENTION)
