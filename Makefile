all:
	gcc main.c main.h -o main -lm

clean:
	rm main

run:
	./main
