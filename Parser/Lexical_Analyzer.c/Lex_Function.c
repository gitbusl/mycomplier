//此文件包含词法分析相关函数
#include "Lex_Head.h"
#include "Table.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

//文件打开函数
FILE* openfile()
{
    static char filename[127]="HelloWorld.c";//文件名数组
//    printf("Input The Filename To Analyze:\neg: HelloWorld.c\n");//提示输入文件名
//    scanf("%s", filename);
    FILE* filepointer = fopen(filename, "r");
    if (filepointer){return filepointer;}//返回文件指针
    else{printf("File open error!!!");return NULL;}//提示文件打开失败
}


//运算符判断函数   判断是否为双字符的运算符/分隔符  输出0为假,1为真
int isDoubleOpe(char c1, char c2)
{
	int C0 = ispunct(c2);//首先判断c2是否为符号(非数字,非字母,非控制符)
	int C1 = (c1 == '+' || c1 == '-' || c1 == '=' || c1 == '<' || c1 == '>' || c1 == '!') && c2 == '=';
	int C2 = (c1 == '|' || c1 == '+' || c1 == '-' || c1 == '&' || c1 == '<' || c1 == '>') && c2 == c1;
	int C3 = (c1 == '\\') && (c2 == c1 || c2 == '?' || c2 == '*');//没看懂
	int C4 = c1 == '-' && c2 == '>';
	int C = C0 && (C1 || C2 || C3 || C4);
	return C;
}

//扫描子程序，单词读取函数  读取一个单词  输出 词串结点
struct WordNode* getWord(FILE* filepointer)
{
	char in = 0;//暂存读入字符
	char word[127];//单词
	int i = 0;//单词中字符指针计数
	int Type = 0;//单词类型
	int Value = 0;//常量值
	word[i++] = in = getc(filepointer);//读取第一个字符
	//根据第一个字符判断单词可能的类型
	//文件结束
	if(in==EOF)return NULL;
	//空格,返回下一个单词
	if (isspace(in))
		{ return getWord(filepointer); }
    //换行符
    if (in==10)
		{ rowcount++;return getWord(filepointer); }

    //可能是注释语句
	if (in == '/')
	{
		in = getc(filepointer);//读取一个字,符进一步判断
		//行注释
		if (in == '/')
		{
			while ((in = getc(filepointer)) != EOF)//抛弃本行所有内容
			{
			    if (in == 10)//直到读取到换行符
				{ rowcount++;return getWord(filepointer); }//抛掉注释后,返回一个单词
			}
		}
		//段注释
		else if (in == '*')
			{
				while ((in = getc(filepointer)) != EOF)//抛掉注释段内所有内容
				{
				    if(in==10)rowcount++;
					if (in == '*')//遇到 * 符号,可能是段注释结束
                    {
						if ((in = getc(filepointer)) == '/' )//再读取一个一个字符,进一步判断是否结束
						{
							return getWord(filepointer);//抛掉注释后,返回一个单词
						}
                        else ungetc(in,filepointer);//段注释未结束,放回字符----防止**/结束的情况
                    }
				}
			}
        //不是注释,放回字符.恢复状态
        else
			{
				ungetc(in, filepointer);
				in = word[i - 1];
			}
	}
//是标识符/关键字/常量   isalnum 数字字母判断  isalpha英文字母判断
	if (isalnum(in) || in == '_')
	{
	    //标识符/关键字
		if (in == '_' || isalpha(in))
		{
			Type = VARIABLE;//假设是标识符

			while ((in = getc(filepointer)) != EOF)//读取到标识符结束
			{
				if (isalnum(in) || in == '_')//可能是标识符/关键字的组成
				{
					word[i++] = in;
					continue;
				}

				else//结束,非(数字/字母/'_')
				{
					ungetc(in, filepointer);
					break;
				}
			}
		}
		//常量
		else
		{
			Type = VALUE;
			Value = in - '0';//计算值

			while ((in = getc(filepointer)) != EOF)
			{
				if (isdigit(in))//读取以后所有连续数字
				{
					Value = Value * 10 + in - '0';//计算值
					continue;
				}
				else//遇到非数字字符
				{
					ungetc(in, filepointer);
					break;
				}
			}
		}
	}

//分隔符/运算符
    else
    {
			Type = SPARATOR;//假设是分隔符
			while ((in = getc(filepointer)) != EOF)
			{
				if (isDoubleOpe(word[i - 1], in))//判断是否为双字符分隔符/运算符
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

    //将读取到的单词组装成词串结点
	struct WordNode* node = malloc(sizeof(struct WordNode));

	node->type = Type;//单词类型
    node->row=rowcount;
	node->number = WordCount++;//词串计数加一

	node->Varnum = 0;//单词在表中的序号

	node->next = NULL;//下一结点

		word[i++] = '\0';
		node->Word = malloc(i * sizeof(char));
		strcpy(node->Word, word);
		node->value = Value;
	return node;//返回结点
}

//单词分析函数  进一步分析单词类型   将变量写到表中
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

//词串添加函数    把结点添加到词串链表
void addNode(struct WordHead* Head, struct WordNode* node)
{
	if (Head->nodecount == 0)//词串为空
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

//词串输出函数    将分析得到的单词串输出
int OutPutNode(struct WordHead* Head,FILE* out)
{
	int i = 0;
    printf("\n词法分析结果：\n\n");
	if (Head != NULL)
	{
		struct WordNode* node = Head->first;
        //printf("序号\t行号\t类型\t表序号\t单词\n");
        fprintf(out,"序号\t行号\t类型\t表序号\t单词\n");
		while (node != NULL)
		{
            //printf("%d\t%d\t", node->number,node->row);
			fprintf(out,"%d\t%d\t", node->number,node->row);
			char* T[]={
			"关键字",
			"分隔符",
			"运算符",
			"常  量",
			"标识符"
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
    printf("词法分析成功\n");
	return i;
}
