// Grammar Specification (MyLanguage.g4 for ANTLR)
grammar MyLanguage;

program: statement+;

statement: ifStatement
         | elifStatement
         | whileStatement
         | doWhileStatement
         | forStatement
         | returnStatement
         | breakStatement
         | continueStatement
         | switchStatement
         | tryCatchStatement
         | exprStatement
         | functionDeclaration
         | classDeclaration
         | variableDeclaration;

ifStatement: 'if' expr 'then' statement ('else' statement)?;
elifStatement: 'elif' expr 'then' statement;
whileStatement: 'while' expr 'do' statement;
doWhileStatement: 'do' statement 'while' expr ';';
forStatement: 'for' '(' variableDeclaration expr ';' expr ')' statement;
returnStatement: 'return' expr? ';';
breakStatement: 'break' ';';
continueStatement: 'continue' ';';
switchStatement: 'switch' '(' expr ')' '{' (caseStatement | defaultStatement)* '}';
caseStatement: 'case' expr ':' statement*;
defaultStatement: 'default' ':' statement*;
tryCatchStatement: 'try' statement 'catch' '(' IDENTIFIER ')' statement ('finally' statement)?;

variableDeclaration: type IDENTIFIER ('=' expr)? ';';
functionDeclaration: type IDENTIFIER '(' parameterList? ')' block;
classDeclaration: 'class' IDENTIFIER '{' (variableDeclaration | functionDeclaration)* '}';

type: 'int' | 'float' | 'double' | 'bool' | 'char' | 'string' | 'void';

parameterList: parameter (',' parameter)*;
parameter: type IDENTIFIER;

block: '{' statement* '}';

exprStatement: expr ';';

expr: expr ('==' | '!=' | '<' | '>' | '<=' | '>=') expr
    | expr ('+' | '-' | '*' | '/' | '%') expr
    | expr ('&&' | '||') expr
    | expr '++'
    | expr '--'
    | IDENTIFIER
    | NUMBER
    | FLOAT_LITERAL
    | STRING_LITERAL
    | CHAR_LITERAL
    | 'true' | 'false'
    | '(' expr ')';

IF: 'if';
ELIF: 'elif';
ELSE: 'else';
WHILE: 'while';
DO: 'do';
FOR: 'for';
RETURN: 'return';
BREAK: 'break';
CONTINUE: 'continue';
SWITCH: 'switch';
CASE: 'case';
DEFAULT: 'default';
TRY: 'try';
CATCH: 'catch';
FINALLY: 'finally';
INT: 'int';
FLOAT: 'float';
DOUBLE: 'double';
BOOL: 'bool';
CHAR: 'char';
STRING: 'string';
VOID: 'void';
TRUE: 'true';
FALSE: 'false';
IDENTIFIER: [a-zA-Z_][a-zA-Z0-9_]*;
NUMBER: [0-9]+;
FLOAT_LITERAL: [0-9]*'.'[0-9]+;
STRING_LITERAL: '"'([^"\\]|\\.)*'"';
CHAR_LITERAL: '\''([^'\\]|\\.)*'\'';
EQ: '==';
NEQ: '!=';
LE: '<=';
GE: '>=';
LT: '<';
GT: '>';
ASSIGN: '=';
PLUS: '+';
MINUS: '-';
MUL: '*';
DIV: '/';
MOD: '%';
AND: '&&';
OR: '||';
NOT: '!';
INC: '++';
DEC: '--';
ARROW: '->';
DOT: '.';
LPAREN: '(';
RPAREN: ')';
LBRACE: '{';
RBRACE: '}';
LBRACKET: '[';
RBRACKET: ']';
SEMICOLON: ';';
COMMA: ',';
WS: [ \t\n\r]+ -> skip;
