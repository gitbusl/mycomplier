#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "..\Lexical_Analyzer.c\Table.h"

//语法分析程序
//语法树采用二叉树

//取出语法分析单元      前进一步
int Forword()
{
    if(Now)//词串结束
    {
        Last_token=token;//保存当前token
        token=malloc(sizeof(struct Syntax_Node));//取一个单词
        token->type=Now->type;
        token->Varnum=Now->Varnum;
        token->number=Now->number;
        token->Word=Now->Word;
        token->value=Now->value;
        token->len=Now->row;
        token->Bro=token->Son=NULL;
        Last=Now;//保存上一个词法单元结点
        Now=Now->next;//单词指针后移
    }
    return 0;
}

//回退词法单元结点
int UnForword()
{
    Now=Last;
    free(token);//free掉当前token
    token=Last_token;//从Last_token取出上一个token
    return 0;
}

//Program 语法单元的分析程序
//产生式 Program --> Res Main_Func
//first(Program)=first(Res)
struct Syntax_Node * Program(struct WordNode *head){
    Now=head;
    Last=NULL;//传递词串开始语法分析
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));//创建根节点Program
    struct Syntax_Node* son1=Res();//声明分析子程序
    struct Syntax_Node* son2=Main_Func();//主函数分析子程序
    //将孩子节点与父节点相连
    if(son1){
        This->Son=son1;
        This->Bro=NULL;
        son1->Bro=son2;
    }
    else{
        This->Son=son2;
    }
    //初始化当前节点
    This->Varnum=This->len=This->number=This->type=This->value=0;
    This->Word="Program";
    return This;//函数结束,返回子树
};

//Res 语法单元的分析程序
//产生式 Res--> Re Res | null
//first(Res)=first(Re)='#' follow(Res)=first(Main_Func)=int
struct Syntax_Node * Res(){
    Forword();//取出语法单位
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->Word="Res";//创建当前节点
    if(!strcmp(token->Word,"#")){

        struct Syntax_Node* son1=Re();//声明分析子程序
        struct Syntax_Node* son2=Res();//多重声明
        This->Son=son1;
        son1->Bro=son2;//子树连接
        return This;
    }
    else if(!strcmp(token->Word,"int")){
        return NULL;
    }
    else{
        Error(1,token,This);return NULL;
    }

};
//Re语法单元的分析程序
//产生式 Re--> # include < Vari . h >
//first(Re)='#'
struct Syntax_Node * Re(){
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->Word="Re";
    struct Syntax_Node* son1=token;
    Forword();
    if(!strcmp(token->Word,"include")){
            struct Syntax_Node* son2=token;
            son1->Bro=son2;
            Forword();
        if(!strcmp(token->Word,"<")){
            struct Syntax_Node* son3=token;
            son2->Bro=son3;
            Forword();
            if(token->type==VARIABLE){
                struct Syntax_Node* son4=token;
                son3->Bro=son4;
                Forword();
                if(!strcmp(token->Word,".")){
                struct Syntax_Node* son5=token;
                son4->Bro=son5;
                Forword();
                    if(!strcmp(token->Word,"h")){
                    struct Syntax_Node* son6=token;
                    son5->Bro=son6;
                    Forword();
                        if(!strcmp(token->Word,">")){
                        struct Syntax_Node* son7=token;
                        son6->Bro=son7;
            }else{Error(1,token,This);}
            }else{Error(1,token,This);}
            }else{Error(1,token,This);}
            }else{Error(2,token,This);}
        }else{Error(1,token,This);}
    }
    else{Error(2,token,This);}
    This->Son=son1;
    This->Bro=NULL;
    This->Varnum=This->len=This->number=This->type=This->value=0;
    return This;
};

//Main_Func语法单元的分析程序
//产生式Main_Func-->int main ( ) Block
//first(Main_Func)="int"
struct Syntax_Node * Main_Func(){
    struct Syntax_Node *This=malloc(sizeof(struct Syntax_Node));
    This->Word="Main_Func";
    if(!strcmp(token->Word,"int")){
        struct Syntax_Node* son1=token;
        Forword();
        if(!strcmp(token->Word,"main")){
            struct Syntax_Node *son2=token;
            Forword();
            if(!strcmp(token->Word,"(")){
                struct Syntax_Node *son3=token;
                Forword();
                if(!strcmp(token->Word,")")){
                    struct Syntax_Node *son4=token;
                    struct Syntax_Node *son5=Block();
                    This->Son=son1;
                    son1->Bro=son2;
                    son2->Bro=son3;
                    son3->Bro=son4;
                    son4->Bro=son5;
                    This->Bro=NULL;
                    This->Varnum=This->len=This->number=This->type=This->value=0;
                    return This;
                }
                else{Error(1,token,This);return NULL;}
            }
            else{Error(1,token,This);return NULL;}
        }
        else{Error(2,token,This);return NULL;}
    }
    else{Error(2,token,This);return NULL;}
};

