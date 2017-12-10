#include<stdio.h>
#include<stdlib.h>
#include<string.h>
enum yytokentype {
	INT=258,STR=259,VOID=260,ID=261,IF=262,ELSE=263,WHILE=264,
	RETURN=265,PRINT=266,SCAN=267,STRING=268,ASSIGN=269,CMP=270,
	NUMBER=271,NOTE=272,
	EOL=273

};


extern int yylex();
extern int yylval;
extern char* yytext;
extern FILE* yyin;


int tok;

void advance()
{
	tok = yylex();
	if(tok==NOTE)
	{
		//printf("tok:NOTE\n");
		printf("tok: NOTE");
		tok=yylex();
	}	
	while(tok==EOL)
	{
		printf("tok:EOL\n");
		tok = yylex();
	}
	
	printf("tok: %s ", yytext);
}

void program()
{
	external_declaration();
	while(tok!=EOF)
	{
		external_declaration();
	}
}
external_declaration()
{
	type();
	declarator();
	decl_or_stmt();
}
decl_or_stmt()
{
	//advance();
	if(tok=='{')
	{
		printf("{\n");
		advance();
		if(tok=='}')
		{
			printf("}\n");	
			advance();
		}
		else
		{
			statement_list();
			if(tok=='}')
			{
				printf("}\n");
				advance();
			}

			else
				{printf("decl_stmt!"); error();}
		}
	}
	else if(tok==',')
	{
		printf(" ,\n");
		advance();
		declarator_list();
		if(tok==';')
		{
			printf(";");
			advance();
		}

		else
			{printf("decl_stmt!"); error();}
	}
	else if(tok==';')
	{
		printf(";\n");
		advance();
	}

	else
		{//printf("decl_stmt!");
			printf("finish");			 
			exit(0);}
}
statement_list()
{
	int k=statement();
	while(k!=0)
	{
		//advance();
		k=statement();
	}
}
int statement()
{
	//advance();
	if(tok=='{')
	{
		printf("{\n");
		advance();
		statement_list();
		if(tok=='}')
		{
			printf("}\n");
			advance();
			return 1;
		}
		else 
		{
			printf("error in statement");
			return 0;
		}
	}
	else if(tok==INT||tok==STR||tok==VOID)
	{
		
		if(tok==INT)
			printf("INT\n");
		else if(tok==STR)
			printf("STR\n");
		else if(tok==VOID)
			printf("VOID\n");
		advance();
		declarator_list();
			if(tok==';')
			{
				printf(";\n");
				advance();
				return 1;
			}
			else
			{
				printf("error in statement");
				return 0;
			}

	}
	else if(tok==IF)
	{
		printf("IF\n");
		advance();
		if(tok=='(')
		{
			printf("(\n");
			advance();
			if(expr())
			{
				if(tok==')')
				{
					printf(")\n");
					advance();
					int t=statement();
					if(tok==ELSE)
					{
						printf("ELSE\n");
						advance();
						return statement();
					}
					return t;
				}
				else
				{
					printf("error in statement");
					return 0;
				}
			}
			else
			{
				printf("error in statement");
				return 0;
			}

		}
		else
		{
			printf("error in statement");
			return 0;
		}
	
	}
	else if (tok==WHILE)
	{
		printf("WHILE\n");
		advance();
		if(tok=='(')
		{
			printf("(\n");
			advance();
			if(expr())
			{
				if(tok==')')
				{
					printf(")\n");
					advance();
					return statement();
				}
				else 
				{
					printf("error in statement");
					return 0;
				}
			}
			else
			{
				printf("error in statement");
				return 0;
			}

		}
		else
		{
			printf("error in statement");
			return 0;
		}

	}
	else if(tok==RETURN)
	{
		printf("RETURN\n");
		advance();
		if(tok==';')
		{
			printf(";\n");	
			advance();
			return 1;
		}

		else
		{
			if(expr())
			{
				if(tok==';')
				{
					printf(";\n");
					advance();
					return 1;
				}
				else
				{
					printf("error in statement");
					return 0;
				}
			}
			else
			{
				printf("error in statement");
				return 0;
			}


		}
	}
	else if (tok==PRINT)
	{
		printf("PRINT\n");
		advance();
		if(tok==';')
		{
			printf(";\n");
			advance();
			return 1;
		}
		else
		{
			if(expr_list())
			{
				if(tok==';')
				{
					printf(";\n");
					advance();
					return 1;
				}
				else  	 
				{
					printf("error in statement");
					return 0;
				}
			}
			else  	 
			{
				printf("error in statement");
				return 0;
			}

		}
	}
	else if(tok==SCAN)	
	{
		printf("SCAN\n");
		advance();
		if(id_list())
		{
			if(tok==';')
			{
				printf(";\n");
				advance();
				return 1;
			}
			else  
			{
				printf("error in statement");
				return 0;
			}
		}
		else
		{
			printf("error in statement");
			return 0;
		}
	}
	else 
	{
		return expr_statement();
		
	}
	
}
declarator_list()
{
	declarator();
	while(tok==',')
	{
		printf(" ,\n");
		advance();
		declarator();
	}
}

