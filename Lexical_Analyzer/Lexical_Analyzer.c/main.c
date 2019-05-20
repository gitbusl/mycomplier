//词法分析器主程序
#include <stdio.h>
#include <stdlib.h>
#include "Lex_Head.h"
#include "Table.h"

int main()
{
    //rowcount++;
    //printf("%d",rowcount);system("pause");
	FILE* filepointer;//文件指针
	struct WordHead  Head;//词串链表头
	struct WordNode* node;//词串链表结点
	Head.first = Head.last = NULL;
	Head.row = Head.nodecount = 0;//初始化词串表
	filepointer = openfile();//打开文件
	while (1)//循环获取单词并分析
	{
		node = getWord(filepointer);//读取一个单词节点
		if(!node)break;//文件结束
		AnalyseWord(node);//分析单词
		addNode(&Head, node);//将当初添加到词串链表
	}
	fclose(filepointer);//关闭文件
	OutPutNode(&Head,fopen("LEX_List.txt","w"));//输出词法分析结果
	OutPutVar(fopen("VAR_Table.txt","w"));//输出变量表
	return 0;
}

