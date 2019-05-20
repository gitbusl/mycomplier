//���ļ������ʷ�������غ���
#include "Lex_Head.h"
#include "Table.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

//�ļ��򿪺���
FILE* openfile()
{
    static char filename[127]="HelloWorld.c";//�ļ�������
//    printf("Input The Filename To Analyze:\neg: HelloWorld.c\n");//��ʾ�����ļ���
//    scanf("%s", filename);
    FILE* filepointer = fopen(filename, "r");
    if (filepointer){return filepointer;}//�����ļ�ָ��
    else{printf("File open error!!!");return NULL;}//��ʾ�ļ���ʧ��
}


//������жϺ���   �ж��Ƿ�Ϊ˫�ַ��������/�ָ���  ���0Ϊ��,1Ϊ��
int isDoubleOpe(char c1, char c2)
{
	int C0 = ispunct(c2);//�����ж�c2�Ƿ�Ϊ����(������,����ĸ,�ǿ��Ʒ�)
	int C1 = (c1 == '+' || c1 == '-' || c1 == '=' || c1 == '<' || c1 == '>' || c1 == '!') && c2 == '=';
	int C2 = (c1 == '|' || c1 == '+' || c1 == '-' || c1 == '&' || c1 == '<' || c1 == '>') && c2 == c1;
	int C3 = (c1 == '\\') && (c2 == c1 || c2 == '?' || c2 == '*');//û����
	int C4 = c1 == '-' && c2 == '>';
	int C = C0 && (C1 || C2 || C3 || C4);
	return C;
}

//ɨ���ӳ��򣬵��ʶ�ȡ����  ��ȡһ������  ��� �ʴ����
struct WordNode* getWord(FILE* filepointer)
{
	char in = 0;//�ݴ�����ַ�
	char word[127];//����
	int i = 0;//�������ַ�ָ�����
	int Type = 0;//��������
	int Value = 0;//����ֵ
	word[i++] = in = getc(filepointer);//��ȡ��һ���ַ�
	//���ݵ�һ���ַ��жϵ��ʿ��ܵ�����
	//�ļ�����
	if(in==EOF)return NULL;
	//�ո�,������һ������
	if (isspace(in))
		{ return getWord(filepointer); }
    //���з�
    if (in==10)
		{ rowcount++;return getWord(filepointer); }

    //������ע�����
	if (in == '/')
	{
		in = getc(filepointer);//��ȡһ����,����һ���ж�
		//��ע��
		if (in == '/')
		{
			while ((in = getc(filepointer)) != EOF)//����������������
			{
			    if (in == 10)//ֱ����ȡ�����з�
				{ rowcount++;return getWord(filepointer); }//�׵�ע�ͺ�,����һ������
			}
		}
		//��ע��
		else if (in == '*')
			{
				while ((in = getc(filepointer)) != EOF)//�׵�ע�Ͷ�����������
				{
				    if(in==10)rowcount++;
					if (in == '*')//���� * ����,�����Ƕ�ע�ͽ���
                    {
						if ((in = getc(filepointer)) == '/' )//�ٶ�ȡһ��һ���ַ�,��һ���ж��Ƿ����
						{
							return getWord(filepointer);//�׵�ע�ͺ�,����һ������
						}
                        else ungetc(in,filepointer);//��ע��δ����,�Ż��ַ�----��ֹ**/���������
                    }
				}
			}
        //����ע��,�Ż��ַ�.�ָ�״̬
        else
			{
				ungetc(in, filepointer);
				in = word[i - 1];
			}
	}
//�Ǳ�ʶ��/�ؼ���/����   isalnum ������ĸ�ж�  isalphaӢ����ĸ�ж�
	if (isalnum(in) || in == '_')
	{
	    //��ʶ��/�ؼ���
		if (in == '_' || isalpha(in))
		{
			Type = VARIABLE;//�����Ǳ�ʶ��

			while ((in = getc(filepointer)) != EOF)//��ȡ����ʶ������
			{
				if (isalnum(in) || in == '_')//�����Ǳ�ʶ��/�ؼ��ֵ����
				{
					word[i++] = in;
					continue;
				}

				else//����,��(����/��ĸ/'_')
				{
					ungetc(in, filepointer);
					break;
				}
			}
		}
		//����
		else
		{
			Type = VALUE;
			Value = in - '0';//����ֵ

			while ((in = getc(filepointer)) != EOF)
			{
				if (isdigit(in))//��ȡ�Ժ�������������
				{
					Value = Value * 10 + in - '0';//����ֵ
					continue;
				}
				else//�����������ַ�
				{
					ungetc(in, filepointer);
					break;
				}
			}
		}
	}

//�ָ���/�����
    else
    {
			Type = SPARATOR;//�����Ƿָ���
			while ((in = getc(filepointer)) != EOF)
			{
				if (isDoubleOpe(word[i - 1], in))//�ж��Ƿ�Ϊ˫�ַ��ָ���/�����
				{
					word[i++] = in;
					break;
				}
				else
				{
					ungetc(in, filepointer);
					break;
				}
			}
    }

