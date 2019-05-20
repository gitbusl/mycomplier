//�����ʷ�������ر�ṹ
#include "Lex_Head.h"
#ifndef TABLE_H_
#define TABLE_H_

#define KEY         0//�ؼ���
#define SPARATOR    1//�ָ���
#define OPERATOR    2//�����
#define VALUE       3//����
#define VARIABLE    4//����

//�ؼ��ֱ�
static int Key_Len= 32;//����
static char* Key[] =
{
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "eindex", "extern",
    "float", "for", "goto", "if", "int", "long",
    "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while"
};

//�ָ�����
static int Sparator_Len= 15;//����
static char*  Sparator[] =
{
    "[", "]", "{", "}", ";", "\"", "\'", "#", "(", ")", ".", ":", ",",
    "\\", "\?"
};

//�������
static int Operator_Len= 26;//����
static char* Operator[] =
{
    "+", "-", "*", "/", "<", ">", "=", "^", "&", "|", "%", "~","!",
    ">>", "<<", "++", "--", "&&", "||", "!=", "==", "+=", "-=", "<=", ">=", "->"
};

//��ʶ����㣬�������ɱ�������洢
struct VariableNode
{
    int index;//����
    double Value;//ֵ
    char* name;//������
    int Type;//��������
    struct VariableNode* next;//��һ���
    int keeper;//����
};

static struct VariableNode* head;//ȫ�ֱ�����Ϊ����������̵ı�����ͷ
int InitVarNode(struct WordNode* node);//��ʼ��������
int AddVarNode(struct WordNode* node);//������ڵ����
int SearchVar(struct WordNode*);//�ӱ��������
int Search(struct WordNode*, int);//�ӱ�(�ؼ���,�ָ���,�����)����
int OutPutVar(FILE* out);//�������������
#endif // TABLE_H_
