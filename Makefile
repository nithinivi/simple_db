CC =clang
CFLAGS=
INCLDS=-I./include

SRC:=$(shell find ./src -name '*.c')
OBJ:=$(SRC:.c=.o)


TRAGET=build/db


all: $(OBJ)
	@mkdir -p build
	$(CC) $(OBJ) -o  $(TRAGET) 
	@mv $(OBJ) build	
	
./src/%.o: %.c
	$(CC) -c $(CFLAGS) $^

clean: 
	rm $(OBJ)
	rm -rf build/*

test:
	bundle exec rspec spec/test_spec.rb --format doc
	