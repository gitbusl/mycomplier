//词法分析相关函数及数据结构的定义
#include<stdio.h>
#ifndef LEX_HEAD_H_
#define LEX_HEAD_H_

static int wordCount=0;//单词串计数
static int rowcount=1;//行号

//词串结点
struct wordNode
{
	int type;//单词类型
	int Varnum;//单词在相关表中的编号
	int number;//单词在词串中的编号
	char* word;//单词
	double value;//变量或常量的值
	struct wordNode* next;//下一结点指针
	int row;//行号
};

//词串头
struct wordHead
{
	struct wordNode * first;//词串第一个单词结点
	struct wordNode * last;//词串最后一个单词结点
	int nodecount;//词串单词计数
	int row;//保留
};

FILE* openfile();//打开文件
struct wordNode* getword(FILE* filepointer);//读取一个单词
void Analyseword(struct wordNode* node);//分析单词
void addNode(struct wordHead* Head, struct wordNode* node);//单词结点添加到词串中
int OutPutNode(struct wordHead* Head,FILE* out);//输出词串
#endif // LEX_HEAD_H_
