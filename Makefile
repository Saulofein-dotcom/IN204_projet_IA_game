GXX = g++
LIB = -lsfml-graphics -lsfml-window -lsfml-system

default: game-app.exe

game-app.exe:
	$(GXX) -c ./Sources/*.cpp
	$(GXX) *.o -o game-app $(LIB)
	rm *.o

clear:
	rm *.o game-app