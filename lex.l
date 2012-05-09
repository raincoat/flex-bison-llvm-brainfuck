%{
  #define YYSTYPE double
  #ifndef LEX
    #include "brainf.tab.h"
  #endif

  #ifdef LEX
    enum yytokentype{
      CHAR_INC = 258,
      CHAR_DEC = 259,
      POINT_INC = 260,
      POINT_DEC = 261,
      PUT_CHAR = 262,
      GET_CHAR = 263,
      JUM_BEGIN = 264,
      JUM_END = 265
    };
    YYSTYPE yylval;
  #endif
%}

%%
"+"  { return CHAR_INC; }
"-"  { return CHAR_DEC; }
">"  { return POINT_INC; }
"<"  { return POINT_DEC; }
"."  { return PUT_CHAR; }
","  { return GET_CHAR; }
"["  { return JUM_BEGIN; }
"]"  { return JUM_END; }
%%

#ifdef LEX
int main (int argc, char** argv) {
  int token;

  while (token = yylex()) {
    printf("%d", token);
  }

  return 0;
}
#endif
