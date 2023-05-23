%{
#include "scanType.h"
#include "parser.tab.h"

using namespace std;

#define YY_DECL extern "C" int yylex()
#define YYSTYPE int

char *lastToken=(char *)"";

int line=1;

int setValue(int linenum, int tokenClass, char *svalue) {
    yylval.tinfo.tokenclass = tokenClass;
    yylval.tinfo.linenum = linenum;
    yylval.tinfo.tokenstr = strdup(svalue);
    yylval.tinfo.cvalue = '@';
    yylval.tinfo.nvalue = 777;
    yylval.tinfo.svalue = NULL;
    return tokenClass;
}

%}

%option noyywrap 


letter		[a-zA-Z]
digit		[0-9]
quote		[\']
underbar	[_]
letdig		{digit}|{letter}|{underbar}
limitters	[\;\,\(\)\{\}\[\]]
mathop		[\-\+\*\/\%]
compop		[\:\?\<\>\=]
op		{limitters}|{mathop}|{compop}
whitespace	[\ \t]
id		{letter}{letdig}*
numconst	{digit}+
charconst	\'\\?.\'
charerr		\'..\'
stringconst	\"([^"]|\\\")*\"
comment		"//".*

%%

"and"		{ return setValue(line, AND, yytext); }
"bool"		{ return setValue(line, BOOL, yytext); }
"break"		{ return setValue(line, BREAK, yytext); }
"by"		{ return setValue(line, BY, yytext); }
"char"		{ return setValue(line, CHAR, yytext); }
"do"		{ return setValue(line, DO, yytext); }
"else"		{ return setValue(line, ELSE, yytext); }
"false"		{ return setValue(line, BOOLCONST, yytext); }
"for"		{ return setValue(line, FOR, yytext); }
"if"		{ return setValue(line, IF, yytext); }
"int"		{ return setValue(line, INT, yytext); }
"not"		{ return setValue(line, NOT, yytext); }
"or"		{ return setValue(line, OR, yytext); }
"return"	{ return setValue(line, RETURN, yytext); }
"static"	{ return setValue(line, STATIC, yytext); }
"then"		{ return setValue(line, THEN, yytext); }
"to"		{ return setValue(line, TO, yytext); }
"true"		{ return setValue(line, BOOLCONST, yytext); }
"while"		{ return setValue(line, WHILE, yytext); }
{op}		{ return setValue(line, OP, yytext); }
"=="		{ return setValue(line, EQ, yytext); }
"<="		{ return setValue(line, LEQ, yytext); }
">="		{ return setValue(line, GEQ, yytext); }
":<:"		{ return setValue(line, MIN, yytext); }
":>:"		{ return setValue(line, MAX, yytext); }
"!="		{ return setValue(line, NEQ, yytext); }
"+="		{ return setValue(line, ADDASS, yytext); }
"-="		{ return setValue(line, SUBASS, yytext); }
"*="		{ return setValue(line, MULASS, yytext); }
"/="		{ return setValue(line, DIVASS, yytext); }
"++"		{ return setValue(line, INC, yytext); }
"--"		{ return setValue(line, DEC, yytext); }
{id}		{ return setValue(line, ID, yytext); }
{numconst}      { return setValue(line, NUMCONST, yytext); }
{charconst}	{ return setValue(line, CHARCONST, yytext); }
{charerr}	{ return setValue(line, CHARCONST, yytext); }
{stringconst}	{ return setValue(line, STRINGCONST, yytext); }
^"#DRBC ".*	{ return setValue(line, PRECOMPILER, yytext); }
^"#DRBCRUN ".*	{ return setValue(line, PRECOMPILER, yytext); }
\n       	{ line++; }
{comment}	;
{whitespace}+   ;
.     		{ yylval.tinfo.tokenstr = yytext; return ERROR; }

%%
