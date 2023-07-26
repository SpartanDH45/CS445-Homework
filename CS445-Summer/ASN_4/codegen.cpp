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

void codegenGeneral(TreeNode *currnode);

void codegenHeader(char *srcFile){
    emitComment((char *)"bC compiler version bC-Su23");
    emitComment((char *)"File compiled: ", srcFile);
}

void commentLineNum(TreeNode *currnode){
    char buf[16];
    if(linenumFlag){
        sprintf(buf, "%d", currnode->lineno);
        emitComment((char *)"Line: ", buf);
    }
}

void codegenLibraryFun(TreeNode *currnode){
    emitComment((char *)"");
    emitComment((char *)"** ** ** ** ** ** ** ** ** ** ** **");
    emitComment((char *)"FUNCTION", currnode->attr.name);
    // remember where this function is
    currnode->offset = emitSkip(0);
    // Store return address
    emitRM((char *)"ST", AC, RETURNOFFSET, FP, (char *)"Store return address");
// Next slides here
    if (strcmp(currnode->attr.name, (char *)"input")==0) {
        emitRO((char *)"IN", RT, RT, RT, (char *)"Grab int input");
    }
    else if (strcmp(currnode->attr.name, (char *)"inputb")==0) {
        emitRO((char *)"INB", RT, RT, RT, (char *)"Grab bool input");
    }
    else if (strcmp(currnode->attr.name, (char *)"inputc")==0) {
        emitRO((char *)"INC", RT, RT, RT, (char *)"Grab char input");
    }
    else if (strcmp(currnode->attr.name, (char *)"output")==0) {
        emitRM((char *)"LD", AC, -2, FP, (char *)"Load parameter");
        emitRO((char *)"OUT", AC, AC, AC, (char *)"Output integer");
    }
    else if (strcmp(currnode->attr.name, (char *)"outputb")==0) {
        emitRM((char *)"LD", AC, -2, FP, (char *)"Load parameter");
        emitRO((char *)"OUTB", AC, AC, AC, (char *)"Output bool");
    }else if (strcmp(currnode->attr.name, (char *)"outputc")==0) {
        emitRM((char *)"LD", AC, -2, FP, (char *)"Load parameter");
        emitRO((char *)"OUTC", AC, AC, AC, (char *)"Output char");
    }
    else if (strcmp(currnode->attr.name, (char *)"outnl")==0) {
        emitRO((char *)"OUTNL", AC, AC, AC, (char *)"Output a newline");
    }
    else {
        emitComment((char *)"ERROR(LINKER): No support for special function");
        emitComment(currnode->attr.name);
    }

    emitRM((char *)"LD", AC, RETURNOFFSET, FP, (char *)"Load return address");
    emitRM((char *)"LD", FP, OFPOFF, FP, (char *)"Adjust fp");
    emitGoto(0, AC, (char *)"Return");
    emitComment((char *)"END FUNCTION", currnode->attr.name);
}

void codegenFun(TreeNode *currnode){
    emitComment((char *)"");
    emitComment((char *)"** ** ** ** ** ** ** ** ** ** ** **");
    emitComment((char *)"FUNCTION", currnode->attr.name);
    toffset = currnode->size; // recover the end of activation record
    emitComment((char *)"TOFF set:", toffset);
    // IMPORTANT: For function nodes the offset is defined to be the position of the
    // function in the code space! This is accessible via the symbol table.
    // remember where this function is:
    currnode->offset = emitSkip(0); // offset holds the instruction address!!
    // Store return address
    emitRM((char *)"ST", AC, RETURNOFFSET, FP, (char *)"Store return address");
    // Generate code for the statements...
    codegenGeneral(currnode->child[1]);
    // In case there was no return statement
    // set return register to 0 and return
    emitComment((char *)"Add standard closing in case there is no return statement");
    emitRM((char *)"LDC", RT, 0, 6, (char *)"Set return value to 0");
    emitRM((char *)"LD", AC, RETURNOFFSET, FP, (char *)"Load return address");
    emitRM((char *)"LD", FP, OFPOFF, FP, (char *)"Adjust fp");
    emitGoto(0, AC, (char *)"Return");
    emitComment((char *)"END FUNCTION", currnode->attr.name);
}

void codegenStatement(TreeNode *currnode){
    // local state to remember stuff
    int skiploc=0, skiploc2=0, currloc=0; // some temporary instuction addresses
    TreeNode *loopindex=NULL; // a pointer to the index variable declaration node
    commentLineNum(currnode);
    switch (currnode->kind.stmt) {
    /////////////////Other cases
    case CompoundK:
        { 
            int savedToffset;
            savedToffset = toffset;
            toffset = currnode->size; // recover the end of activation record
            emitComment((char *)"COMPOUND");
            emitComment((char *)"TOFF set:", toffset);
            codegenGeneral(currnode->child[0]); // process inits
            emitComment((char *)"Compound Body");
            codegenGeneral(currnode->child[1]); // process body
            toffset = savedToffset;
            emitComment((char *)"TOFF set:", toffset);
            emitComment((char *)"END COMPOUND");
        }
        break;
    default:
        break;
    }
}

void codegenExpression(TreeNode *currnode){
    
}

void codegenDecl(TreeNode *currnode){
    commentLineNum(currnode);
    switch(currnode->kind.decl) {
        case VarK:
            // You have a LOT to do here!!!!!
            if(currnode->isArray){
                if(currnode->child[0]){
                    //if(currnode->varKind == local){
                        //emitRM((char *)"LCD", 3, currnode->size-1, 6, (char *)"load size of array", currnode->attr.name);
                        //emitRM((char *)"ST", AC, -2, FP, (char *), )
                    //}
                }
            }
            break;
        case FuncK:
            if (currnode->lineno == -1) { // These are the library functions we just added
                codegenLibraryFun(currnode);
            }
            else {
                codegenFun(currnode);
            }
            break;
            case ParamK:
                // IMPORTANT: no instructions need to be allocated for parameters here
                break;
    }
}

void codegenGeneral(TreeNode *currnode){
    while(currnode){
        switch(currnode->nodekind){
            case StmtK:
                codegenStatement(currnode);
                break;
            case ExpK:
                emitComment((char *)"EXPRESSION");
                codegenExpression(currnode);
                break;
            case DeclK:
                codegenDecl(currnode);
                break;
        }
        currnode = currnode->sibling;
    }
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