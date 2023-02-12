%{
// Donald Hammer CS445
// Adapted code from provided calculator program

// this *order* of these includes is mandatory
#include "scanType.h"  // TokenData Type
#include "parser.tab.h"  // token definitions from the bison

int line;
int numErrors;    // ERR err count

static int setValue(int linenum, int tokenClass, char *svalue)
{
    // create the pass-back data space
    yylval.tokenData = new TokenData;
    printf("Line %i Token: ", linenum);

    // fill it up
    yylval.tokenData->linenum = linenum;
    yylval.tokenData->tokenstr = strdup(svalue);  // duplicating string!!
    if (tokenClass == BOOLCONST){
        if (svalue == "true"){
            yylval.tokenData->numValue = 1;
        } else {
            yylval.tokenData->numValue = 0;
        }
    }
    else if (tokenClass == ID) {
        //svalue[0]-'a'
        yylval.tokenData->strValue = strdup(svalue);
        printf("ID Value: %s\n", svalue);
    }
    else if (tokenClass == NUMCONST) {
        yylval.tokenData->numValue = atoi(svalue); 
        printf(" NUMCONST Value: %i Input %s\n",atoi(svalue), svalue);
    }
    else if (tokenClass == CHARCONST) {
        yylval.tokenData->charValue = svalue[0];
    }
    else if (tokenClass == STRINGCONST) {
        yylval.tokenData->strValue = strdup(svalue);
    }
    // return the tokenclass
    return tokenClass;
}

%}

%option noyywrap

%%
"true"|"false"         { return setValue(line, BOOLCONST, yytext);}
[A-Za-z][A-Za-z0-9]*   { return setValue(line, ID, yytext);}
[0-9]*                 { return setValue(line, NUMCONST, yytext);}
"'\n'"                 { return setValue(line, CHARCONST, yytext);}
"'\0'"                 { return setValue(line, CHARCONST, yytext);}
[']([^\\'])[']         { return setValue(line, CHARCONST, yytext);}
"'\\"([.])[']          { return setValue(line, CHARCONST, yytext);}
[\"]([^\\"]*(\\[\\"]))+["] { return setValue(line, STRINGCONST, yytext);}
[ \t]                  ;
\n                     { line++; return setValue(line, yytext[0], yytext ); }
.                      { printf("ERROR(%i): Invalid or misplaced input character: \'%c\' Character Ignored.\n", line, yytext[0]); numErrors++; }
%%
