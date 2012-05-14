all: parser

OBJS = brainf.tab.o \
			 codegen.o \
			 main.o \
			 lex.o \

cl: clean

clean:
	@$(RM) -rf $(OBJS) lex.cpp lex brainf.tab.cpp brainf.tab.hpp parser

brainf.tab.hpp: brainf.tab.cpp

lex.cpp: lex.l brainf.tab.hpp
	flex -o $@ $^

lex: lex.cpp
	g++ -D LEX -o $@ $^

brainf.tab.cpp: brainf.y
	bison -d -o $@ $^

%.o: %.cpp
	g++ -o $@ -c $<

parser: $(OBJS)
	g++ -o $@ $(OBJS)

t.expr:
	echo ".+..." | ./parser

t.out:
	echo "," | ./parser

t.in:
	echo "." | ./parser

t.loop:
	echo ".[..[.]]" | ./parser
