%{
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include "treeUtils.h"
#include "scanType.h"
#include "dot.h"
using namespace std;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

int numErrors;
int numWarnings;
extern int line;
extern int yylex();

TreeNode *addSibling(TreeNode *t, TreeNode *s)
{
// make sure s is not null. If it is this s a major error. Exit the program!
// Make sure t is not null. If ti is, just return s
// look down t’s sibling list until you fin with with sibblin = null (the end o f the lsit) and add s there.
return s;
}
// pass the static and type attribute down the sibling list
void setType(TreeNode *t, ExpType type, bool isStatic)
{
while (t) {
// set t->type and t->isStatic
// t = t->sibling;
}
}
// the syntax tree goes here
TreeNode *syntaxTree;

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
   TokenData *tokenData;
   TreeNode *tree;
   ExpType type; // for passing type spec up the tree
}
%type    <tree>   program
%type    <tree>   precomlist
%type    <tree>   declList
%type    <tree>   decl
%type    <tree>   varDecl
%type    <tree>   scopedVarDecl
%type    <tree>   varDeclList
%type    <tree>   varDeclInit
%type    <tree>   varDeclId
%type    <type>   typeSpec
%type    <tree>   funDecl
%type    <tree>   parms
%type    <tree>   parmList
%type    <tree>   parmTypeList
%type    <tree>   parmIdList
%type    <tree>   parmId
%type    <tree>   stmt
%type    <tree>   matched
%type    <tree>   iterRange
%type    <tree>   unmatched
%type    <tree>   expstmt
%type    <tree>   compoundstmt
%type    <tree>   localDecls
%type    <tree>   stmtList
%type    <tree>   returnstmt
%type    <tree>   breakstmt
%type    <tree>   exp
%type    <tree>   assignop
%type    <tree>   simpleExp
%type    <tree>   andExp
%type    <tree>   unaryRelExp
%type    <tree>   relExp
%type    <tree>   relop
%type    <tree>   minmaxExp
%type    <tree>   minmaxop
%type    <tree>   sumExp
%type    <tree>   sumop
%type    <tree>   mulExp
%type    <tree>   mulop
%type    <tree>   unaryExp
%type    <tree>   unaryop
%type    <tree>   factor
%type    <tree>   mutable
%type    <tree>   immutable
%type    <tree>   call
%type    <tree>   args
%type    <tree>   argList
%type    <tree>   constant
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
%%
program  :  precomlist declList        {syntaxTree = $2}
   ;
precomlist  :  precomlist PRECOMPILER  {$$ = $1}
   |  PRECOMPILER                      { printf("%s\n", yyval.tokenData->tokenstr);}
   |  /*empty*/                        {$$ = NULL;}
   ;
declList  :  declList decl             {$$ = addSibling($1,$2);}
   |  decl                             {$$ = $1;}
   ;
decl  :  varDecl                       {$$ = $1;}
   |  funDecl                          {$$ = $1;}
   ;
varDecl  :  typeSpec varDeclList ';'   {$$ = $2; setType( $2, $1, false); yyerrok;}
   ;
scopedVarDecl  :  STATIC typeSpec varDeclList ';'
   |  typeSpec varDeclList ';'
   ;
varDeclList  :  varDeclList ',' varDeclInit
   |  varDeclInit
   ;
varDeclInit  :  varDeclId
   | varDeclId ':' simpleExp
   ;
varDeclId  :  ID
   | ID '['NUMCONST']'
   ;
typeSpec  :  INT
   |  BOOL
   |  CHAR
   ;
funDecl  :  typeSpec ID '(' parms ')' stmt
   |  ID '(' parms ')' stmt
   ;
parms  :  parmList
   |  /*empty*/
   ;
parmList  :  parmList ';' parmTypeList
   |  parmTypeList
   ;
parmIdList  :  parmIdList ',' parmId
   |  parmId
   ;
parmId  :  ID
   | ID '['']'
   ;
stmt  :  matched
   |  unmatched
   ;
matched  :  IF simpleExp THEN matched ELSE matched
   |  WHILE simpleExp DO matched
   |  FOR ID '=' iterRange DO matched
   |  expstmt
   |  compoundstmt
   |  returnstmt
   |  breakstmt
   ;
iterRange  :  simpleExp TO simpleExp
   |  simpleExp TO simpleExp BY simpleExp
   ;
unmatched  :  IF simpleExp THEN stmt
   |  IF simpleExp THEN matched ELSE unmatched
   | WHILE simpleExp DO unmatched
   | FOR ID '=' iterRange DO unmatched
   ;
expstmt  :  exp ';'
   ;
compoundstmt  :  '{' localDecls stmtList '}'
   ;
localDecls  :  localDecls scopedVarDecl
   |  /*empty*/
   ;
stmtList  :  stmtList stmt
   | /*empty*/
   ;
returnstmt  :  RETURN ';'
   | RETURN exp ';'
   ;
breakstmt  :  BREAK ';'
   ;
exp  :  mutable assignop exp
   |  mutable INC
   |  mutable DEC
   |  simpleExp
   |  mutable assignop error
   ;
assignop  :  '='
   |  ADDASS
   |  SUBASS
   |  MULASS
   |  DIVASS
   ;
simpleExp  :  simpleExp OR andExp
   |  andExp
   ;
andExp  :  andExp AND unaryRelExp
   |  unaryRelExp
   ;
unaryRelExp  :  NOT unaryRelExp
   |  relExp
   ;
relExp  :  minmaxExp relop minmaxExp
   |  minmaxExp
   ;
relop  :  LEQ
   |  '<'
   |  '>'
   |  GEQ
   |  EQ
   |  NEQ
   ;
minmaxExp  :  minmaxExp minmaxop sumExp
   |  sumExp
   ;
minmaxop  :  MAX
   |  MIN
   ;
sumExp  :  sumExp sumop mulExp
   |  mulExp
   ;
sumop  :  '+'
   |  '-'
   ;
mulExp  :  mulExp mulop unaryExp
   |  unaryExp
   ;
mulop  :  '*'
   |  '/'
   |  '%'
   ;
unaryExp  :  unaryop unaryExp
   |  factor
   ;
unaryop  :  '-'
   |  '*'
   |  '?'
   ;
factor  :  immutable
   |  mutable
   ;
mutable  :  ID
   |  ID '[' exp ']'
   ;
immutable  :  '(' exp ')'
   |  call
   |  constant
   ;
call  :  ID '(' args ')'
   ;
args  :  argList
   |  /*empty*/
   ;
argList  :  argList ',' exp
   | exp
   ;
constant  :  NUMCONST
   |  CHARCONST
   |  STRINGCONST
   |  BOOLCONST
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
   //yylval.tinfo.linenum = 1;
   int index;
   char *file = NULL;
   bool dotAST = false;             // make dot file of AST
   extern FILE *yyin;

   int ch;

   while ((ch = getopt(argc, argv, "d")) != -1) {
      switch (ch) {
         case 'd':
                 dotAST = true;
                 break;
         case '?':
         default:
                 //usage();
               ;
      }
   }

   if ( optind == argc ) yyparse();
   for (index = optind; index < argc; index++)
   {
      yyin = fopen (argv[index], "r");
      yyparse();
      fclose (yyin);
   }
   if (numErrors==0) {
      printTree(stdout, syntaxTree, true, true);
      if(dotAST) {
         printDotTree(stdout, syntaxTree, false, false);
      }
   }
   else {
      printf("-----------\n");
      printf("Errors: %d\n", numErrors);
      printf("-----------\n");
   }
   return 0;
}
