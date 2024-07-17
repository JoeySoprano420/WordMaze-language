// Example: ANTLR Grammar Specification (MyLanguage.g4)
grammar MyLanguage;

program: statement+;

statement: ifStatement | exprStatement;

ifStatement: 'if' expr 'then' statement ('else' statement)?;

exprStatement: expr ';';

expr: IDENTIFIER | NUMBER;

IF: 'if';
ELSE: 'else';
IDENTIFIER: [a-zA-Z_][a-zA-Z0-9_]*;
NUMBER: [0-9]+;
WS: [ \t\n\r]+ -> skip;
