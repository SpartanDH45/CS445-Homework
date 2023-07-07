#include "semantics.h"
#include "parser.tab.h"
#include "symbolTable.h"

static ExpectType expectType[LASTOP];
static ReturnType returnType[LASTOP];
static int goffset;     // top of global space
static int foffset;     // top of local space

TreeNode *loadIOLib(TreeNode *syntree)
{
    TreeNode *input, *output, *param_output;
    TreeNode *inputb, *outputb, *param_outputb;
    TreeNode *inputc, *outputc, *param_outputc;
    TreeNode *outnl;

    input = newDeclNode(FuncK, Integer);
    input->lineno = -1; // all are -1
    input->attr.name = strdup("input"); //We named the variables well input->type = Integer;
    input->type = Integer;

    inputb = newDeclNode(FuncK, Boolean);
    inputb->attr.name = strdup("inputb"); //We named the variables well input->type = Integer;
    inputb->lineno = -1; // all are -1
    inputb->type = Boolean;


    inputc = newDeclNode(FuncK, Boolean);
    inputc->type = Char;
    inputc->attr.name = strdup("inputc"); //We named the variables well input->type = Integer;
    inputc->lineno = -1; // all are -1

    param_output = newDeclNode(ParamK, Void);
    param_output->attr.name = strdup("*dummy*");
    param_output->type = Integer;
    param_output->lineno = -1; // all are -1

    output = newDeclNode(FuncK, Void);
    output->child[0] = param_output;
    output->attr.name = strdup("output");
    output->type = Void;
    output->lineno = -1; // all are -1

    param_outputb = newDeclNode(ParamK, Void);
    param_outputb->attr.name = strdup("*dummy*");
    param_outputb->type = Boolean;
    param_outputb->lineno = -1; // all are -1

    outputb = newDeclNode(FuncK, Void);
    outputb->child[0] = param_outputb;
    outputb->attr.name = strdup("outputb");
    outputb->lineno = -1; // all are -1
    outputb->type = Void;

    param_outputc = newDeclNode(ParamK, Void);
    param_outputc->attr.name = strdup("*dummy*");
    param_outputc->type = Char;
    param_outputc->lineno = -1; // all are -1

    outputc = newDeclNode(FuncK, Void);
    outputc->child[0] = param_outputc;
    outputc->attr.name = strdup("outputc");
    outputc->lineno = -1; // all are -1
    outputc->type = Void;

    outnl = newDeclNode(FuncK, Void);
    outnl->child[0] = NULL;
    outnl->attr.name = strdup("outnl");
    outnl->lineno = -1; // all are -1
    outnl->type = Void;

    input->sibling = output;
    output->sibling = inputb;
    inputb->sibling = outputb;
    outputb->sibling = inputc;
    inputc->sibling = outputc;
    outputc->sibling = outnl;
    outnl->sibling = syntree; // add in the tree we were given return input;

    return input;
}

void traverseDeclK(TreeNode *current, SymbolTable *symtab){

}
void traverseStmtK(TreeNode *current, SymbolTable *symtab){

}
void traverseExpK(TreeNode *current, SymbolTable *symtab){
    
}

bool isNodeCompound(TreeNode *current) {
    if(current == NULL) {return false;}
    if(current->nodekind == DeclK  && current->kind.decl == FuncK) { foffset = -2; return true;}
    if(current->nodekind == StmtK) {
        if( current->kind.stmt == CompoundK || current->kind.stmt == ForK)
        {
            return true;
        }
    }
    return false;
}

void treeTraverse(TreeNode *current, SymbolTable *symtab){
    bool isCompound = false;
    if(current == NULL) {return;}

    isCompound = isNodeCompound(current);

    if(isCompound) {
        char *id = strdup("{");
        symtab->enter("NewScope from " + (std::string)id);
    }
    int temptoff = foffset;
    treeTraverse(current->child[0], symtab);
    bool isCompound = false;
    if(current == NULL) {return;}

    isCompound = isNodeCompound(current);

    if(isCompound) {
        char *id = strdup("{");
        symtab->enter("NewScope from " + (std::string)id);
    }
    int temptoff = foffset;
    treeTraverse(current->child[0], symtab);
}


TreeNode *semanticAnalysis(TreeNode *syntree,          // pass in and return an annotated syntax tree
                           bool shareCompoundSpaceIn,   // SPECIAL OPTION: make compound after a function share scope
                           bool noDuplicateUndefsIn,    // SPECIAL OPTION: no duplicate undefines
                           SymbolTable *symtabX,       // pass in and return the symbol table
                           int &globalOffset)            // return the offset past the globals
{
    //binary ops
   for (int i=0; i<LASTOP; i++) returnType[i] = RetInt;
   returnType[AND] = RetBool;
   returnType[OR] = RetBool;
   returnType[EQ] = RetBool;
   returnType[NEQ] = RetBool;
   returnType[LEQ] = RetBool;
   returnType[int('<')] = RetBool;
   returnType[GEQ] = RetBool;
   returnType[int('>')] = RetBool;
   returnType[int('=')] = RetLHS;
   returnType[int('[')] = RetLHS;

   //unary ops
   returnType[NOT] = RetBool;

   //anything else to add
   syntree = loadIOLib(syntree);

   treeTraverse(syntree, symtabX);

   // remember where the globals are
   globalOffset = goffset;

    return syntree;
}