//Block语法单元的分析程序
//Block-->{ Decls Stmts }
//first(Block)="{"
struct Syntax_Node * Block(){
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->Word="Block";
    Forword();
    if(strcmp(token->Word,"{")){Error(1,token,This);return NULL;}
    else{
        struct Syntax_Node* son1=token;
        struct Syntax_Node* son2=Decls();
        struct Syntax_Node* son3=Stmts();
//       Forword();
//        if(strcmp(token->Word,"}")){Error(1,token,This);return NULL;}注释 方便调试
//        else{
//            struct Syntax_Node* son4=token;
            This->Bro=NULL;
            This->Son=son1;
            son1->Bro=son2;
            son2->Bro=son3;
//            son3->Bro=son4;
            son3->Bro=NULL;
            This->Varnum=This->len=This->number=This->type=This->value=0;
            return This;
 //       }
    }
};

//Decls语法单元的分析程序
//产生式Decls   --> Decl Decls | null
//first(Decls)=first(Decl)=Basic follow(Decls)=first(Stmts)=first(Stmt)={"if","while","basic"}
struct Syntax_Node * Decls(){
    Forword();
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->Word="Decls";
    if(token->type==KEY)//first(Decls)=basic  怎么解决？？？
    {
        struct Syntax_Node *son1=Decl();
        struct Syntax_Node *son2=Decls();
        This->Son=son1;
        This->Bro=NULL;
        son1->Bro=son2;
        son2->Bro=NULL;
        This->Varnum=This->len=This->number=This->type=This->value=0;
        return This;
    }
    else if(token->Word=="if"){
        UnForword();
        return NULL;
    }
    else{Error(0,token,This);return This;}//返回空 follow集合不正确

};


//Decl语法单元的分析程序
//产生式Decl    --> Basic Vari Init ;
//first(Decl)=Basic
struct Syntax_Node * Decl(){
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->Word="Decl";
    struct Syntax_Node *son1=token;
    Forword();
    if(token->type==VARIABLE){//token是变量
        struct Syntax_Node *son2=token;
        Forword();
        if(token->type==VALUE){//token 是值
            struct Syntax_Node *son3=token;
            Forword();
            if(token->Word==";"){
                struct Syntax_Node *son4=token;
                This->Son=son1;
                This->Bro=NULL;
                son1->Bro=son2;
                son2->Bro=son3;
                son3->Bro=son4;
                son4->Bro=NULL;
                This->Varnum=This->len=This->number=This->type=This->value=0;
                return This;
            }
        }
    }else{Error(2,token,This);return NULL;}

};

//Init语法单元的分析程序
//产生式Init    --> = Bool
//first(Init)="="
struct Syntax_Node * Init(){};

//Stmts语法单元的分析程序
//产生式Stmts--> Stmt Stmts | null
//first(Stmts)=first(Stmt)="if" follow(Decls)="}"
struct Syntax_Node * Stmts(){
struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
This->Word="Stmts";
return This;
};

//Stmt语法单元的分析程序
/*产生式Stmt    --> if ( Bool ) Stmt ELSE
        --> while ( Bool ) Stmt
        --> Block
        --> Loc = Bool ;
        --> Return Bool ;*/
//first(Stmt)={"if","while","{",Vari,"Return"}
struct Syntax_Node * Stmt(){};

//Loc语法单元的分析程序
//产生式Loc     --> Vari
//first(Loc)=Vari
struct Syntax_Node * Loc(){};

//Bool语法单元的分析程序
//产生式Bool    --> Expr == Expr | Expr != Expr | Expr > Expr | Expr < Expr | Expr
//first(Bool)=first(Expr)=first(Term)=first(Unary)=first(Factor)="("
struct Syntax_Node * Bool(){};

//Expr语法单元的分析程序
//产生式Expr    --> Term + Expr | Term - Expr | Term
//first(Expr)=first(Term)=first(Unary)=first(Factor)="("
struct Syntax_Node * Expr(){};

//Term语法单元的分析程序
//产生式Term    --> Unary * Term | Unary / Term | Unary
//first(Term)=first(Unary)=first(Factor)="("
struct Syntax_Node * Term(){};

//Unary语法单元的分析程序
//产生式Unary   --> ! Factor | - Factor | Factor ++ | Factor -- | Factor
//first(Unary)=first(Factor)="("
struct Syntax_Node * Unary(){};

//Factor语法单元的分析程序
//产生式Factor  --> ( Bool ) | Vari | Value
//first(Factor)="("
struct Syntax_Node * Factor(){};

//错误处理子程序
int Error(int E_code,struct Syntax_Node *e,struct Syntax_Node *This)
{
    printf("in func:%s\t%d\n\tERROR:\tCode:%d %s\n",This->Word,e->len,E_code,Error_Messages[E_code]);
    return 0;
}


//打印空格,打印语法树时调用,让语法树对齐
int PrintSpace(int Count,FILE*out)
{
    int i=0;
    for(i=0; i<Count; i++)
        fprintf(out," ");
    return 0;
}
//打印语法树
int PrintSyntaxTree(struct Syntax_Node* Head,int Count,FILE*out)
{
    if(Head)
    {
        int i=0;
        i=fprintf(out,"(%s)",Head->Word);
        if(Head->Bro)
            fprintf(out,"->");
        PrintSyntaxTree(Head->Bro,Count+i+2,out);
        if(Head->Son)
        {
            fprintf(out,"\n");
            PrintSpace(Count+3,out);
            fprintf(out,"|");
            PrintSyntaxTree(Head->Son,Count,out);
        }
    }
    return 0;
}
