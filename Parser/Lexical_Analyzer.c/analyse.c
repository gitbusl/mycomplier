//�ʷ�������������
#include <stdio.h>
#include <stdlib.h>
#include "Lex_Head.h"
#include "Table.h"

struct wordHead *Analyse()
{

	FILE* filepointer;//�ļ�ָ��
	struct wordHead  *Head=(struct wordHead*)malloc(sizeof(struct wordHead));//�ʴ�����ͷ
	if(!Head){printf("�ڴ����ʧ��");}
	struct wordNode* node;//�ʴ�������
	Head->first = Head->last = NULL;
	Head->row = Head->nodecount = 0;//��ʼ���ʴ���
	filepointer = openfile();//���ļ�
	while (1)//ѭ����ȡ���ʲ�����
	{
		node = getword(filepointer);//��ȡһ�����ʽڵ�
		if(!node)break;//�ļ�����
		Analyseword(node);//��������
		addNode(Head, node);//��������ӵ��ʴ�����
	}
	fclose(filepointer);//�ر��ļ�
	OutPutNode(Head,fopen("LEX_List.txt","w"));//����ʷ��������
	OutPutVar(fopen("VAR_Table.txt","w"));//���������
	return Head;
}

