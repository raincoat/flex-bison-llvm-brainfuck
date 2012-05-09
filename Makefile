all: parser

OBJS = lex.o

cl: clean

clean:
	$(RM) -rf OBJS lex.cpp lex

lex.cpp: lex.l
	flex -o $@ $^

lex: lex.cpp
	g++ -D LEX -o $@ $^ -lfl

parser: $(OBJS)
	g++ -o $@
