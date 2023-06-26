#include "treeUtils.h"

TreeNode *cloneNode(TreeNode *currnode){

}

TreeNode *newDeclNode(DeclKind kind,
                    ExpType type,
                    TokenData *token=NULL,
                    TreeNode *c0=NULL,
                    TreeNode *c1=NULL,
                    TreeNode *c2=NULL){  // save TokenData block!!
                        TreeNode *newNode;
   int i;

   newNode = new TreeNode;
   newNode->nodeNum = nodeNum++;

   if (newNode == NULL) {
        yyerror("ERROR: Out of memory");
   }
   else {
        // set the defaults for a general node
        newNode->child[0] = c0;
        newNode->child[1] = c1;
        newNode->child[2] = c2;
        newNode->sibling = NULL;
        newNode->lineno = (token ? token->linenum : -1);
        newNode->attr.name = (token ? token->svalue : strdup("DUMMY"));    // NOTE: just copies pointer
        newNode->type = type;
        // set default values for inferred parts
        newNode->size = 1;
        newNode->varKind = Local;
        newNode->offset = 0;
        newNode->isArray = false;
        newNode->isStatic = false;
        newNode->isConst = false;

        // set the defaults for this class of nodes
        newNode->nodekind = DeclK;
        newNode->kind.decl = kind;
   }
   return newNode;
}
TreeNode *newStmtNode(StmtKind kind,
                    TokenData *token,
                    TreeNode *c0=NULL,
                    TreeNode *c1=NULL,
                    TreeNode *c2=NULL){
    TreeNode *newNode;
    int i;

    newNode = new TreeNode;
    newNode->nodeNum = nodeNum++;

    if (newNode == NULL) {
        yyerror("ERROR: Out of memory");
    }
    else {
        // set the defaults for a general node
        newNode->child[0] = c0;
        newNode->child[1] = c1;
        newNode->child[2] = c2;
        newNode->sibling = NULL;
        newNode->lineno = token->linenum;
    // set default just to be sure (should not be needed except varKind)
        newNode->varKind = None;
        newNode->size = 1;
        newNode->offset = 0;
        newNode->isArray = false;
        newNode->isStatic = false;
        newNode->isConst = false;

    // set the defaults for this class of nodes
        newNode->nodekind = StmtK;
        newNode->kind.stmt = kind;
   }
   return newNode;
}
TreeNode *newExpNode(ExpKind kind,
    TreeNode *newNode;
   int i;

   newNode = new TreeNode;
   newNode->nodeNum = nodeNum++;

   if (newNode == NULL) {
      yyerror("ERROR: Out of memory");
   }
   else {
//////////////////
//printf("%c %d\n", token->tokenclass, token->tokenclass);
        // set the defaults for a general node
        newNode->child[0] = c0;
        newNode->child[1] = c1;
        newNode->child[2] = c2;
        newNode->sibling = NULL;
        newNode->lineno = token->linenum;
        newNode->attr.op = OpKind(token->tokenclass);
        newNode->type = UndefinedType;
        // set default just to be sure
        newNode->size = 1;
        newNode->varKind = Local;
        newNode->offset = 0;
        newNode->isArray = false;
        newNode->isStatic = false;
        newNode->isConst = false;

        // set the defaults for this class of nodes
        newNode->nodekind = ExpK;
        newNode->kind.exp = kind;
   }
   return newNode;
                    }

char *tokenToStr(int type){

}

char *expTypeToStr(ExpType type, bool isArray=false, bool isStatic=false){

}

void printTreeNode(FILE *out, TreeNode *syntaxTree, bool showExpType, bool showAllocation){

}

void printTree(FILE *out, TreeNode *syntaxTree, bool showExpType, bool showAllocation){

}




char *varKindToStr(int kind)
{
    switch (kind) {
    case None:
        return (char *)"None";
    case Local:
        return (char *)"Local";
    case Global:
        return (char *)"Global";
    case Parameter:
        return (char *)"Parameter";
    case LocalStatic:
        return (char *)"LocalStatic";
    default:
   return (char *)"unknownVarKind";
    }
}

