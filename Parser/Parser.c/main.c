#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "Parser.c"

int main()
{
    struct WordHead *Head=NULL;//�ʴ����ָ�룬�﷨�����ʴ�ͷ
    struct Syntax_Node* tHead=NULL;//�﷨��ͷ�ڵ�
    Head=Analyse();//���ôʷ������ӳ��򣬷��شʴ�ͷ
    tHead=Program(Head->first);//�﷨�������
    PrintSyntaxTree(tHead,0,fopen("SyntaxTree.txt","w"));
    PrintSyntaxTree(tHead,0,stdout);
    return 0;
}
