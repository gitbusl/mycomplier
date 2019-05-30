#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED
/**
���ļ�����
    1.�﷨�������õ����ķ�����ʽ
    2.�﷨������Ҫ����غ����Ķ���
    3.�﷨����������ݽṹ�Ķ���,�����õ���ȫ�ֱ����Ķ���
    4.�﷨��������ʱ������Ϣ��
*/

/**
�����ķ�:
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
// �����﷨���Ľ��
struct Syntax_Node
{
    int type;//��������
	int Varnum;//��������ر��еı��
	int number;//�����ڴʴ��еı��
	char* word;//����
	double value;//����������ֵ
	struct Syntax_Node* Bro;//�﷨��Ԫ�ֵܽ��(�ҽڵ�)
	struct Syntax_Node* Son;//�﷨��Ԫ���ӽ��(��ڵ�)
	int len;//�к�
};

//�﷨�����Ĵ�����Ϣ��
static char* Error_Messages[]=
{
    "Unkonwn error!!!\0",
    "The lack of Separator\0",
    "The lack of word\0"
};
//�﷨�����õ���ȫ�ֱ���
static struct wordNode* Now;//ָ����һ�������Ĵʷ���Ԫ���
static struct wordNode* Last;//ָ�����ڷ����Ĵʷ���Ԫ���
static struct Syntax_Node * Last_token;//ָ����һ���������﷨��Ԫ���
static struct Syntax_Node * token;//ָ�����ڷ������﷨��Ԫ���

void Forword();//Forword ȡ����һ�����������﷨��Ԫ���
void UnForword();//UnForword �����﷨��Ԫ���
//����ĵݹ��½��﷨��������
struct Syntax_Node * Program(struct wordNode *head);//Program �﷨��Ԫ�ķ�������
struct Syntax_Node * Res();//Res �﷨��Ԫ�ķ�������
struct Syntax_Node * Re();//Re �﷨��Ԫ�ķ�������
struct Syntax_Node * Main_Func();//Main_Func �﷨��Ԫ�ķ�������
struct Syntax_Node * Block();//Block �﷨��Ԫ�ķ�������
struct Syntax_Node * Decls();//Decls �﷨��Ԫ�ķ�������
struct Syntax_Node * Decl();//Decl �﷨��Ԫ�ķ�������
struct Syntax_Node * Stmts();//Stmts �﷨��Ԫ�ķ�������
struct Syntax_Node * Stmt();//Stmt �﷨��Ԫ�ķ�������
struct Syntax_Node * Bool();//Bool �﷨��Ԫ�ķ�������
struct Syntax_Node * Expr();//Expr �﷨��Ԫ�ķ�������
struct Syntax_Node * Term();//Term �﷨��Ԫ�ķ�������
struct Syntax_Node * Unary();//Unary �﷨��Ԫ�ķ�������
struct Syntax_Node * Factor();//Factor �﷨��Ԫ�ķ�������
void Error(int E_code,struct Syntax_Node *token,struct Syntax_Node *This);//���������
void PrintSyntaxTree(struct Syntax_Node* Head,int Count,FILE*out);//�﷨���Ĵ�ӡ�������
#endif // TREE_H_INCLUDED