declarator()
{
	if(tok==ID)
	{
		printf("ID\n");
		advance();
		//printf("%d",tok);
		if(tok==ASSIGN)
		{
			printf("=\n");
			advance();
			if(!expr())
				{printf("decalarator!"); error();}
		}
		else if(tok=='(')
		{
			printf("(\n");
			advance();
			if(tok==')')
			{
				printf(")\n");
				advance();
			}
			else
			{
				parameter_list();
				if(tok==')')
				{
					printf(")\n");
					advance();
				}
				else{printf("decalarator!"); error();}
			}
		}
		else if(tok=='[')
		{
			printf("[\n");
			advance();
			if(tok==']')
			{
				printf("]\n");
				advance();
				if(tok==ASSIGN)
				{
					printf("=\n");
					advance();
					if(tok=='{')
					{
						printf("{\n");
						advance();
						intstr_list();
						if(tok=='}')
						{
							printf("}\n");
							advance();
						}
						else{printf("decalarator!"); error();}
					}
					else{printf("decalarator!"); error();}
				}
			}
			else
			{
				if(expr())
				{
					if(tok==']')
					{
						printf("]\n");
						advance();
						if(tok==ASSIGN)
						{
							printf("=\n");
							advance();
							if(tok=='{')
							{
								printf("{\n");
								advance();
								intstr_list();
								if(tok=='}')
								{
									printf("}\n");
									advance();
								}
								else{printf("decalarator!"); error();}
							}
							else{printf("decalarator!"); error();}
						}
					}
					else{printf("decalarator!"); error();}
				}
				else{printf("decalarator!"); error();}
			}
		}
	}
	//else{printf("decalarator!"); error();}
}
intstr_list()
{
	if(tok==NUMBER)
	{
		printf("NUMBER\n");
		advance();
	}
	else if(tok==STRING)
	{
		printf("STRING\n");
		advance();
	}

	while(tok==',')
	{
			
		printf(" ,\n");
		advance();			
		if(tok==NUMBER)
		{
			printf("NUMBER\n");
			advance();
		}
		else if(tok==STRING)
		{
			printf("STRING\n");
			advance();
		}
	}
}

parameter_list()
{
	parameter();
	while(tok==',')
	{
		printf(" ,\n");
		advance();
		parameter();
	}
}
parameter()
{
	type();
	if(tok==ID)
	{
		printf("ID\n");
		advance();
	}
	else {printf("parameter!"); error();}
}
type()
{
	if(tok==INT)
	{
		printf("INT\n");
		advance();
	}
	else if(tok==STR)
	{
		printf("STR\n");
		advance();
	}
	else if(tok==VOID)
	{
		printf("VOID\n");
		advance();
	}
	//else {printf("type!"); error();}
}
int expr_statement()
{
	if(tok==';')
	{
		printf(";\n");
		advance();
		return 1;
	}
	else
	{
		if(expr())
		{	
			if(tok==';')
			{
				printf(";\n");
				advance();
				return 1;
			}
			else 
			{
				printf("error in expr_statement!");
				return 0;
			}
		}
		else 
		{
		//	printf("error in expr_statement!");
			return 0;
		}

	}
}

