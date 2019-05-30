#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED
/**
此文件包含
    1.语法分析所用到的文法产生式
    2.语法分析需要的相关函数的定义
    3.语法分析相关数据结构的定义,包括用到的全局变量的定义
    4.语法分析错误时错误信息表
*/

/**
所用文法:
Program --> Res Main_Func
Res     --> Re Res | null
Re      --> # include < Vari . h >
Main_Func --> int main ( ) Block
Block   --> { Decls Stmts }
Decls   --> Decl Decls | null
Decl    --> Basic Vari = Val ;
Stmts   --> Stmt Stmts | null
Stmt    --> if ( Bool ) block ELSE
        --> while ( Bool ) block
        --> Vari = Bool ;!
        --> Return 0 ;
		--> Vari("something");
ELSE    --> else block
Bool    --> Vari == Value | Vari != Value | Vari > Value | Vari < Value
Expr    --> Term + Expr | Term - Expr | Term
Term    --> Unary * Term | Unary / Term | Unary
Unary   --> ! Factor | - Factor | Factor ++ | Factor -- | Factor
Factor  --> ( Expr ) | Vari | Value
*/
// 二叉语法树的结点
struct Syntax_Node
{
    int type;//单词类型
	int Varnum;//单词在相关表中的编号
	int number;//单词在词串中的编号
	char* word;//单词
	double value;//变量或常量的值
	struct Syntax_Node* Bro;//语法单元兄弟结点(右节点)
	struct Syntax_Node* Son;//语法单元孩子结点(左节点)
	int len;//行号
};

//语法分析的错误信息表
static char* Error_Messages[]=
{
    "Unkonwn error!!!\0",
    "The lack of Separator\0",
    "The lack of word\0"
};
//语法分析用到的全局变量
static struct wordNode* Now;//指向下一个分析的词法单元结点
static struct wordNode* Last;//指向正在分析的词法单元结点
static struct Syntax_Node * Last_token;//指向上一个分析的语法单元结点
static struct Syntax_Node * token;//指向正在分析的语法单元结点

void Forword();//Forword 取出下一个待分析的语法单元结点
void UnForword();//UnForword 回退语法单元结点
//具体的递归下降语法分析函数
struct Syntax_Node * Program(struct wordNode *head);//Program 语法单元的分析程序
struct Syntax_Node * Res();//Res 语法单元的分析程序
struct Syntax_Node * Re();//Re 语法单元的分析程序
struct Syntax_Node * Main_Func();//Main_Func 语法单元的分析程序
struct Syntax_Node * Block();//Block 语法单元的分析程序
struct Syntax_Node * Decls();//Decls 语法单元的分析程序
struct Syntax_Node * Decl();//Decl 语法单元的分析程序
struct Syntax_Node * Stmts();//Stmts 语法单元的分析程序
struct Syntax_Node * Stmt();//Stmt 语法单元的分析程序
struct Syntax_Node * Bool();//Bool 语法单元的分析程序
struct Syntax_Node * Expr();//Expr 语法单元的分析程序
struct Syntax_Node * Term();//Term 语法单元的分析程序
struct Syntax_Node * Unary();//Unary 语法单元的分析程序
struct Syntax_Node * Factor();//Factor 语法单元的分析程序
void Error(int E_code,struct Syntax_Node *token,struct Syntax_Node *This);//错误处理程序
void PrintSyntaxTree(struct Syntax_Node* Head,int Count,FILE*out);//语法树的打印输出程序
#endif // TREE_H_INCLUDED
