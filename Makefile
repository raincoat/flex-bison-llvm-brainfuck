all: parser

OBJS = lex.o

cl: clean

clean:
	@$(RM) -rf OBJS lex.c lex brainf.tab.c brainf.tab.h parser

lex.c: lex.l
	flex -o $@ $^

lex: lex.c
	gcc -D LEX -o $@ $^ -lfl

brainf.tab.c: brainf.y
	bison -d -o $@ $^

parser: brainf.tab.c lex.c
	gcc -o $@ $^ -lfl
