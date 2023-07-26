#include "codegen.h"
FILE *code;

void codegen(FILE *codeIn,          // where the code should be written
            char *srcFile,         // name of file compiled
            TreeNode *syntaxTree,  // tree to process
	        SymbolTable *globalsIn,     // globals so function info can be found
	        int globalOffset,      // size of the global frame
             bool linenumFlagIn)
{
    
}   // comment with line numbers