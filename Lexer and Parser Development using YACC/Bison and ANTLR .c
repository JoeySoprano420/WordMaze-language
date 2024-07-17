// Lexer Specification (lexer.l for YACC/Bison)
%{
#include "parser.tab.h"
%}

%%

[ \t\n]+                      ;  // Skip whitespace
"if"                          return IF;
"else"                        return ELSE;
"elif"                        return ELIF;
"while"                       return WHILE;
"for"                         return FOR;
"do"                          return DO;
"return"                      return RETURN;
"int"                         return INT;
"float"                       return FLOAT;
"double"                      return DOUBLE;
"bool"                        return BOOL;
"char"                        return CHAR;
"string"                      return STRING;
"true"                        return TRUE;
"false"                       return FALSE;
"void"                        return VOID;
"break"                       return BREAK;
"continue"                    return CONTINUE;
"switch"                      return SWITCH;
"case"                        return CASE;
"default"                     return DEFAULT;
"import"                      return IMPORT;
"export"                      return EXPORT;
"namespace"                   return NAMESPACE;
"class"                       return CLASS;
"struct"                      return STRUCT;
"public"                      return PUBLIC;
"private"                     return PRIVATE;
"protected"                   return PROTECTED;
"static"                      return STATIC;
"final"                       return FINAL;
"abstract"                    return ABSTRACT;
"new"                         return NEW;
"delete"                      return DELETE;
"try"                         return TRY;
"catch"                       return CATCH;
"finally"                     return FINALLY;
"throw"                       return THROW;
"async"                       return ASYNC;
"await"                       return AWAIT;
"const"                       return CONST;
"constexpr"                   return CONSTEXPR;
"sizeof"                      return SIZEOF;
"typedef"                     return TYPEDEF;
"using"                       return USING;
"template"                    return TEMPLATE;
"decltype"                    return DECLTYPE;
"auto"                        return AUTO;
"enum"                        return ENUM;
"extern"                      return EXTERN;
"goto"                        return GOTO;
"inline"                      return INLINE;
"friend"                      return FRIEND;
"mutable"                     return MUTABLE;
"volatile"                    return VOLATILE;
"operator"                    return OPERATOR;
[a-zA-Z_][a-zA-Z0-9_]*        return IDENTIFIER;
[0-9]+                        return NUMBER;
[0-9]*"."[0-9]+               return FLOAT_LITERAL;
"=="                          return EQ;
"!="                          return NEQ;
"<="                          return LE;
">="                          return GE;
"<"                           return LT;
">"                           return GT;
"="                           return ASSIGN;
"+"                           return PLUS;
"-"                           return MINUS;
"*"                           return MUL;
"/"                           return DIV;
"%"                           return MOD;
"&&"                          return AND;
"||"                          return OR;
"!"                           return NOT;
"++"                          return INC;
"--"                          return DEC;
"->"                          return ARROW;
"."                           return DOT;
"("                           return LPAREN;
")"                           return RPAREN;
"{"                           return LBRACE;
"}"                           return RBRACE;
"["                           return LBRACKET;
"]"                           return RBRACKET;
";"                           return SEMICOLON;
","                           return COMMA;
"\""([^"\\]|\\.)*\""          return STRING_LITERAL;
"'"([^'\\]|\\.)*'"            return CHAR_LITERAL;
.                             return yytext[0];

%%

int main(void) {
  yylex();
  return 0;
}
