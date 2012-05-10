%{
    #include "node.hpp"
    #include <cstdio>
    #include <cstdlib>
    NBlock *programBlock;
    extern int yylex();
    void yyerror(const char *s) { printf("Error: %s\n", s);  }
%}

%union {
    NBlock *block;
    std::string *string;
    int token;
}

%token <token> CHAR_INC CHAR_DEC POINT_INC POINT_DEC PUT_CHAR GET_CHAR
%token <token> JUM_BEGIN JUM_END

%type <block> stmts



%start program

%%
program: stmts {programBlock = $1;}
       ;

stmts: stmt {$$ = new NBlock(); }
     | stmts stmt
     ;

stmt: factor
    | loop
    ;

loop: JUM_BEGIN stmts JUM_END
     | JUM_BEGIN JUM_END
     ;

factor: point_op
      | char_op
      | PUT_CHAR
      | GET_CHAR
      ;

point_op: POINT_INC
        | POINT_DEC
        ;

char_op: CHAR_DEC
       | CHAR_INC
       ;
%%
