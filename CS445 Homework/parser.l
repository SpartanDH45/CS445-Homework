%{
// Donald Hammer CS445
// Adapted code from provided calculator program

// this *order* of these includes is mandatory
#include "scanType.h"  // TokenData Type
#include "parser.tab.h"  // token definitions from the bison

int line = 1;
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
        printf("Line %i Token: ID Value: %s\n", linenum, svalue);
    }
    else if (tokenClass == NUMCONST) {
        yylval.tokenData->numValue = atoi(svalue); 
        printf("Line %i Token: NUMCONST Value: %i Input %s\n", linenum, atoi(svalue), svalue);
    }
    else if (tokenClass == CHARCONST) {
        if (strlen(svalue) > 3){
            if(strlen(svalue) > 4 || svalue[1] != '\\'){
                printf("WARNING(%i): character is %i characters long and not a single character: '%s'.  The first char will be used.", linenum, strlen(svalue)-2, svalue);
            }
            if(svalue[1] == '\\'){
                if(svalue[2] == 'n'){
                    printf("Line %i Token: CHARCONST Value: '\\n' Input: %s", linenum, svalue);
                    yylval.tokenData->charValue = '\n';
                } else if (svalue[2] == '0'){
                    printf("Line %i Token: CHARCONST Value: '\\0' Input: %s", linenum, svalue);
                    yylval.tokenData->charValue = '\0';
                } else {
                    printf("Line %i Token: CHARCONST Value: '%c' Input: %s", linenum, svalue[2], svalue);
                    yylval.tokenData->charValue = svalue[2];
                }
            } else {
                printf("Line %i Token: CHARCONST Value: '%c' Input: %s", linenum, svalue[1], svalue);
                yylval.tokenData->charValue = svalue[1];
            }
        }
        yylval.tokenData->charValue = svalue[0];
        printf("Line %i Token: CHARCONST Value: '%s' Input: '%s'", linenum);
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
[\+\-\*\/\(\)\[\]\<\>\%\?\,\:\;\=] {return setValue(line, yytext[0], yytext);}
[ \t]                  ;
\n                     { line++; }
.                      { printf("ERROR(%i): Invalid or misplaced input character: \'%c\' Character Ignored.\n", line, yytext[0]); numErrors++; }
%%
