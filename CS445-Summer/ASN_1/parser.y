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
   if(s == NULL){
      printf("Error, sibling without room with parent");
      exit();
   }
   // Make sure t is not null. If ti is, just return s
   if(t == NULL){
      return s;
   }
   // look down t’s sibling list until you fin with with sibblin = null (the end o f the lsit) and add s there.
   while(t->sibling != NULL){
      t = t->sibling;
   }
   t->sibling = s;
   return s;
}
// pass the static and type attribute down the sibling list
void setType(TreeNode *t, ExpType type, bool isStatic)
{
   while (t) {
   // set t->type and t->isStatic
      t->type = type;
      t->isStatic = isStatic
      t = t->sibling;
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
   TokenData *tokenData;
   TreeNode *tree;
   ExpType type; // for passing type spec up the tree
}

%type    <tokenData>   assignop
%type    <tokenData>   relop
%type    <tokenData>   minmaxop
%type    <tokenData>   sumop
%type    <tokenData>   mulop
%type    <tokenData>   unaryop

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

%type    <tree>   simpleExp
%type    <tree>   andExp
%type    <tree>   unaryRelExp
%type    <tree>   relExp
%type    <tree>   minmaxExp
%type    <tree>   sumExp
%type    <tree>   mulExp
%type    <tree>   unaryExp
%type    <tree>   factor
%type    <tree>   mutable
%type    <tree>   immutable
%type    <tree>   call
%type    <tree>   args
%type    <tree>   argList
%type    <tree>   constant
%type    <type>   typeSpec

%token   <tokenData>  FIRSTOP
%token	<tokenData>  PRECOMPILER
%token   <tokenData>  '*' '+' '-' '/' '<' '=' '>' '%' '?'
%token   <tokenData>  AND
%token   <tokenData>  NOT
%token   <tokenData>  OR

%token   <tokenData>  EQ
%token   <tokenData>  LEQ
%token   <tokenData>  GEQ
%token   <tokenData>  MIN
%token   <tokenData>  MAX
%token   <tokenData>  NEQ
%token   <tokenData>  ADDASS
%token   <tokenData>  SUBASS
%token   <tokenData>  MULASS
%token   <tokenData>  DIVASS
%token   <tokenData>  INC
%token   <tokenData>  DEC
%token   <tokenData>  LASTOP

%token   <tokenData>  BOOL
%token   <tokenData>  BREAK
%token   <tokenData>  BY
%token   <tokenData>  CHAR
%token   <tokenData>  DO
%token   <tokenData>  ELSE
%token   <tokenData>  FOR
%token   <tokenData>  IF
%token   <tokenData>  INT
%token   <tokenData>  ID
%token   <tokenData>  RETURN
%token   <tokenData>  STATIC
%token   <tokenData>  THEN
%token   <tokenData>  TO
%token   <tokenData>  WHILE
%token   <tokenData>  BOOLCONST
%token   <tokenData>  NUMCONST
%token   <tokenData>  CHARCONST
%token   <tokenData>  STRINGCONST

%token   <tokenData>  '(' ')' ',' ';' '[' '{' '}' ']' ':'
%token   <tokenData>  LASTTERM
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
funDecl  :  typeSpec ID '(' parms ')' stmt      {$$ = newDeclNode(Funck, $1, $2 $4, $6);}
   |  ID '(' parms ')' stmt                     {$$ = newDeclNode(FuncK, Void, $1, $3, $5);}
   ;
parms  :  parmList
   |  /*empty*/
   ;
parmList  :  parmList ';' parmTypeList
   |  parmTypeList
   ;
parmTypeList  :  typeSpec parmIdList
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
matched  :  IF simpleExp THEN matched ELSE matched       {$$ = newStmtNode(IfK, $1, $2, $4, $6);}
   |  WHILE simpleExp DO matched                         {$$ = newStmtNode(WhileK, $1, $2, $4);}
   |  FOR ID '=' iterRange DO matched                    {$$ = newStmtNode(ForK, $1, NULL, $4, $6);}
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
compoundstmt  :  '{' localDecls stmtList '}'    {$$ = newStmtNode(CompoundK, $1, $2, $3); yyerrok;}
   ;
localDecls  :  localDecls scopedVarDecl
   |  /*empty*/
   ;
stmtList  :  stmtList stmt          {$$ = ($2==NULL ? $1 : addSibling($1, $2));}
   | /*empty*/                      {$$ = NULL;}
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
     AND {printToken(yylval.tokenData, "AND");}
   |  BOOL {printToken(yylval.tokenData, "BOOL");}
   |  BREAK {printToken(yylval.tokenData, "BREAK");}
   |  BY {printToken(yylval.tokenData, "BY");}
   |  CHAR {printToken(yylval.tokenData, "CHAR");}
   |  DO {printToken(yylval.tokenData, "DO");}
   |  ELSE {printToken(yylval.tokenData, "ELSE");}
   |  FOR {printToken(yylval.tokenData, "FOR");}
   |  IF {printToken(yylval.tokenData, "IF");}
   |  INT {printToken(yylval.tokenData, "INT");}
   |  NOT {printToken(yylval.tokenData, "NOT");}
   |  OR {printToken(yylval.tokenData, "OR");}
   |  RETURN {printToken(yylval.tokenData, "RETURN");}
   |  STATIC {printToken(yylval.tokenData, "STATIC");}
   |  THEN {printToken(yylval.tokenData, "THEN");}
   |  TO {printToken(yylval.tokenData, "TO");}
   |  WHILE {printToken(yylval.tokenData, "WHILE");}
   |  BOOLCONST {printToken(yylval.tokenData, "BOOLCONST");}
   |  OP {printToken(yylval.tokenData, "OP");}
   |  EQ {printToken(yylval.tokenData, "EQ");}
   |  LEQ {printToken(yylval.tokenData, "LEQ");}
   |  GEQ {printToken(yylval.tokenData, "GEQ");}
   |  MIN {printToken(yylval.tokenData, "MIN");}
   |  MAX {printToken(yylval.tokenData, "MAX");}
   |  NEQ {printToken(yylval.tokenData, "NEQ");}
   |  ADDASS {printToken(yylval.tokenData, "ADDASS");}
   |  SUBASS {printToken(yylval.tokenData, "SUBASS");}
   |  MULASS {printToken(yylval.tokenData, "MULASS");}
   |  DIVASS {printToken(yylval.tokenData, "DIVASS");}
   |  INC {printToken(yylval.tokenData, "INC");}
   |  DEC {printToken(yylval.tokenData, "DEC");}
   |  ID {printToken(yylval.tokenData, "ID");}
   |  NUMCONST {printToken(yylval.tokenData, "NUMCONST");}
   |  CHARCONST {printToken(yylval.tokenData, "CHARCONST");}
   |  STRINGCONST {printToken(yylval.tokenData, "STRINGCONST");}
   |  PRECOMPILER {printToken(yylval.tokenData, "PRECOMPILER");}
   |  ERROR    {cout << "ERROR(" << yylval.tokenData.linenum << "): Invalid or misplaced input character: '" << yylval.tokenData.tokenstr << "'. Character Ignored." << endl; }
   ;
%%
void yyerror (const char *msg)
{ 
   cout << "Error: " <<  msg << endl;
}
int main(int argc, char **argv) {
   //yylval.tokenData.linenum = 1;
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
