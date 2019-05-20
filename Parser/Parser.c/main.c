#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "Parser.c"

int main()
{
    struct WordHead *Head=NULL;//词串结点指针，语法分析词串头
    struct Syntax_Node* tHead=NULL;//语法树头节点
    Head=Analyse();//调用词法分析子程序，返回词串头
    tHead=Program(Head->first);//语法分析入口
    PrintSyntaxTree(tHead,0,fopen("SyntaxTree.txt","w"));
    PrintSyntaxTree(tHead,0,stdout);
    return 0;
}
