all:
	mkdir -p ./bin
	gcc -Wall main.c methods.c -o bin/main `pkg-config --cflags --libs gtk+-3.0`

clean:
	rm -r ./bin
