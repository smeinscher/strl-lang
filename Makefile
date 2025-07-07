CC = clang

CFLAGS = -g -Wall

TARGET = strllang

all: final

final: main.o lexer.o
	$(CC) $(CFLAGS) main.o lexer.o -o $(TARGET)

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o main.o

lexer.o: src/lexer.c src/lexer.h
	$(CC) $(CFLAGS) -c src/lexer.c -o lexer.o

clean:
	rm main.o lexer.o strllang strllang.ilk strllang.pdb
