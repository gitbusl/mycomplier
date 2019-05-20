//�ʷ�������غ��������ݽṹ�Ķ���
#include<stdio.h>
#ifndef LEX_HEAD_H_
#define LEX_HEAD_H_

static int WordCount=0;//���ʴ�����
static int rowcount=1;//�к�

//�ʴ����
struct WordNode
{
	int type;//��������
	int Varnum;//��������ر��еı��
	int number;//�����ڴʴ��еı��
	char* Word;//����
	double value;//����������ֵ
	struct WordNode* next;//��һ���ָ��
	int row;//�к�
};

//�ʴ�ͷ
struct WordHead
{
	struct WordNode * first;//�ʴ���һ�����ʽ��
	struct WordNode * last;//�ʴ����һ�����ʽ��
	int nodecount;//�ʴ����ʼ���
	int row;//����
};

FILE* openfile();//���ļ�
struct WordNode* getWord(FILE* filepointer);//��ȡһ������
void AnalyseWord(struct WordNode* node);//��������
void addNode(struct WordHead* Head, struct WordNode* node);//���ʽ����ӵ��ʴ���
int OutPutNode(struct WordHead* Head,FILE* out);//����ʴ�
#endif // LEX_HEAD_H_
