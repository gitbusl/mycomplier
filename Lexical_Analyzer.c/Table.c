//表操作函数的定义
#include "Table.h"
#include<string.h>
#include<malloc.h>

//变量表查找 单词在变量表中的位置  不在表中时,添加到表中再返回序号
int SearchVar(struct wordNode* node)
{
    struct VariableNode* varnode = head;

    if (varnode == NULL)
    {
        InitVarNode(node);//第一个节点
    }
    while (varnode != NULL)//查找结点
    {
        if (node->type == varnode->Type && !strcmp(node->word, varnode->name))
        {
            return varnode->index;
        }

        else
        {
            varnode = varnode->next;
            //continue;
        }
    }

    return AddVarNode(node);


}
//变量表初始化函数，创建第一个节点
int  InitVarNode(struct wordNode* node)
{
    struct VariableNode* NewNode = malloc(sizeof(struct VariableNode));
    NewNode->index = 0;
    NewNode->next = NULL;
    NewNode->Type = node->type;
    NewNode->name = node->word;
    NewNode->Value = node->value;
    head=NewNode;
    return 1;
}
//标识符表插入函数  尾插法
int AddVarNode(struct wordNode* node)
{
    head->index++;
    struct VariableNode* NewNode = malloc(sizeof(struct VariableNode));
    NewNode->index = head->index;
    NewNode->Type = node->type;
    NewNode->name = node->word;
    NewNode->Value = node->value;
    NewNode->next = head->next;
    head->next=NewNode;
    return NewNode->index;
}

//查找表(关键字,分隔符,运算符)    单词在表中的序号    不在表中时,返回-1
int Search(struct wordNode* node, int Type)
{
    char** Table;
    int length = 0;
    int i = 0;

    switch(Type)
    {
    case 0:
        Table = Key;
        length=Key_Len;
        break;
    case 1:
        Table = Sparator;
        length=Sparator_Len;
        break;
    case 2:
        Table = Operator;
        length=Operator_Len;
        break;
    default:
        ;
    }
    for (i = 0; i < length; i++)
    {
        if (!strcmp(Table[i], node->word))return i;
    }
    return -1;
}

//变量表输出函数
int OutPutVar(FILE* out)
{
    struct VariableNode * node = head->next;
    int i=0;
    fprintf(out,"类型\t序号\t值\t\t变量名\n");
    while (node != NULL)
    {
        fprintf(out,"%d\t%d\t%f\t%s\n", node->Type, node->index, node->Value, node->name);
        node = node->next;
        i++;
    }

    return i;
}

