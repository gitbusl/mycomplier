//�ʷ�������������
#include <stdio.h>
#include <stdlib.h>
#include "Lex_Head.h"
#include "Table.h"

struct WordHead *Analyse()
{

	FILE* filepointer;//�ļ�ָ��
	struct WordHead  *Head=(struct WordHead*)malloc(sizeof(struct WordHead));//�ʴ�����ͷ
	if(!Head){printf("�ڴ����ʧ��");}
	struct WordNode* node;//�ʴ��������
	Head->first = Head->last = NULL;
	Head->row = Head->nodecount = 0;//��ʼ���ʴ���
	filepointer = openfile();//���ļ�
	while (1)//ѭ����ȡ���ʲ�����
	{
		node = getWord(filepointer);//��ȡһ�����ʽڵ�
		if(!node)break;//�ļ�����
		AnalyseWord(node);//��������
		addNode(Head, node);//���������ӵ��ʴ�����
	}
	fclose(filepointer);//�ر��ļ�
	OutPutNode(Head,fopen("LEX_List.txt","w"));//����ʷ��������
	OutPutVar(fopen("VAR_Table.txt","w"));//���������
	return Head;
}
