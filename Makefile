client:	client_main.cpp lib
	g++ -o client client_main.cpp -L. -lLibrary

lib:	Chat.cpp Chat.h sha1.cpp sha1.h User.cpp User.h Functions.h
	g++ -o ChatLib.o Chat.cpp -c
	g++ -o sha1Lib.o sha1.cpp -c
	g++ -o UserLib.o User.cpp -c
	ar rc libLibrary.a ChatLib.o sha1Lib.o UserLib.o

clean:
	rm -rf *.o *.a
