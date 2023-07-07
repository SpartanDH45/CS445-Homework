%{
#include "scanType.h"
#include "treeNodes.h"
#include "parser.tab.h" //This has to be last

using namespace std;

#define YY_DECL extern "C" int yylex()
#define YYSTYPE int

char *lastToken=(char *)"";

int line=1;

int setValue(int linenum, int tokenClass, char *svalue) {
    yylval.tokenData = new TokenData;
    yylval.tokenData->tokenclass = tokenClass;
    yylval.tokenData->linenum = line;
    yylval.tokenData->tokenstr =strdup(svalue);
    yylval.tokenData->cvalue = svalue[0];
    //May cause an overflow error
    yylval.tokenData->nvalue = atoi(svalue);
    yylval.tokenData->svalue = strdup(svalue);
    switch(tokenClass){
        case STRINGCONST:
            yylval.tokenData0>nvalue-strlen(svalue)-2;
            for(int i = 0; i < strlen(svalue)-1; i++){
                yylval.tokenData->svalue[i] = yylval.tokenData->svalue[i+1];
            }
            break;
        case BOOLCONST:
            if(svalue[0]=='t'){yylval.tokenData->nvalue = 1;}
            else{yylval.tokenData->nvalue=0;}
            break;
        case CHARCONST:
            yylval.tokenData->cvalue=svalue[1];
            if(svalue[1]=='\\'){
                if(svalue[2]=='n'){yylval.tokenData->cvalue='\n';}
                if(svalue[2]=='0'){yylval.tokenData->cvalue='\0';}
            }
    }
    return tokenClass;
}

%}

%option noyywrap 


letter		[a-zA-Z]
digit		[0-9]
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
{op}		{ return setValue(line, yytext[0], yytext); }
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
.     		{ printf("TOKEN ERROR(%d): invalid or misplaced input character: \'%c\'. Character Ignored.\n", line, yytext[0]); /*numErrors++;*/}

%%
