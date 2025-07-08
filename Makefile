CC = clang

CFLAGS = -g -Wall

TARGET = strllang.exe

all: final

final: main.o lexer.o parser.o emitter.o
	$(CC) $(CFLAGS) main.o lexer.o parser.o emitter.o -o $(TARGET)

main.o: src/main.c
	$(CC) $(CFLAGS) -c src/main.c -o main.o

lexer.o: src/lexer.c src/lexer.h
	$(CC) $(CFLAGS) -c src/lexer.c -o lexer.o

parser.o: src/parser.c src/parser.h
	$(CC) $(CFLAGS) -c src/parser.c -o parser.o

emitter.o: src/emitter.c src/emitter.h
	$(CC) $(CFLAGS) -c src/emitter.c -o emitter.o

clean:
	rm main.o lexer.o parser.o emitter.o strllang strllang.ilk strllang.pdb
