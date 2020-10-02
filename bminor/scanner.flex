%{
#include "token.h"
%}
DIGIT  [0-9]
LETTER [a-zA-Z]
%%
(" "|\t|\n|\r)  	                                                        /* skip whitespace */
\/\/.*                                                                    /* skip c++ style comments */
\/\*((\*[^\/])|\/)?([^\*^\/]|(\*[^\/])|([^\*]\/))*(\*|([^\*]\/))?\*\/     /* skip c-style comments */
array              { return TOKEN_ARRAY; }
boolean            { return TOKEN_BOOLEAN; }
char               { return TOKEN_CHAR; }
else               { return TOKEN_ELSE; }
false              { return TOKEN_FALSE; }
for                { return TOKEN_FOR; }
function           { return TOKEN_FUNCTION; }
if                 { return TOKEN_IF; }
integer            { return TOKEN_INTEGER; }
print              { return TOKEN_PRINT; }
return             { return TOKEN_RETURN; }
string             { return TOKEN_STRING; }
true               { return TOKEN_TRUE; }
void               { return TOKEN_VOID; }
while              { return TOKEN_WHILE; }
({LETTER}|_){1}({LETTER}|{DIGIT}|_)*      {return TOKEN_IDENT; }
[-\+]?{DIGIT}+                            { return TOKEN_INT;}
\'([^'\\]|\\(.|\n))+\'                    { return TOKEN_CHAR_LITERAL; }
\"([^"\\]|\\(.|\n))*\"                    { return TOKEN_STRING_LITERAL; }
\(                 { return TOKEN_LPAREN; }
\)                 { return TOKEN_RPAREN; }
\[                 { return TOKEN_LBRACKET; }
\]                 { return TOKEN_RBRACKET; }
\+\+               { return TOKEN_INCR; }
\+                 { return TOKEN_PLUS; }
--                 { return TOKEN_DECR; }
-                  { return TOKEN_MINUS; }
\<                 { return TOKEN_LT; }
\<=                { return TOKEN_LE; }
\>                 { return TOKEN_GT; }
\>=                { return TOKEN_GE; }
==                 { return TOKEN_EQ; }
!=                 { return TOKEN_NE; }
&&                 { return TOKEN_LOGIC_AND; }
\|\|               { return TOKEN_LOGIC_OR; }
=                  { return TOKEN_ASSIGN; }
!                  { return TOKEN_LOGIC_NOT; }
\^                 { return TOKEN_EXPONENT; }
\*                 { return TOKEN_MULT; }
\/                 { return TOKEN_DIVISION; }
\%                 { return TOKEN_MOD; }
:                  { return TOKEN_COLON; }
,                  { return TOKEN_COMMA; }
;                  { return TOKEN_SEMICOLON; }
\{                 { return TOKEN_LBRACE; }
\}                 { return TOKEN_RBRACE;}
(\/\*(^(\*\/))*)|(([^\/^\*^\"])*\*\/)                                        { return TOKEN_UNMATCHED_COMMENT; }
.          				 { return TOKEN_ERROR; }
%%
int yywrap() { return 1; }
