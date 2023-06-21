#include "treeUtils.h"

TreeNode *cloneNode(TreeNode *currnode){

}

TreeNode *newDeclNode(DeclKind kind,
                    ExpType type,
                    TokenData *token=NULL,
                    TreeNode *c0=NULL,
                    TreeNode *c1=NULL,
                    TreeNode *c2=NULL){  // save TokenData block!!
                        int i;
                        newNode = new TreeNode;
                        newNode->nodeNum = nodeNum++;
                        newNode->kind.decl = kind;
                        newNode->type = type;
                        newNode->child[0] = c0;
                        newNode->child[1] = c1;
                        newNode->child[2] = c2;
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


                        return newNode;
                    }
TreeNode *newExpNode(ExpKind kind,
                    TokenData *token,
                    TreeNode *c0=NULL,
                    TreeNode *c1=NULL,
                    TreeNode *c2=NULL){
                        int i;
                        newNode = new TreeNode;
                        newNode->nodeNum = nodeNum++;

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
