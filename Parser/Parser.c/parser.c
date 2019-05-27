#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "..\Lexical_Analyzer.c\Table.h"

//语法分析程序
//语法树采用二叉树

//取出语法分析单元      前进一步
void Forword()
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
}

//回退词法单元结点
void UnForword()
{
    Now=Last;
    free(token);//free掉当前token
    token=Last_token;//从Last_token取出上一个token
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
            if(son2){
                son1->Bro=son2;
                son2->Bro=son3;
            }
            else{son1->Bro=son3;}

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
    if(!strcmp(token->Word,"int"))//first(Decls)=basic  怎么解决？？？
    {
        struct Syntax_Node *son1=Decl();
        struct Syntax_Node *son2=Decls();
        This->Son=son1;
        This->Bro=NULL;
        son1->Bro=son2;
        if(son2){son2->Bro=NULL;}
        This->Varnum=This->len=This->number=This->type=This->value=0;
        return This;
    }
    else if(!strcmp(token->Word,"if")){
        UnForword();
        return NULL;
    }
    else{UnForword();return NULL;}//返回空 follow集合不正确

};


//Decl语法单元的分析程序
//产生式Decl    --> Basic Vari = Val ;
//first(Decl)=Basic
struct Syntax_Node * Decl(){
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->Word="Decl";
    struct Syntax_Node *son1=token;
    Forword();
    if(token->type==VARIABLE){//token是变量
        struct Syntax_Node *son2=token;
        Forword();
        if(!strcmp(token->Word,"=")){//改动文法
            struct Syntax_Node *son3=token;
            Forword();
            if(token->type==VALUE){
                    struct Syntax_Node *son4=token;
                    Forword();
                if(!strcmp(token->Word,";")){
                struct Syntax_Node *son5=token;
                This->Son=son1;
                This->Bro=NULL;
                son1->Bro=son2;
                son2->Bro=son3;
                son3->Bro=son4;
                son4->Bro=son5;
                son5->Bro=NULL;
                This->Varnum=This->len=This->number=This->type=This->value=0;
                return This;
                }else{Error(2,token,This);return NULL;}
            }
        }else{Error(2,token,This);return NULL;}
    }else{Error(2,token,This);return NULL;}
};

//Stmts语法单元的分析程序
//产生式Stmts--> Stmt Stmts | null
//first(Stmts)=first(Stmt)={if,while,Vari,return} follow(Decls)="}"
struct Syntax_Node * Stmts(){
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->Word="Stmts";
    Forword();
    if(!strcmp(token->Word,"if")||!strcmp(token->Word,"while")||!strcmp(token->Word,"return")||(token->type==VARIABLE)){
        struct Syntax_Node* son1=Stmt();
        struct Syntax_Node* son2=Stmts();
        This->Bro=NULL;
        This->Son=son1;
        son1->Bro=son2;
        if(son2)son2->Bro=NULL;
        return This;
    }
    else{
        UnForword();
        Error(2,token,This);
        return NULL;
    }

};

//Stmt语法单元的分析程序
/*产生式Stmt    --> if ( Bool ) Stmt ELSE
        --> while ( Bool ) Stmt
        --> Block
        --> Loc = Bool ;
        --> Return Bool ;*/
//first(Stmt)={"if","while","{",Vari,"Return"}
struct Syntax_Node * Stmt(){
    char str[128];
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->Word="Stmt";
    struct Syntax_Node* son1=token;
    if(!strcmp(token->Word,"if")){}
    else if(!strcmp(token->Word,"while")){}
    else if(!strcmp(token->Word,"return")){
        Forword();
        if(!strcmp(token->Word,"0")){
            struct Syntax_Node* son2=token;
            Forword();
            if(!strcmp(token->Word,";")){
                struct Syntax_Node* son3=token;
                This->Bro=NULL;
                This->Son=son1;
                son1->Bro=son2;
                son2->Bro=son3;
                return This;
            }
        }

    }
    else if(token->type==VARIABLE){
        Forword();
        if(!strcmp(token->Word,"=")){
            struct Syntax_Node* son2=token;
            struct Syntax_Node* son3=Bool();
            Forword();
 //           if(!strcmp(token->Word,";")){
 //               struct Syntax_Node* son4=token;
                This->Bro=NULL;
                This->Son=son1;
                son1->Bro=son2;
                son2->Bro=son3;
   //             son3->Bro=son4;
  //          }
        }
        else if(!strcmp(token->Word,"(")){
            struct Syntax_Node* son2=token;
            Forword();
            struct Syntax_Node* son3=token;
            if(!strcmp(token->Word,"\"")){
                strcpy(&str,"\"");
                Forword();
                while(strcmp(token->Word,"\"")){
                    strcat(&str,token->Word);
                    Forword();
                }
                strcat(&str,"\"");
                strcpy(son3->Word,str);
                Forword();
                if(!strcmp(token->Word,")")){
                    struct Syntax_Node* son4=token;
                    Forword();
                    if(!strcmp(token->Word,";")){
                    struct Syntax_Node* son5=token;
                    This->Bro=NULL;
                    This->Son=son1;
                    son1->Bro=son2;
                    son2->Bro=son3;
                    son3->Bro=son4;
                    son4->Bro=son5;
                    return This;
                    }
                }
            }
        }
    }
    else{Error(2,token,This);return NULL;}
    return This;
};

//Bool语法单元的分析程序
//产生式Bool    --> Expr == Expr | Expr != Expr | Expr > Expr | Expr < Expr | Expr
//first(Bool)=first(Expr)=first(Term)=first(Unary)=first(Factor)="("
struct Syntax_Node * Bool(){
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->Word="Bool";
    return This;
};

//Expr语法单元的分析程序
//产生式Expr    --> Term + Expr | Term - Expr | Term
//first(Expr)=first(Term)=first(Unary)=first(Factor)="("
struct Syntax_Node * Expr(){
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->Word="Expr";
    return This;
};

//Term语法单元的分析程序
//产生式Term    --> Unary * Term | Unary / Term | Unary
//first(Term)=first(Unary)=first(Factor)="("
struct Syntax_Node * Term(){
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->Word="Term";
    return This;
};

//Unary语法单元的分析程序
//产生式Unary   --> ! Factor | - Factor | Factor ++ | Factor -- | Factor
//first(Unary)=first(Factor)="("
struct Syntax_Node * Unary(){
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->Word="Unary";
    return This;
};

//Factor语法单元的分析程序
//产生式Factor  --> ( Bool ) | Vari | Value
//first(Factor)="("
struct Syntax_Node * Factor(){
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->Word="Factor";
    return This;
};

//错误处理子程序
void Error(int E_code,struct Syntax_Node *e,struct Syntax_Node *This)
{
    printf("in func:%s\t%d\n\tERROR:\tCode:%d %s\n",This->Word,e->len,E_code,Error_Messages[E_code]);
}


//打印空格,打印语法树时调用,让语法树对齐
void PrintSpace(int Count,FILE*out)
{
    int i=0;
    for(i=0; i<Count; i++)
        fprintf(out," ");
}
//打印语法树
void PrintSyntaxTree(struct Syntax_Node* Head,int Count,FILE*out)
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
}
