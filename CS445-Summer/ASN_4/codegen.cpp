#include "codegen.h"
extern int numErrors;
extern int numWarnings;
extern void yyparse();
extern int yydebug;
extern TreeNode *syntaxTree;
extern char **largerTokens;
extern void initTokenStrings();

//These offsets that never change
#define OFPOFF 0
#define RETURNOFFSET -1

int toffset; // next available temporary space

FILE *code;                     // shared global code
static bool linenumFlag;        // mark with line numbers
static int breakloc;            // which while to break to
static SymbolTable *globals;    // the global symbol table


void codegenHeader(char *srcFile){
    emitComment((char *)"bC compiler version bC-Su23");
    emitComment((char *)"File compiled: ", srcFile);
}

void codegenGeneral(TreeNode *syntaxTree){

}

void codegenInit(int initJump, int globalOffset){

}



void codegen(FILE *codeIn,          // where the code should be written
            char *srcFile,         // name of file compiled
            TreeNode *syntaxTree,  // tree to process
	        SymbolTable *globalsIn,     // globals so function info can be found
	        int globalOffset,      // size of the global frame
             bool linenumFlagIn)
{
    int initJump;
    code = codeIn;
    globals = globalsIn;
    linenumFlag = linenumFlagIn;
    breakloc = 0;

    initJump = emitSkip(1);
    codegenHeader(srcFile);
    codegenGeneral(syntaxTree);
    codegenInit(initJump,globalOffset);   
}   // comment with line numbers