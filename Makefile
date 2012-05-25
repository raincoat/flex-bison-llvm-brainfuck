all: clean parser

OBJS = brainf.tab.o \
			 codegen.o \
			 main.o \
			 lex.o \

LLVM_MODULES = core jit native

CPPFLAGS = `llvm-config --cppflags $(LLVM_MODULES)`
LDFLAGS = `llvm-config --ldflags $(LLVM_MODULES)`
LIBS = `llvm-config --libs $(LLVM_MODULES)`

cl: clean

m: parser

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
	g++ -o $@ ${CPPFLAGS} -c $<

parser: $(OBJS)
	g++ -o $@ $(LDFLAGS) $(OBJS) $(LIBS) -ldl -pthread

t.expr:
	echo "++++," | ./parser

t.out:
	echo "." | ./parser

t.in:
	echo "," | ./parser

t.loop:
	echo "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>." | ./parser
