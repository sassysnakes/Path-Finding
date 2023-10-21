CFLAGS = -std=c++17 -O2
LDFLAGS =

out: *.cpp *.h
	g++ $(CFLAGS) -o main *.cpp $(LDFLAGS) -lSDL2

.PHONY: test clean

test: main
	./main

clean:
	rm -f main
