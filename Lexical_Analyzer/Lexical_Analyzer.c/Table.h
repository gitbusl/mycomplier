//包含词法分析相关表结构
#include "Lex_Head.h"
#ifndef TABLE_H_
#define TABLE_H_

#define KEY         0//关键字
#define SPARATOR    1//分隔符
#define OPERATOR    2//运算符
#define VALUE       3//常量
#define VARIABLE    4//变量

//关键字表
static int Key_Len= 32;//表长度
static char* Key[] =
{
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "eindex", "extern",
    "float", "for", "goto", "if", "int", "long",
    "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while"
};

//分隔符表
static int Sparator_Len= 15;//表长度
static char*  Sparator[] =
{
    "[", "]", "{", "}", ";", "\"", "\'", "#", "(", ")", ".", ":", ",",
    "\\", "\?"
};

//运算符表
static int Operator_Len= 26;//表长度
static char* Operator[] =
{
    "+", "-", "*", "/", "<", ">", "=", "^", "&", "|", "%", "~","!",
    ">>", "<<", "++", "--", "&&", "||", "!=", "==", "+=", "-=", "<=", ">=", "->"
};

//标识符结点，变量名可变用链表存储
struct VariableNode
{
    int index;//索引
    double Value;//值
    char* name;//变量名
    int Type;//变量类型
    struct VariableNode* next;//下一结点
    int keeper;//保留
};

static struct VariableNode* head;//全局变量作为整个编译过程的变量表头
int InitVarNode(struct WordNode* node);//初始化变量表
int AddVarNode(struct WordNode* node);//变量表节点插入
int SearchVar(struct WordNode*);//从变量表查找
int Search(struct WordNode*, int);//从表(关键字,分隔符,运算符)查找
int OutPutVar(FILE* out);//输出变量表内容
#endif // TABLE_H_
