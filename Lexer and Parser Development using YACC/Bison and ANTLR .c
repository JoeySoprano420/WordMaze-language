/* Example: YACC/Bison Lexer Specification (lexer.l) */
%{
#include "parser.tab.h"
%}

%%

[ \t\n]+         ;  // Skip whitespace
"if"             return IF;
"else"           return ELSE;
[a-zA-Z_][a-zA-Z0-9_]*  return IDENTIFIER;
[0-9]+           return NUMBER;
.                return yytext[0];

%%

int main(void) {
  yylex();
  return 0;
}
