GXX = g++
GDB = gdb
LIB = -lsfml-graphics -lsfml-window -lsfml-system

default: game-app.exe

game-app.exe:
	$(GXX) -c ./Sources/*.cpp -I "./SFML-2.5.1/include/"
	$(GXX) *.o -o "./bin/game-app" -L "./SFML-2.5.1/lib/" $(LIB) 
	rm *.o

clear:
	rm *.o game-app

debug:
	$(GXX) -g ./Sources/*.cpp -I "./SFML-2.5.1/include/"
	$(GDB) ./bin/game-app
