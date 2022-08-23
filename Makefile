CC=clang++
FLAGS=-std=c++20 -Wall -Wextra
LIB=-Iinc -lsfml-system -lsfml-network -lstdc++
OUT=build/message-server
CCMD=

main:
	${CC} ${LIB} ${FLAGS} -o ${OUT} src/*.cpp

debug:
	${CC} ${LIB} ${FLAGS} -o ${OUT} src/*.cpp -g -v

clean:
	rm build/*
