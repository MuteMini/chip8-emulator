all:
	g++ -I libs/include -L libs/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2