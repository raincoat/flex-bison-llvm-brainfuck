%{
  #ifndef LEX
    #include "node.hpp"
    #include "brainf.tab.hpp"
    #define TOKEN(t) (yylval.token = t)
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
  extern "C" int yywrap() { }
%}


%%
"+"  { return TOKEN(CHAR_INC); }
"-"  { return TOKEN(CHAR_DEC); }
">"  { return TOKEN(POINT_INC); }
"<"  { return TOKEN(POINT_DEC); }
"."  { return TOKEN(PUT_CHAR); }
","  { return TOKEN(GET_CHAR); }
"["  { return TOKEN(JUM_BEGIN); }
"]"  { return TOKEN(JUM_END); }
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