    //����ȡ���ĵ�����װ�ɴʴ����
	struct WordNode* node = malloc(sizeof(struct WordNode));

	node->type = Type;//��������
    node->row=rowcount;
	node->number = WordCount++;//�ʴ�������һ

	node->Varnum = 0;//�����ڱ��е����

	node->next = NULL;//��һ���

		word[i++] = '\0';
		node->Word = malloc(i * sizeof(char));
		strcpy(node->Word, word);
		node->value = Value;
	return node;//���ؽ��
}

//���ʷ�������  ��һ��������������   ������д������
void AnalyseWord(struct WordNode* node)
{
	if (node->type == VARIABLE)
	{
		if ((node->Varnum = Search(node, KEY)) != -1)
		{ node->type = KEY; }
		else { node->Varnum = SearchVar(node); }
	}

	else
		if (node->type == SPARATOR)
		{
			if ((node->Varnum = Search(node, OPERATOR)) != -1)
			{
				node->type = OPERATOR;
			}

			else { node->Varnum = Search(node, SPARATOR); }
		}
}

//�ʴ���Ӻ���    �ѽ����ӵ��ʴ�����
void addNode(struct WordHead* Head, struct WordNode* node)
{
	if (Head->nodecount == 0)//�ʴ�Ϊ��
	{
		Head->first = Head->last = node;
		node->next = NULL;

	}
	else
    {
	struct WordNode * temp = Head->last;
	temp->next = node;
	node->next = NULL;
	Head->last = node;
    }
	Head->nodecount++;
}

//�ʴ��������    �������õ��ĵ��ʴ����
int OutPutNode(struct WordHead* Head,FILE* out)
{
	int i = 0;
    printf("\n�ʷ����������\n\n");
	if (Head != NULL)
	{
		struct WordNode* node = Head->first;
        //printf("���\t�к�\t����\t�����\t����\n");
        fprintf(out,"���\t�к�\t����\t�����\t����\n");
		while (node != NULL)
		{
            //printf("%d\t%d\t", node->number,node->row);
			fprintf(out,"%d\t%d\t", node->number,node->row);
			char* T[]={
			"�ؼ���",
			"�ָ���",
			"�����",
			"��  ��",
			"��ʶ��"
			};
			char *Type=T[node->type];
			//printf("%s\t%d\t", Type, node->Varnum);
			fprintf(out,"%s\t%d\t", Type, node->Varnum);

			if (node->type == VALUE) {
                    //printf("%f\n", node->value);
                    fprintf(out,"%f\n", node->value); }
			else
			{
			    //printf("%s\n", node->Word);
			    fprintf(out,"%s\n", node->Word); }
			i++;
			node = node->next;
		}
	}
    printf("�ʷ������ɹ�\n");
	return i;
}
