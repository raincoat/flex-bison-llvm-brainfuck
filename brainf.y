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
    NStmt *stmt;
    NLoop *loop;
    std::string *string;
    int token;
}

%token <token> CHAR_INC CHAR_DEC POINT_INC POINT_DEC PUT_CHAR GET_CHAR
%token <token> JUM_BEGIN JUM_END

%type <block> stmts
%type <stmt> factor point_op char_op
%type <loop> loop



%start program

%%
program: stmts {programBlock = $1;}
       ;

stmts: stmt {$$ = new NBlock(); $$->stmtList.push_back($<stmt>1);}
     | stmts stmt {$$->stmtList.push_back($<stmt>2);}
     ;

stmt: factor
    | loop
    ;

loop: JUM_BEGIN stmts JUM_END {$$ = new NLoop(*$2);}
    ;

factor: point_op
      | char_op
      | PUT_CHAR {$$ = new NOut();}
      | GET_CHAR {$$ = new NInp();}
      ;

point_op: POINT_INC {$$ = new NPosOp($1);}
        | POINT_DEC {$$ = new NPosOp($1);}
        ;

char_op: CHAR_DEC {$$ = new NValOp($1);}
       | CHAR_INC {$$ = new NValOp($1);}
       ;
%%