int expr_list()
{
	int t=expr();
	while(tok==',')
	{
		printf(" ,\n");
		advance();
		t=expr();
	}
	return t;
}

int expr()
{
	if(cmp_expr())
		return 1;
	else 
	{
	//	printf("error in expr");
		return 0;
	}
}
int cmp_expr()
{
	int t=add_expr();
	while(tok==CMP)
	{
		printf("CMP\n");
		advance();
		t=add_expr();
	}
	return t;
}
int add_expr()
{
	int t=mul_expr();
	while(tok=='+'||tok=='-')
	{
		int ntok=tok;
		if(ntok=='+')
		{
			printf("+\n");
			advance();
			t=mul_expr();
		}
		else if(ntok=='-')
		{
			printf("-\n");
			advance();
			t=mul_expr();
		}
	}
	return t;
}
int mul_expr()
{
	int t;
	if(tok=='-')
	{
		printf("-\n");
		advance();
		t=primary_expr();
	}
	else
	{
		t=primary_expr();
		if(t==1)
		{
			while(tok=='*'||tok=='/'||tok=='%')
			{
				int ntok=tok;
				if(ntok=='*')
				{
					printf("*\n");
					advance();
					t=primary_expr();
				}
				else if (ntok=='/')
				{
					printf("/\n");
					advance();
					t=primary_expr();
				}
				else if (ntok=='%')
				{
					printf("%%\n");
					advance();
					t=primary_expr();
				}
			}
		}
		else
		{
		//	printf("error in mul_expr");
			t=0;
		}

	}
	return t;
}
int id_list()
{
	if(tok==ID)
	{
		printf("ID\n");
		advance();
		while(tok==',')
		{
			printf(" ,\n");
			advance();
			if(tok==ID)
			{
				printf("ID\n");
				advance();
			}
			else 
			{
				printf("error in id_list");
				return 0;
			}
		}
		return 1;
	}
	else
	{
		printf("error in id_list");
		return 0;
	}
}

int primary_expr()
{
	if(tok==ID)
	{
		printf("ID\n");
		advance();
		if(tok=='(')
		{
			printf("(\n");
			advance();
			if(tok==')')
			{
				printf(")\n");
				advance();
				return 1;
			}
			else
			{
				if(expr_list())
				{
					if(tok==')')
					{
						printf(")\n");
						advance();
						return 1;
					}
					else 
					{
						printf("error in primary_expr");
						return 0;
					}

				}
				else
				{
					printf("error in primary_expr");
					return 0;
				}
			}
		}
	
		else if(tok==ASSIGN)
		{
			printf("=\n");
			advance();
			return expr();
		}
		else if(tok=='[')
		{
			printf("[\n");
			advance();
			if(expr())
			{
				if(tok==']')
				{
					printf("]\n");
					advance();
					if(tok==ASSIGN)
					{
						printf("=\n");
						advance();
						return expr();
					}
				}
				else 
				{
					printf("error in primary_expr");
					return 0;
				}
			}
			else
			{
				printf("error in primary_expr");
				return 0;
			}

		}
		return 1;
		
	}
	else if(tok==NUMBER)
	{
		printf("NUMBER\n");
		advance();
		return 1;
	}
	else if(tok==STRING)
	{
		printf("STRING\n");
		advance();
		return 1;
	}
	else if(tok == 'q')
		exit(0);
	else
	{
	//	printf("error in primary_expr");
		return 0;
	}
}
error()
{
	printf("error!");
	exit(0);
}
int main(int argc, char **argv)
{
	FILE* f = fopen(argv[1],"r+");
	setbuf(stdout, NULL);
	yyin = fopen("test.c", "r");
	advance();
	program();
	
	return 0;
}


