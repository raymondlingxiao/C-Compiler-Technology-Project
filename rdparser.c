/*
 ============================================================================
 Name        : rdparser.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum yytokentype {
	INT = 258,STR = 259,VOID=260,ID=261,IF=262,ELSE=263,WHILE=264,
	RETURN=265,PRINT=266,SCAN=267,STRING=268,ASSIGN=269,CMP=270,
	NUMBER=271,NOTE=272,EOL=273
};


extern int yylex();
extern int yylval;
extern char* yytext;
extern FILE* yyin;

int tok;



void advance()
{
    tok = yylex();
    while(tok==EOL){
	tok=yylex();
    }
    printf("tok: %s\n", yytext);
}

typedef struct _ast ast;
typedef struct _ast *past;

struct _ast{
	int ivalue;
	char* nodeType;
	past left;
	past right;
};


past newAstNode();
past newNum(int value);
past newExpr(int oper, past left,past right);
past newBoth(past left,past right);
past newVarRef(int tok,int i);
past newBracket(int oper1,past var,int oper2);
past astType();
past astExpr_list();
past astId_list();
past astParameter();
past astIntstr_list();
past astParameter_list();
past astDeclarator();
past astDeclarator_list();
past astExpression_statement();
past astStatement();
past astStatement_list();
past astDecl_or_stmt();
past astExternal_declaration();
past astProgram();
past astPrimary_expr();
past astMul_expr();
past astAdd_expr();
past astCmp_expr();
past ast_expr();
void showAst(past node, int nest);

past newAstNode()
{
	past node = malloc(sizeof(ast));
	if(node == NULL)
	{
		printf("run out of memory.\n");
		exit(0);
	}
	memset(node, 0, sizeof(ast));
	return node;
}

past newNum(int value)
{
	past var = newAstNode();
	var->nodeType = "intValue";
	var->ivalue = value;
	return var;
}
past newExpr(int oper, past left,past right)
{
	past var = newAstNode();
	var->nodeType = "expr";
	var->ivalue = oper;
	var->left = left;
	var->right = right;
	return var;
}

past newBoth(past left,past right)
{
    past var=newAstNode();
    var->left=left;
    var->right=right;
    var->ivalue=-11;
    var->nodeType="none";
    return var;
}

past newVarRef(int tok,int i)
{
    past var = newAstNode();
    if(i==1){
        var->nodeType="expr";
    }
    else if(i==2){
        var->nodeType="id";
    }
    else if(i==3){
        var->nodeType="initializer";
    }
    else if(i==4){
        var->nodeType="type";
    }
    else if(i==5){
        var->nodeType="if";
    }
    else if(i==6){
        var->nodeType="else";
    }
    else if(i==7){
        var->nodeType="while";
    }
    else if(i==8){
        var->nodeType="return";
    }
    else if(i==9){
        var->nodeType="print";
    }
    else if(i==10){
        var->nodeType="scan";
    }
    var->ivalue=tok;
    return var;
}

past newBracket(int oper1,past var,int oper2)
{
    if(var==NULL){
        var=newAstNode();
	var->ivalue=-11;
	var->nodeType="none";
    }
past var1=newAstNode();
past var2=newAstNode();
    var1->nodeType="expr";
    var1->ivalue=oper1;
    var2->nodeType="expr";
    var2->ivalue=oper2;
past parent1=newAstNode();
parent1->ivalue=-11;
parent1->nodeType="none";
parent1->left=var1;
parent1->right=var;
past parent2=newAstNode();
parent2->ivalue=-11;
parent2->nodeType="none";
parent2->left=parent1;
parent2->right=var2;
    return parent2;
}

past astType()
{
    if(tok==INT||tok==STR||tok==VOID){
        past l=newVarRef(tok,4);
        advance();
        return l;
    }
    else if(tok=='q'){
        exit(0);
    }
//printf("ccccc\n");
    return NULL;
}

past astExpr_list()
{
    past l=ast_expr();
    if(l!=NULL){
        if(tok==','){
            past n=astExpr_list();
            if(n!=NULL){
                l=newExpr(',',l,n);
            }
        }
    }
    return l;
}

past astId_list()
{
    if(tok == ID)
	{
		past n = newVarRef(tok,2);
		advance();
		if(tok == ','){
            past r = astId_list();
            if(r!=NULL){
                n = newExpr(',', n, r);
            }
		}
		return n;
	}
	else if(tok == 'q')
		exit(0);
	return NULL;
}

past astParameter()
{
    past l=astType();
//printf("bbbbb\n");
    if(l!=NULL){
        if(tok==ID){
            past n=newVarRef(tok,2);
            l=newBoth(l,n);
            advance();
        }
    }
    return l;
}

past astIntstr_list()
{
    if(tok==NUMBER){
	past n=newNum(yylval);
	advance();
	if(tok == ','){
	    advance();
            past r = astIntstr_list();
            if(r!=NULL){
                n = newExpr(',', n, r);
            }
	}
	return n;
    }
    else if(tok==STRING)
	{
		past n = newVarRef(tok,3);
		advance();
		if(tok == ','){
		advance();
            past r = astIntstr_list();
            if(r!=NULL){
                n = newExpr(',', n, r);
            }
		}
		return n;
	}
	else if(tok == 'q')
		exit(0);
	return NULL;
}

past astParameter_list()
{
    past l=astParameter();
    if(tok==','){
        past n=astParameter_list();
        if(n!=NULL){
            l=newExpr(',',l,n);
        }
    }
    return l;
}

past astDeclarator()
{
    if(tok==ID){
        past n=newVarRef(tok,2);
        advance();
        if(tok==ASSIGN){
            advance();
            past l=ast_expr();
            if(l!=NULL){
                l=newExpr('=',n,l);
                return l;
            }
        }
        else if(tok=='(')
        {
            advance();
            past l=astParameter_list();
            if(tok==')'){
                l=newBracket('(',l,')');
                l=newBoth(n,l);
                advance();
                return l;
            }
        }
        else if(tok=='[')
        {
            advance();
            past l=ast_expr();
            if(tok==']'){
                l=newBracket('[',l,']');
                l=newBoth(n,l);
                advance();
                if(tok==ASSIGN){
                    advance();
                    if(tok=='{'){
                        advance();
                        past n=astIntstr_list();
                        if(n!=NULL){
                            if(tok=='}'){
                                n=newBracket('{',n,'}');
                                l=newExpr('=',l,n);
                                advance();
                            }
                        }
                    }
                }
                return l;
            }
        }
	return n;
    }
    return NULL;
}

past astDeclarator_list()
{
    past l=astDeclarator();
    if(tok==','){
        advance();
        past n=astDeclarator_list();
        l=newExpr(',',l,n);
    }
    return l;
}

past astExpression_statement()
{
    past l=ast_expr();
    if(tok==';'){
        past n=newVarRef(tok,1);
        if(l!=NULL){
            l=newBoth(n,l);
            advance();
            return l;
        }
        else{
            return n;
        }
    }
    return NULL;
}

past astStatement()
{
    past l=astType();
    if(l!=NULL){
        past n=astDeclarator_list();
        if(n!=NULL){
            if(tok==';'){
                past r=newVarRef(';',1);
                l=newBoth(l,n);
                l=newBoth(l,r);
                advance();
                return l;
            }
        }
    }
    l=astExpression_statement();
    if(l!=NULL){
        return l;
    }
    if(tok=='{'){
        advance();
        l=astStatement_list();
        if(l!=NULL){
            if(tok=='}'){
                l=newBracket('{',l,'}');
                advance();
                return l;
            }
        }
    }
    else if(tok==IF){
        past r=newVarRef(tok,5);
        advance();
        if(tok=='('){
            advance();
            l=ast_expr();
            if(l!=NULL){
                if(tok==')'){
                    l=newBracket('(',l,')');
                    l=newBoth(r,l);
                    advance();
                    past n=astStatement();
                    if(n!=NULL){
                        l=newBoth(n,l);
                        if(tok==ELSE){
                            r=newVarRef(tok,6);
                            advance();
                            n=astStatement();
                            if(n!=NULL){
                                n=newBoth(r,n);
                                l=newBoth(l,n);
                            }
                        }
                        return l;
                    }
                }
            }
           }
    }
    else if(tok==WHILE)
    {
        past n=newVarRef(tok,7);
        advance();
        if(tok=='(')
        {
            advance();
            past l=ast_expr();
            if(l!=NULL)
            {
                if(tok==')')
                {
                    l=newBracket('(',l,')');
                    advance();
                    past r=astStatement();
                    if(r!=NULL){
                        l=newBoth(n,l);
                        l=newBoth(l,r);
                        return l;
                    }
                }
            }
        }
    }
    else if(tok==RETURN)
    {
        past n=newVarRef(tok,8);
        advance();
        past l=ast_expr();
        if(l!=NULL){
            n=newBoth(n,l);
        }
        if(tok==';')
        {
            past r=newVarRef(tok,1);
            advance();
            n=newBoth(n,r);
            return n;
        }
    }
    else if(tok==PRINT)
    {
        past n=newVarRef(tok,3);
        advance();
        past l=astExpr_list();
        if(l!=NULL){
            n=newBoth(n,l);
        }
        if(tok==';')
        {
            past r=newVarRef(tok,1);
            advance();
            n=newBoth(n,r);
            return n;
        }
    }
    else if(tok==SCAN)
    {
        past n=newVarRef(tok,10);
        advance();
        past l=astId_list();
        if(l!=NULL)
        {
            if(tok==';')
            {
                past r=newVarRef(tok,1);
                advance();
                l=newBoth(n,l);
                l=newBoth(l,r);
                return l;
            }
        }
    }
    return NULL;
}


past astStatement_list()
{
    past l=astStatement();
    if(l!=NULL){
        past n=astStatement();
        while(n!=NULL){
            l=newBoth(l,n);
            n=astStatement_list();
        }
    }
    return l;
}

past astDecl_or_stmt()
{
    if(tok=='{')
    {
        advance();
        past l=astStatement_list();
        if(tok=='}')
        {
            advance();
            l=newBracket('{',l,'}');
            return l;
        }
    }
    else if(tok==',')
    {
        past n=newVarRef(tok,1);
        advance();
        past l=astDeclarator_list();
        if(l!=NULL)
        {
            if(tok==';')
            {
                past r=newVarRef(tok,1);
                advance();
                l=newBoth(n,l);
                l=newBoth(l,r);
                return l;
            }
        }
    }
    else if(tok==';')
    {
        past l=newVarRef(tok,1);
        advance();
        return l;
    }
    return NULL;
}

past astExternal_declaration()
{
    past l=astType();
    if(l!=NULL){
        past n=astDeclarator();
        if(n!=NULL){
            n=newBoth(l,n);
            past r=astDecl_or_stmt();
            if(r!=NULL){
                l=newBoth(n,r);
            }
        }
    }
    return l;
}

past astProgram()
{
    past l=astExternal_declaration();
    if(l!=NULL){
        past n=astExternal_declaration();
        while(n!=NULL){
            l=newBoth(l,n);
            n=astProgram();
        }
    }
    return l;
}

past astPrimary_expr()
{
    if(tok==ID){
        past n=newVarRef(tok,2);
        advance();
        if(tok=='(')
        {
            advance();
            past l=astExpr_list();
            if(tok==')')
            {
                advance();
                l=newBracket('(',l,')');
                l=newBoth(n,l);
                return l;
            }
        }
        else if(tok=='[')
        {
            advance();
            past l=ast_expr();
            if(l!=NULL)
            {
                if(tok==']')
                {
                    advance();
                    l=newBracket('[',l,']');
                    l=newBoth(n,l);
                    if(tok==ASSIGN)
                    {
                        past r=ast_expr();
                        if(r!=NULL)
                        {
                            l=newExpr('=',l,r);
                        }
                    }
                    return l;
                }
            }
        }
        else if(tok==ASSIGN)
        {
            advance();
            past l=ast_expr();
            if(l!=NULL)
            {
                l=newExpr(ASSIGN,n,l);
                return l;
            }
        }
        return n;
    }
    else if(tok=='(')
    {
        advance();
        past l=ast_expr();
        if(l!=NULL)
        {
            if(tok==')')
            {
                advance();
                l=newBracket('(',l,')');
                return l;
            }
        }
    }
    else if(tok==NUMBER)
    {
        past n=newNum(yylval);
        advance();
        return n;
    }
    else if(tok==STRING)
    {
        past n=newVarRef(tok,3);
        advance();
        return n;
    }
    return NULL;
}

past astMul_expr()
{
    if(tok=='-'){
        advance();
        past l=astPrimary_expr();
        if(l!=NULL){
            l=newExpr('-',NULL,l);
            return l;
        }
    }
    else{
        past l=astPrimary_expr();
        if(l!=NULL){
            while(tok == '*' || tok == '/'||tok == '%')
            {
                int oper = tok;
		advance();
                past r = astPrimary_expr();
                if(r!=NULL){
                    l = newExpr(oper, l, r);
                }
            }
        }
        return l;
    }
}

past astAdd_expr()
{
	past l = astMul_expr();
	if(l!=NULL){
        while(tok == '+' || tok == '-')
        {
            int oper = tok;
	    advance();
            past r = astMul_expr();
            if(r!=NULL){
                l = newExpr(oper, l, r);
            }
        }
	}
	return l;
}

past astCmp_expr()
{
	past l = astAdd_expr();
	if(l!=NULL){
        while(tok == CMP)
        {
	    advance();
            past r = astAdd_expr();
            if(r!=NULL){
                l = newExpr(CMP, l, r);
            }
        }
	}
	return l;
}

past ast_expr()
{
    past l = astCmp_expr();
    return l;
}

void showAst(past node, int nest)
{
	if(node == NULL)
		return;

    if(node->ivalue!=-11){
        int i = 0;
        for(i = 0; i < nest; i ++)
            printf("  ");

        if(strcmp(node->nodeType, "expr") == 0){
            printf("%s '%c'\n", node->nodeType, (char)node->ivalue);
}
else if(strcmp(node->nodeType,"intValue")==0){
    printf("%s %d\n",node->nodeType,node->ivalue);
}
else {
	printf("%s ", node->nodeType);
	if(node->ivalue==258){printf("INT\n");}
	else if(node->ivalue==259){printf("STR\n");}
	else if(node->ivalue==260){printf("VOID\n");}
	else if(node->ivalue==261){printf("ID\n");}
	else if(node->ivalue==262){printf("IF\n");}
	else if(node->ivalue==263){printf("ELSE\n");}
	else if(node->ivalue==264){printf("WHILE\n");}
	else if(node->ivalue==265){printf("RETURN\n");}
	else if(node->ivalue==266){printf("PRINT\n");}
	else if(node->ivalue==267){printf("SCAN\n");}
	else if(node->ivalue==268){printf("STRING\n");}
	else if(node->ivalue==268){printf("ASSIGN\n");}
	else if(node->ivalue==270){printf("CMP\n");}
	else if(node->ivalue==272){printf("NOTE\n");}
	else if(node->ivalue==273){printf("EOL\n");}
}
    
    }
    showAst(node->left, nest+1);
    showAst(node->right, nest+1);

}

int main(int argc, char **argv)
{
//	if(argc != 2 )
//	{
//		printf("input file is needed.\n");
//		return 0;
//	}
//	FILE* f = fopen(argv[1]);
	setbuf(stdout, NULL);
	yyin = fopen("test.c", "r");
	advance();
	past rr = astProgram();
	showAst(rr, 0);

	return 0;
}




