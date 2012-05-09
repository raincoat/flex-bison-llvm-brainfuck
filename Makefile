all: parser

OBJS = lex.o

cl: clean

clean:
	@$(RM) -rf OBJS lex.cpp lex brainf.tab.cpp brainf.tab.hpp parser

lex.cpp: lex.l
	flex -o $@ $^

lex: lex.cpp
	g++ -D LEX -o $@ $^ -lfl

brainf.tab.cpp: brainf.y
	bison -d -o $@ $^

parser: brainf.tab.cpp lex.cpp
	g++ -o $@ $^ -lfl
