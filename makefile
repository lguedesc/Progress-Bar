CC=clang
CSTD=c17
NAME=test
FILES=$(NAME).c progress_monitor.c

all:
	@$(CC) -std=$(CSTD) -fopenmp -o $(NAME) $(FILES)

run: all
	@./test

clean:
	@rm -fr test