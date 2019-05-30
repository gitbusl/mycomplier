#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "..\Lexical_Analyzer.c\Table.h"

//语法分析程序
//语法树采用二叉树

//取出语法分析单元      前进一步
void Advance()
{
    if(Now)//词串结束
    {
        Last_token=token;//保存当前token
        token=malloc(sizeof(struct Syntax_Node));//取一个单词
        token->type=Now->type;
        token->Varnum=Now->Varnum;
        token->number=Now->number;
        token->word=Now->word;
        token->value=Now->value;
        token->len=Now->row;
        token->Bro=token->Son=NULL;
        Last=Now;//保存上一个词法单元结点
        Now=Now->next;//单词指针后移
    }
}

//回退词法单元结点
void Unadvance()
{
    Now=Last;
    free(token);//free掉当前token
    token=Last_token;//从Last_token取出上一个token
}

//Program 语法单元的分析程序
//产生式 Program --> Res Main_Func
//first(Program)=first(Res)
struct Syntax_Node * Program(struct wordNode *head){
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
    This->word="Program";
    return This;//函数结束,返回子树
};

//Res 语法单元的分析程序
//产生式 Res--> Re Res | null
//first(Res)=first(Re)='#' follow(Res)=first(Main_Func)=int
struct Syntax_Node * Res(){
    Advance();//取出语法单位
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->word="Res";//创建当前节点
    if(!strcmp(token->word,"#")){

        struct Syntax_Node* son1=Re();//声明分析子程序
        struct Syntax_Node* son2=Res();//多重声明
        This->Son=son1;
        son1->Bro=son2;//子树连接
        return This;
    }
    else if(!strcmp(token->word,"int")){
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
    This->word="Re";
    struct Syntax_Node* son1=token;
    Advance();
    if(!strcmp(token->word,"include")){
            struct Syntax_Node* son2=token;
            son1->Bro=son2;
            Advance();
        if(!strcmp(token->word,"<")){
            struct Syntax_Node* son3=token;
            son2->Bro=son3;
            Advance();
            if(token->type==VARIABLE){
                struct Syntax_Node* son4=token;
                son3->Bro=son4;
                Advance();
                if(!strcmp(token->word,".")){
                struct Syntax_Node* son5=token;
                son4->Bro=son5;
                Advance();
                    if(!strcmp(token->word,"h")){
                    struct Syntax_Node* son6=token;
                    son5->Bro=son6;
                    Advance();
                        if(!strcmp(token->word,">")){
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
    return This;
};

//Main_Func语法单元的分析程序
//产生式Main_Func-->int main ( ) Block
//first(Main_Func)="int"
struct Syntax_Node * Main_Func(){
    struct Syntax_Node *This=malloc(sizeof(struct Syntax_Node));
    This->word="Main_Func";
    if(!strcmp(token->word,"int")){
        struct Syntax_Node* son1=token;
        Advance();
        if(!strcmp(token->word,"main")){
            struct Syntax_Node *son2=token;
            Advance();
            if(!strcmp(token->word,"(")){
                struct Syntax_Node *son3=token;
                Advance();
                if(!strcmp(token->word,")")){
                    struct Syntax_Node *son4=token;
                    struct Syntax_Node *son5=Block();
                    This->Son=son1;
                    son1->Bro=son2;
                    son2->Bro=son3;
                    son3->Bro=son4;
                    son4->Bro=son5;
                    This->Bro=NULL;
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
    This->word="Block";
    Advance();
    if(strcmp(token->word,"{")){Error(1,token,This);return NULL;}
    else{
        struct Syntax_Node* son1=token;
        struct Syntax_Node* son2=Decls();
        struct Syntax_Node* son3=Stmts();
       Advance();
        if(strcmp(token->word,"}")){Error(1,token,This);return NULL;}//注释 方便调试
        else{
            struct Syntax_Node* son4=token;
            This->Bro=NULL;
            This->Son=son1;
            if(son2){
                son1->Bro=son2;
                son2->Bro=son3;
            }
            else{son1->Bro=son3;}

            son3->Bro=son4;
            return This;
        }
    }
};

//Decls语法单元的分析程序
//产生式Decls   --> Decl Decls | null
//first(Decls)=first(Decl)=Basic follow(Decls)=first(Stmts)=first(Stmt)={"if","while","basic"}
struct Syntax_Node * Decls(){
    Advance();
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->word="Decls";
    if(!strcmp(token->word,"int"))//first(Decls)=basic  怎么解决？？？
    {
        struct Syntax_Node *son1=Decl();
        struct Syntax_Node *son2=Decls();
        This->Son=son1;
        This->Bro=NULL;
        son1->Bro=son2;
        if(son2){son2->Bro=NULL;}
        return This;
    }
    else if(!strcmp(token->word,"if")){
        Unadvance();
        return NULL;
    }
    else{Unadvance();return NULL;}//返回空 follow集合不正确

};


//Decl语法单元的分析程序
//产生式Decl    --> Basic Vari = Val ;
//first(Decl)=Basic
struct Syntax_Node * Decl(){
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->word="Decl";
    struct Syntax_Node *son1=token;
    Advance();
    if(token->type==VARIABLE){//token是变量
        struct Syntax_Node *son2=token;
        Advance();
        if(!strcmp(token->word,"=")){//改动文法
            struct Syntax_Node *son3=token;
            Advance();
            if(token->type==VALUE){
                    struct Syntax_Node *son4=token;
                    Advance();
                if(!strcmp(token->word,";")){
                struct Syntax_Node *son5=token;
                This->Son=son1;
                This->Bro=NULL;
                son1->Bro=son2;
                son2->Bro=son3;
                son3->Bro=son4;
                son4->Bro=son5;
                son5->Bro=NULL;
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
    This->word="Stmts";
    Advance();
    if(!strcmp(token->word,"if")||!strcmp(token->word,"while")||!strcmp(token->word,"return")||(token->type==VARIABLE)){
        struct Syntax_Node* son1=Stmt();
        struct Syntax_Node* son2=Stmts();
        This->Bro=NULL;
        This->Son=son1;
        son1->Bro=son2;
        if(son2)son2->Bro=NULL;
        return This;
    }
    else{
        Unadvance();
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
    This->word="Stmt";
    struct Syntax_Node* son1=token;
    if(!strcmp(token->word,"if")){
        son1=token;//'if' 放到子树
        Advance();
        if(strcmp(token->word,"("))//'('放到子树
            Error(1,token,This);
        struct Syntax_Node* son2=token;
        struct Syntax_Node* son3=Bool();
        Advance();
        if(strcmp(token->word,")"))
            Error(1,token,This);
        struct Syntax_Node* son4=token;
        struct Syntax_Node* son5=Block();
        Advance();
        if(!strcmp(token->word,"else"))//判断是否有else语句
        {
            struct Syntax_Node* son6=token;
            struct Syntax_Node* son7=Block();
            This->Bro=NULL;
            This->Son=son1;
            son1->Bro=son2;
            son2->Bro=son3;
            son3->Bro=son4;
            son4->Bro=son5;
            son5->Bro=son6;
            son6->Bro=son7;
            return This;
        }
        Unadvance();
        This->Bro=NULL;
        This->Son=son1;
        son1->Bro=son2;
        son2->Bro=son3;
        son3->Bro=son4;
        son4->Bro=son5;
        son5->Bro=NULL;
    }
    else if(!strcmp(token->word,"while")){
        Advance();
        if(strcmp(token->word,"("))
           Error(1,token,This);
        struct Syntax_Node* son2=token;
        struct Syntax_Node* son3=Bool();
        Advance();
        if(strcmp(token->word,")"))
            Error(1,token,This);
        struct Syntax_Node* son4=Block();
        This->Bro=NULL;
        This->Son=son1;
        son1->Bro=son2;
        son2->Bro=son3;
        son3->Bro=son4;
    }
    else if(!strcmp(token->word,"return")){
        Advance();
        if(!strcmp(token->word,"0")){
            struct Syntax_Node* son2=token;
            Advance();
            if(!strcmp(token->word,";")){
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
        Advance();
        if(!strcmp(token->word,"=")){
            struct Syntax_Node* son2=token;
            struct Syntax_Node* son3=Expr();
            Advance();
            if(!strcmp(token->word,";")){
                struct Syntax_Node* son4=token;
                This->Bro=NULL;
                This->Son=son1;
                son1->Bro=son2;
                son2->Bro=son3;
                son3->Bro=son4;
                return This;
            }
        }
        else if(!strcmp(token->word,"(")){
            struct Syntax_Node* son2=token;
            Advance();
            struct Syntax_Node* son3=token;
            if(!strcmp(token->word,"\"")){
                strcpy(&str,"\"");
                Advance();
                while(strcmp(token->word,"\"")){
                    strcat(&str,token->word);
                    Advance();
                }
                strcat(&str,"\"");
                strcpy(son3->word,str);
                Advance();
                if(!strcmp(token->word,")")){
                    struct Syntax_Node* son4=token;
                    Advance();
                    if(!strcmp(token->word,";")){
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
//产生式Bool    --> Vari == Value | Vari != Value | Vari > Value | Vari < Value
//first(Bool)=first(Expr)=first(Term)=first(Unary)={}
struct Syntax_Node * Bool(){
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->word="Bool";
    Advance();
    if(token->type==VARIABLE){
        struct Syntax_Node* son1=token;
        Advance();
        if(!(strcmp(token->word,"==")&&strcmp(token->word,"!=")
            &&strcmp(token->word,">")&&strcmp(token->word,"<")))
        {
            struct Syntax_Node* son2=token;
            Advance();
            if(token->type==VALUE){
                struct Syntax_Node* son3=token;
                This->Bro=NULL;
                This->Son=son1;
                son1->Bro=son2;
                son2->Bro=son3;
                return This;
            }
        }
    }
};

//Expr语法单元的分析程序
//产生式Expr    --> Term + Expr | Term - Expr | Term
//first(Expr)=first(Term)=first(Unary)=first(Factor)="("
struct Syntax_Node * Expr(){
    struct Syntax_Node* son1=Term();
    Advance();
    if(!(strcmp(token->word,"+")&&strcmp(token->word,"-")))
    {
        struct Syntax_Node* son2=token;
        son1->Bro=son2;
        son2=son2->Bro=Expr();
    }
    else
        Unadvance();
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->Bro=NULL;
    This->Son=son1;
    This->word="Expr";
    return This;
};

//Term语法单元的分析程序
//产生式Term    --> Unary * Term | Unary / Term | Unary
//first(Term)=first(Unary)=first(Factor)="("
struct Syntax_Node * Term(){
    struct Syntax_Node* son1=Unary();
    Advance();
    if(!(strcmp(token->word,"*")&&strcmp(token->word,"/")))
    {
        struct Syntax_Node*son2=token;
        son1->Bro=son2;
        son2=son2->Bro=Term();
    }
    else
        Unadvance();
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    This->Bro=NULL;
    This->Son=son1;
    This->word="Term";
    return This;
};

//Unary语法单元的分析程序
//产生式Unary   --> ! Factor | - Factor | Factor ++ | Factor -- | Factor
//first(Unary)=first(Factor)="("
struct Syntax_Node * Unary(){
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    Advance();
    struct Syntax_Node* son1=NULL;
    if(!(strcmp(token->word,"!")&&strcmp(token->word,"-")))
    {
        son1=token;
        Advance();
        if(!strcmp(token->word,"(")||token->type==VARIABLE||token->type==VALUE)
        {
            son1->Bro=Factor();
        }
        else
            Error(1,token,This);
    }
    else if(!strcmp(token->word,"(")||token->type==VARIABLE||token->type==VALUE)
    {
        son1=Factor();
        Advance();
        if(!strcmp(token->word,"++")||!strcmp(token->word,"--"))
            son1->Bro=token;
        else
            Unadvance();
    }
    This->Bro=NULL;
    This->Son=son1;
    This->word="Unary";
    return This;
};

//Factor语法单元的分析程序
//产生式Factor  --> ( Bool ) | Vari | Value
//first(Factor)="("
struct Syntax_Node * Factor(){
    struct Syntax_Node* This=malloc(sizeof(struct Syntax_Node));
    struct Syntax_Node* son1;
    Advance();
    if(!(strcmp(token->word,"(")))
    {
        son1=token;
        struct Syntax_Node* son2=Expr();
        son1->Bro=son2;
        Advance();
        if(strcmp(token->word,")"))
            Error(1,token,This);
        son2=son2->Bro=token;
    }
    else
        Unadvance();
    son1=token;
    This->Bro=NULL;
    This->Son=son1;
    This->word="Factor";
    return This;
};

//错误处理子程序
void Error(int E_code,struct Syntax_Node *e,struct Syntax_Node *This)
{
    printf("in func:%s\t%s\n\tERROR:\tCode:%d %s\n",This->word,e->len,E_code,Error_Messages[E_code]);
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
        i=fprintf(out,"%s",Head->word);
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
