//����������Ķ���
#include "Table.h"
#include<string.h>
#include<malloc.h>

//��������� �����ڱ������е�λ��  ���ڱ���ʱ,��ӵ������ٷ������
int SearchVar(struct wordNode* node)
{
    struct VariableNode* varnode = head;

    if (varnode == NULL)
    {
        InitVarNode(node);//��һ���ڵ�
    }
    while (varnode != NULL)//���ҽ��
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
//�������ʼ��������������һ���ڵ�
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
//��ʶ������뺯��  β�巨
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

//���ұ�(�ؼ���,�ָ���,�����)    �����ڱ��е����    ���ڱ���ʱ,����-1
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

//�������������
int OutPutVar(FILE* out)
{
    struct VariableNode * node = head->next;
    int i=0;
    fprintf(out,"����\t���\tֵ\t\t������\n");
    while (node != NULL)
    {
        fprintf(out,"%d\t%d\t%f\t%s\n", node->Type, node->index, node->Value, node->name);
        node = node->next;
        i++;
    }

    return i;
}

