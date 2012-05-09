%{
    #define YYSTYPE double
    #include <stdio.h>
    extern int yylex();
    void yyerror(const char *s) { printf("Error: %s\n", s);  }
%}

%token CHAR_INC CHAR_DEC POINT_INC POINT_DEC PUT_CHAR GET_CHAR
%token JUM_BEGIN JUM_END

%start program

%%
program: stmts
       ;

stmts: expr
     | expr loop
     | loop
     ;

loop: JUM_BEGIN expr JUM_END
    | JUM_BEGIN JUM_END
    ;


expr: char
    | expr char
    ;

char: point_op
    | char_op
    | put_char
    | get_char
    ;

point_op: POINT_INC
        | POINT_DEC
        ;

char_op: CHAR_DEC
       | CHAR_INC
       ;

put_char: PUT_CHAR

get_char: GET_CHAR

%%

int main (int argc, char** argv) {
    yyparse();
    return 0;
}
