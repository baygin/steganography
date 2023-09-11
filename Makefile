exec = steganography
sources = $(wildcard src/*.c)
includes = $(wildcard include/*.h)
objects = $(sources:.c=.o)
flags = -g -w -fPIC -std=c99 -lm

$(exec): $(objects)
	gcc $(objects) $(flags) -o ./bin/$(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

clean:
	-rm -rf ./bin/*
	-rm -rf ./src/*.o

run: 
	./bin/$(exec)

lineof:
	git ls-files | grep '\.c\|\.h' | xargs wc -l
