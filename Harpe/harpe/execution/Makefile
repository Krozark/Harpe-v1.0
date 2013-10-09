all: $(OBJ) main serveur$(EXTENTION) local$(EXTENTION) 
#difference_masse_parent$(EXTENTION) main

.cpp.o:
	$(CC) $(FLAGS) -o "$@" -c "$<" 
	@cp $@ $(OBJ_DIR)

.c.o:
	$(CC) $(FLAGS) -o "$@" -c "$<"
	@cp $@ $(OBJ_DIR)

main : local.o serveur.o Socket/Socket/Socket.o Socket/Socket/Serializer.o Socket/Socket/SocketSerialized.o Socket/Socket/SelectManager.o

local$(EXTENTION): local-main.cpp Socket/Socket/Socket.o Socket/Socket/Serializer.o Socket/Socket/SocketSerialized.o Socket/Socket/SelectManager.o
	$(CC) $^ $(FLAGS) -o $@

serveur$(EXTENTION): serveur-main.cpp serveur.o Socket/Socket/Socket.o Socket/Socket/Serializer.o Socket/Socket/SocketSerialized.o Socket/Socket/SelectManager.o
	$(CC) $^ $(FLAGS) -o $@

#difference_masse_parent$(EXTENTION): difference_masse_parent-main.cpp Socket/src/Socket.o Socket/src/Serializer.o Socket/src/SocketSerialized.o Socket/src/SelectManager.o difference_masse_parent.o
#	$(CC) $^ $(FLAGS) -o $@

clean:
	@rm -f *.o
	@rm -f Socket/Socket/*.o
	@rm -f local$(EXTENTION)
	@rm -f serveur$(EXTENTION)