// allocate a FIX BUFFER.  You must copy the string if you
// are referencing the function twice in the same printf for example.
char expTypeToStrBuffer[80];
char *expTypeToStr(ExpType type, bool isArray, bool isStatic)
{
    char *typeName;

    switch (type) {
    case Void:
       typeName = (char *)"type void";
       break;
    case Integer:
       typeName = (char *)"type int";
       break;
    case Boolean:
       typeName = (char *)"type bool";
       break;
    case Char:
       typeName = (char *)"type char";
       break;
    case UndefinedType:
       typeName = (char *)"undefined type";
       break;
    default:
       char *buffer;
       buffer = new char [80];
       sprintf(buffer, "invalid expType: %d", (int)type);
       return buffer;
    }

    // add static and array attributes
    // static type int
    // static array of type int
    sprintf(expTypeToStrBuffer, "%s%s%s",
            (isStatic ? "static " : ""),
            (isArray ? "array of " : ""),
            typeName);

    return strdup(expTypeToStrBuffer); // memory leak
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

// print a node without a newline
void printTreeNode(FILE *listing,
                   TreeNode *tree,
                   bool showExpType,
                   bool showAllocation)
{
   // print a declaration node
   if (tree->nodekind == DeclK) {
      switch (tree->kind.decl) {
         case VarK:
            printf("Var: %s ", tree->attr.name);
            printf("of %s", expTypeToStr(tree->type, tree->isArray, tree->isStatic));
            if (showAllocation) {
                printf(" [mem: %s loc: %d size: %d]", varKindToStr(tree->varKind), tree->offset, tree->size);
            }
            break;
         case FuncK:
            printf("Func: %s ", tree->attr.name);
            printf("returns %s", expTypeToStr(tree->type, tree->isArray, tree->isStatic));
            if (showAllocation) {
                printf(" [mem: %s loc: %d size: %d]", varKindToStr(tree->varKind), tree->offset, tree->size);
            }
            break;
         case ParamK:
            printf("Parm: %s ", tree->attr.name);
            printf("of %s", expTypeToStr(tree->type, tree->isArray, tree->isStatic));
            if (showAllocation) {
                printf(" [mem: %s loc: %d size: %d]", varKindToStr(tree->varKind), tree->offset, tree->size);
            }
            break;
         default:
            fprintf(listing, "Unknown declaration node kind: %d",
            tree->kind.decl);
            break;
      }
   }
      // print a statement node
   else if (tree->nodekind == StmtK) {
      switch (tree->kind.stmt) {
      case IfK:
         fprintf(listing, "If");
         break;
      case WhileK:
         fprintf(listing, "While");
         break;
      case CompoundK:
         fprintf(listing, "Compound");
            if (showAllocation) {
               printf(" [mem: %s loc: %d size: %d]", varKindToStr(tree->varKind), tree->offset, tree->size);
            }
         break;
      case ForK:
         fprintf(listing, "For");
         if (showAllocation) {
            printf(" [mem: %s loc: %d size: %d]", varKindToStr(tree->varKind), tree->offset, tree->size);
         }
         break;
      case RangeK:
         fprintf(listing, "Range");
         break;
      case ReturnK:
         fprintf(listing, "Return");
         break;
      case BreakK:
         fprintf(listing, "Break");
         break;
      default:
         fprintf(listing, "Unknown  statement node kind: %d",tree->kind.stmt);
         break;
      }
   }
      // print an expression node
   else if (tree->nodekind == ExpK) {
      switch (tree->kind.exp) {
      case AssignK:
         fprintf(listing, "Assign: %s", tokenToStr(tree->attr.op));
         break;
      case OpK:
         fprintf(listing, "Op: %s", tokenToStr(tree->attr.op));
         break;
      case ConstantK:
         switch (tree->type) {
            case Boolean:
               fprintf(listing, "Const %s", (tree->attr.value) ?  "true" : "false");
               break;
            case Integer:
               fprintf(listing, "Const %d", tree->attr.value);
               break;
            case Char:
               if (tree->isArray) {
                  fprintf(listing, "Const ");
                  printf("\"");
                  for (int i=0; i<tree->size-1; i++) {
                     printf("%c", tree->attr.string[i]);
                  }
                  printf("\"");
               }
               else fprintf(listing, "Const '%c'", tree->attr.cvalue);
               break;
            case Void:
            case UndefinedType:
               fprintf(listing, "SYSTEM ERROR: parse tree contains invalid type for constant: %s\n", expTypeToStr(tree->type));
         }
         break;
      case IdK:
         fprintf(listing, "Id: %s", tree->attr.name);
         break;
      case CallK:
         fprintf(listing, "Call: %s", tree->attr.name);
         break;
      default:
         fprintf(listing, "Unknown expression node kind: %d", tree->kind.exp);
         break;
   }
   if (showExpType) {
       fprintf(listing, " of %s", expTypeToStr(tree->type, tree->isArray, tree->isStatic));
   }
        if (showAllocation) {
            if (tree->kind.exp == IdK || tree->kind.exp == ConstantK && tree->type == Char && tree->isArray) {
                printf(" [mem: %s loc: %d size: %d]", varKindToStr(tree->varKind), tree->offset, tree->size);
            }
        }
    }
    else fprintf(listing, "Unknown class of node: %d",
       tree->nodekind);

    fprintf(listing, " [line: %d]", tree->lineno);
}