%option caseless

%{
#include <string>
#include <cstdio>
#include <cstring>
#include "Stmt.h"
#include "y.tab.h"
char *get_text(const char *text, bool strip = false) {
    int length = strlen(text);
    char *buffer;
    if(not strip) {
        buffer = new char[length + 1];
        memcpy(buffer, text, (length + 1) * sizeof(char));
    } else {
        buffer = new char[length - 1];
        memcpy(buffer, text + 1, (length - 1) * sizeof(char));
        buffer[length - 2] = 0;
    }
    return buffer;
}

void yyerror(const char *);
%}

VALUE_INT       [0-9]+
VALUE_FLOAT     (-?[0-9]+)(\.[0-9]+)?
IDENTIFIER      [A-Za-z][_0-9A-Za-z]*
VALUE_STRING    '[^']*'
WHITESPACE      [ \t\n]

%%

","             { return yytext[0]; }
"("             { return yytext[0]; }
")"             { return yytext[0]; }
"."             { return yytext[0]; }
";"             { return yytext[0]; }
"*"             { return yytext[0]; }

QUIT            { return QUIT;}
DATABASE        { return DATABASE;}
DATABASES       { return DATABASES;}
TABLE           { return TABLE;}
TABLES          { return TABLES;}
SHOW            { return SHOW;}
CREATE          { return CREATE;}
DROP            { return DROP;}
USE             { return USE;}
PRIMARY         { return PRIMARY;}
KEY             { return KEY;}
NOT             { return NOT;}
NULL            { return KWNULL;}
INSERT          { return INSERT;}
INTO            { return INTO;}
VALUES          { return VALUES;}
DELETE          { return DELETE;}
FROM            { return FROM;}
WHERE           { return WHERE;}
UPDATE          { return UPDATE;}
SET             { return SET;}
SELECT          { return SELECT;}
IS              { return IS;}
INT             { return KWINT;}
VARCHAR         { return VARCHAR;}
DEFAULT         { return DEFAULT;}
CONSTRAINT      { return CONSTRAINT;}
CHANGE          { return CHANGE;}
ALTER           { return ALTER;}
ADD             { return ADD;}
RENAME          { return RENAME;}
DESC            { return DESC;}
INDEX           { return INDEX;}
AND             { return AND;}
DATE            { return DATE;}
FLOAT           { return KWFLOAT;}
FOREIGN         { return FOREIGN;}
REFERENCES      { return REFERENCES;}
ON              { return ON;}
TO              { return TO;}

{VALUE_INT}     { sscanf(yytext, "%d", &yylval.ivalue); return VALUE_INT; }
{VALUE_FLOAT}   { sscanf(yytext, "%f", &yylval.fvalue); return VALUE_FLOAT; }
{IDENTIFIER}    { yylval.string = get_text(yytext, false); return IDENTIFIER; }
{VALUE_STRING}  { yylval.string = get_text(yytext, false); return VALUE_STRING; }
{WHITESPACE}    {}

"="             { return EQ; }
">"             { return GT; }
"<"             { return LT; }
">="            { return GE; }
"<="            { return LE; }
"<>"            { return NE; }

.               { std::string mex = "Invalid input:"; yyerror((mex + yytext).c_str()); }

%%

int yywrap() {
    return 1;
}