// Lexer Specification (lexer.l for YACC/Bison)
%{
#include "parser.tab.h"
%}

%%

[ \t\n]+                      ;  // Skip whitespace

// Keywords
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
"union"                       return UNION;
"this"                        return THIS;
"virtual"                     return VIRTUAL;
"override"                    return OVERRIDE;
"dynamic_cast"                return DYNAMIC_CAST;
"reinterpret_cast"            return REINTERPRET_CAST;
"static_cast"                 return STATIC_CAST;
"const_cast"                  return CONST_CAST;
"nullptr"                     return NULLPTR;
"unsigned"                    return UNSIGNED;
"signed"                      return SIGNED;
"long"                        return LONG;
"short"                       return SHORT;
"register"                    return REGISTER;
"inline"                      return INLINE;
"explicit"                    return EXPLICIT;
"typename"                    return TYPENAME;
"decltype"                    return DECLTYPE;
"thread_local"                return THREAD_LOCAL;
"alignas"                     return ALIGNAS;
"alignof"                     return ALIGNOF;
"char16_t"                    return CHAR16_T;
"char32_t"                    return CHAR32_T;
"wchar_t"                     return WCHAR_T;
"noexcept"                    return NOEXCEPT;
"static_assert"               return STATIC_ASSERT;
"concept"                     return CONCEPT;

// Comments
"**"([^*\n]|(\*+[^*\n]))*"**" return COMMENT;  // Double-line comment
"**"([^*\n]|(\*+[^*\n]))*"*\n" return COMMENT;  // Single-line comment
"#"([^*\n])*"\n"              return COMMENT;  // Single-line comment with #

[a-zA-Z_][a-zA-Z0-9_]*        return IDENTIFIER;
[0-9]+"."[0-9]+([eE][+-]?[0-9]+)? { yylval.fval = atof(yytext); return FLOAT_LITERAL; }
[0-9]+"."[0-9]+               { yylval.fval = atof(yytext); return FLOAT_LITERAL; }
[0-9]+([eE][+-]?[0-9]+)?      { yylval.ival = atoi(yytext); return NUMBER; }

// Operators
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
"~"                           return BITWISE_NOT;
"&"                           return BITWISE_AND;
"|"                           return BITWISE_OR;
"^"                           return BITWISE_XOR;
"<<"                          return LSHIFT;
">>"                          return RSHIFT;
"+="                          return PLUS_ASSIGN;
"-="                          return MINUS_ASSIGN;
"*="                          return MUL_ASSIGN;
"/="                          return DIV_ASSIGN;
"%="                          return MOD_ASSIGN;
"&="                          return AND_ASSIGN;
"|="                          return OR_ASSIGN;
"^="                          return XOR_ASSIGN;
"<<="                         return LSHIFT_ASSIGN;
">>="                         return RSHIFT_ASSIGN;
"&&="                         return AND_ASSIGN;
"||="                         return OR_ASSIGN;
"?:"                          return TERNARY;

// Literals
"\""([^"\\]|\\.)*\""          return STRING_LITERAL;
"'"([^'\\]|\\.)*'"            return CHAR_LITERAL;

.                             return yytext[0];

%%

int main(void) {
  yylex();
  return 0;
}

int yywrap() {
  return 1;
}
