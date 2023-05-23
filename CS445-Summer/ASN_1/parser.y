%{
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include "scanType.h"
using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

void yyerror(const char *msg);

void printToken(TokenData myData, string tokenName, int type = 0) {
   cout << "Line: " << myData.linenum << " Type: " << tokenName;
   if(type==0)
     cout << " Token: " << myData.tokenstr;
   if(type==1)
     cout << " Token: " << myData.nvalue;
   if(type==2)
     cout << " Token: " << myData.cvalue;
   cout << endl;
}

%}
%union
{
   struct   TokenData tinfo ;
}
%token   <tinfo>  AND
%token   <tinfo>  BOOL
%token   <tinfo>  BREAK
%token   <tinfo>  BY
%token   <tinfo>  CHAR
%token   <tinfo>  DO
%token   <tinfo>  ELSE
%token   <tinfo>  FOR
%token   <tinfo>  IF
%token   <tinfo>  INT
%token   <tinfo>  NOT
%token   <tinfo>  OR
%token   <tinfo>  RETURN
%token   <tinfo>  STATIC
%token   <tinfo>  THEN
%token   <tinfo>  TO
%token   <tinfo>  WHILE
%token   <tinfo>  BOOLCONST
%token   <tinfo>  OP
%token   <tinfo>  EQ
%token   <tinfo>  LEQ
%token   <tinfo>  GEQ
%token   <tinfo>  MIN
%token   <tinfo>  MAX
%token   <tinfo>  NEQ
%token   <tinfo>  ADDASS
%token   <tinfo>  SUBASS
%token   <tinfo>  MULASS
%token   <tinfo>  DIVASS
%token   <tinfo>  INC
%token   <tinfo>  DEC
%token   <tinfo>  ID
%token   <tinfo>  NUMCONST
%token   <tinfo>  CHARCONST
%token   <tinfo>  STRINGCONST
%token	 <tinfo>  PRECOMPILER
%token   <tinfo>  ERROR 
%type <tinfo>  term program
%%
program  :  program term
   |  term  {$$=$1;}
   ;
term  : 
     AND {printToken(yylval.tinfo, "AND");}
   |  BOOL {printToken(yylval.tinfo, "BOOL");}
   |  BREAK {printToken(yylval.tinfo, "BREAK");}
   |  BY {printToken(yylval.tinfo, "BY");}
   |  CHAR {printToken(yylval.tinfo, "CHAR");}
   |  DO {printToken(yylval.tinfo, "DO");}
   |  ELSE {printToken(yylval.tinfo, "ELSE");}
   |  FOR {printToken(yylval.tinfo, "FOR");}
   |  IF {printToken(yylval.tinfo, "IF");}
   |  INT {printToken(yylval.tinfo, "INT");}
   |  NOT {printToken(yylval.tinfo, "NOT");}
   |  OR {printToken(yylval.tinfo, "OR");}
   |  RETURN {printToken(yylval.tinfo, "RETURN");}
   |  STATIC {printToken(yylval.tinfo, "STATIC");}
   |  THEN {printToken(yylval.tinfo, "THEN");}
   |  TO {printToken(yylval.tinfo, "TO");}
   |  WHILE {printToken(yylval.tinfo, "WHILE");}
   |  BOOLCONST {printToken(yylval.tinfo, "BOOLCONST");}
   |  OP {printToken(yylval.tinfo, "OP");}
   |  EQ {printToken(yylval.tinfo, "EQ");}
   |  LEQ {printToken(yylval.tinfo, "LEQ");}
   |  GEQ {printToken(yylval.tinfo, "GEQ");}
   |  MIN {printToken(yylval.tinfo, "MIN");}
   |  MAX {printToken(yylval.tinfo, "MAX");}
   |  NEQ {printToken(yylval.tinfo, "NEQ");}
   |  ADDASS {printToken(yylval.tinfo, "ADDASS");}
   |  SUBASS {printToken(yylval.tinfo, "SUBASS");}
   |  MULASS {printToken(yylval.tinfo, "MULASS");}
   |  DIVASS {printToken(yylval.tinfo, "DIVASS");}
   |  INC {printToken(yylval.tinfo, "INC");}
   |  DEC {printToken(yylval.tinfo, "DEC");}
   |  ID {printToken(yylval.tinfo, "ID");}
   |  NUMCONST {printToken(yylval.tinfo, "NUMCONST");}
   |  CHARCONST {printToken(yylval.tinfo, "CHARCONST");}
   |  STRINGCONST {printToken(yylval.tinfo, "STRINGCONST");}
   |  PRECOMPILER {printToken(yylval.tinfo, "PRECOMPILER");}
   |  ERROR    {cout << "ERROR(" << yylval.tinfo.linenum << "): Invalid or misplaced input character: '" << yylval.tinfo.tokenstr << "'. Character Ignored." << endl; }
   ;
%%
void yyerror (const char *msg)
{ 
   cout << "Error: " <<  msg << endl;
}
int main(int argc, char **argv) {
   yylval.tinfo.linenum = 1;
   int option, index;
   char *file = NULL;
   extern FILE *yyin;
   while ((option = getopt (argc, argv, "")) != -1)
      switch (option)
      {
      default:
         ;
      }
   if ( optind == argc ) yyparse();
   for (index = optind; index < argc; index++) 
   {
      yyin = fopen (argv[index], "r");
      yyparse();
      fclose (yyin);
   }
   return 0;
}

