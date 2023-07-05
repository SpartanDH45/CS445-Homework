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
      exit(1);
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
      t->isStatic = isStatic;
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
%token   <tokenData>  CHSIGN
%token   <tokenData>  SIZEOF
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
program  :  precomlist declList        {syntaxTree = $2;}
   ;
precomlist  :  precomlist PRECOMPILER   { $$ = NULL; printf("%s\n", yylval.tokenData->tokenstr);}
   |  PRECOMPILER                      { $$ = NULL; printf("%s\n", yylval.tokenData->tokenstr);}
   |  /*empty*/                        { $$ = NULL;}
   ;
declList  :  declList decl             { $$ = addSibling($1,$2);}
   |  decl                             { $$ = $1;}
   ;
decl  :  varDecl                       { $$ = $1;}
   |  funDecl                          { $$ = $1;}
   ;
varDecl  :  typeSpec varDeclList ';'   { $$ = $2; setType( $2, $1, false); yyerrok;}
   ;
scopedVarDecl  :  STATIC typeSpec varDeclList ';'  { $$ = $3; setType($3,$2,true);}
   |  typeSpec varDeclList ';'                     { $$ = $2; setType($2,$1,false);}
   ;
varDeclList  :  varDeclList ',' varDeclInit        { $$ = addSibling($1,$3);}
   |  varDeclInit                                  { $$ = $1;}
   ;
varDeclInit  :  varDeclId                          { $$ = $1;}
   | varDeclId ':' simpleExp                       { $$ = addSibling($1, $3);}
   ;
varDeclId  :  ID                                   { $$ = newDeclNode(VarK, UndefinedType, $1);}
   |  ID '[' NUMCONST ']'                          { $$ = newDeclNode(VarK, UndefinedType, $1);}
   ;
typeSpec  :  INT                                   { $$ = Integer;}
   |  BOOL                                         { $$ = Boolean;}
   |  CHAR                                         { $$ = Char;}
   ;
funDecl  :  typeSpec ID '(' parms ')' stmt         { $$ = newDeclNode(FuncK, $1, $2, $4, $6);}
   |  ID '(' parms ')' stmt                        { $$ = newDeclNode(FuncK, Void, $1, $3, $5);}
   ;
parms  :  parmList                                 { $$ = $1;}
   |  /*empty*/                                    { $$ = NULL;}
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
matched  :  IF simpleExp THEN matched ELSE matched       { $$ = newStmtNode(IfK, $1, $2, $4, $6);}
   |  WHILE simpleExp DO matched                         { $$ = newStmtNode(WhileK, $1, $2, $4);}
   |  FOR ID '=' iterRange DO matched                    { $$ = newStmtNode(ForK, $1, NULL, $4, $6);}
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
compoundstmt  :  '{' localDecls stmtList '}'    { $$ = newStmtNode(CompoundK, $1, $2, $3); yyerrok;}
   ;
localDecls  :  localDecls scopedVarDecl
   |  /*empty*/
   ;
stmtList  :  stmtList stmt          { $$ = ($2==NULL ? $1 : addSibling($1, $2));}
   | /*empty*/                      { $$ = NULL;}
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
%%

void yyerror (const char *msg)
{ 
   cout << "Error: " <<  msg << endl;
}

char *largerTokens[LASTTERM+1];        // used in the utils.cpp file printing routines

// create a mapping from token class enum to a printable name in a
// way that makes it easy to keep the mapping straight.
void initTokenStrings()
{
    largerTokens[ADDASS] = (char *)"+=";
    largerTokens[AND] = (char *)"and";
    largerTokens[BOOL] = (char *)"bool";
    largerTokens[BOOLCONST] = (char *)"boolconst";
    largerTokens[BREAK] = (char *)"break";
    largerTokens[BY] = (char *)"by";
    largerTokens[CHAR] = (char *)"char";
    largerTokens[CHARCONST] = (char *)"charconst";
    largerTokens[CHSIGN] = (char *)"chsign";
    largerTokens[DEC] = (char *)"--";
    largerTokens[DIVASS] = (char *)"/=";
    largerTokens[DO] = (char *)"do";
    largerTokens[ELSE] = (char *)"else";
    largerTokens[EQ] = (char *)"==";
    largerTokens[FOR] = (char *)"for";
    largerTokens[GEQ] = (char *)">=";
    largerTokens[ID] = (char *)"id";
    largerTokens[IF] = (char *)"if";
    largerTokens[INC] = (char *)"++";
    largerTokens[INT] = (char *)"int";
    largerTokens[LEQ] = (char *)"<=";
    largerTokens[MAX] = (char *)":>:";
    largerTokens[MIN] = (char *)":<:";
    largerTokens[MULASS] = (char *)"*=";
    largerTokens[NEQ] = (char *)"!=";
    largerTokens[NOT] = (char *)"not";
    largerTokens[NUMCONST] = (char *)"numconst";
    largerTokens[OR] = (char *)"or";
    largerTokens[RETURN] = (char *)"return";
    largerTokens[SIZEOF] = (char *)"sizeof";
    largerTokens[STATIC] = (char *)"static";
    largerTokens[STRINGCONST] = (char *)"stringconst";
    largerTokens[SUBASS] = (char *)"-=";
    largerTokens[THEN] = (char *)"then";
    largerTokens[TO] = (char *)"to";
    largerTokens[WHILE] = (char *)"while";
    largerTokens[LASTTERM] = (char *)"lastterm";
}


int main(int argc, char **argv) {
   //yylval.tokenData.linenum = 1;
   int index;
   char *file = NULL;
   bool dotAST = false;             // make dot file of AST
   extern FILE *yyin;
   initTokenStrings();
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
      printTree(stdout, syntaxTree, false, false);
      if(dotAST) {
         printDotTree(stdout, syntaxTree, false, false);
      }
   }
   else {
      printf("-----------\n");
      printf("Errors: %d\n", numErrors);
      printf("-----------\n");
   }
   // report the number of errors and warnings
   printf("Number of warnings: %d\n", numWarnings);
   printf("Number of errors: %d\n", numErrors);
   
   return 0;
}
