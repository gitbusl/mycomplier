//�ʷ�������غ��������ݽṹ�Ķ���
#include<stdio.h>
#ifndef LEX_HEAD_H_
#define LEX_HEAD_H_

static int wordCount=0;//���ʴ�����
static int rowcount=1;//�к�

//�ʴ����
struct wordNode
{
	int type;//��������
	int Varnum;//��������ر��еı��
	int number;//�����ڴʴ��еı��
	char* word;//����
	double value;//����������ֵ
	struct wordNode* next;//��һ���ָ��
	int row;//�к�
};

//�ʴ�ͷ
struct wordHead
{
	struct wordNode * first;//�ʴ���һ�����ʽ��
	struct wordNode * last;//�ʴ����һ�����ʽ��
	int nodecount;//�ʴ����ʼ���
	int row;//����
};

FILE* openfile();//���ļ�
struct wordNode* getword(FILE* filepointer);//��ȡһ������
void Analyseword(struct wordNode* node);//��������
void addNode(struct wordHead* Head, struct wordNode* node);//���ʽ����ӵ��ʴ���
int OutPutNode(struct wordHead* Head,FILE* out);//����ʴ�
#endif // LEX_HEAD_H_
