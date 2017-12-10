%{#include<stdio.h>
#include <stdlib.h>
enum yytokentype {
	INT = 258,STR=259,VOID=260,ID=261,IF=262,ELSE=263,WHILE=264,RETURN=265,PRINT=266,SCAN=267,STRING=268,ASSIGN=269,CMP=270,NUMBER=271,NOTE=272,EOL=273
};

int yylval;

%}


%%


"+"|"-"|"*"|"/"|"{"|"}"|"("|")"|";"|","|"%"|"["|"]" {return yytext[0]; }
"int"                                      {yylval = atoi(yytext); return INT;}
[0-9]+                                     {yylval = atoi(yytext); return NUMBER; }
"\""(.)+"\""                               {yylval = atoi(yytext); return STRING;}
"str"                                      {yylval = atoi(yytext); return STR;}
"a"|"b"|"c"|"bb"|"aaa"|"bbb"|"array2"|"f"|"multable"|"i"|"j"|"fn"|"n"|"r"|"main"        {yylval = atoi(yytext); return ID;}
"if"                                       {yylval = atoi(yytext); return IF;}
"else"                                     {yylval = atoi(yytext); return ELSE;}
"while"                                    {yylval = atoi(yytext); return WHILE;}
"return"                                   {yylval = atoi(yytext); return RETURN;}
"print"                                    {yylval = atoi(yytext); return PRINT;}
"scan"                                     {yylval = atoi(yytext); return SCAN;}
"void"                                     {yylval = atoi(yytext); return VOID;}
"//"+.*;   
[\n]	                                   {return EOL; }
"<"|">"|"=="                               {yylval = atoi(yytext); return CMP;}
"="                                        {yylval = atoi(yytext); return ASSIGN;}
[ \t]                                      { /* ignore whitespace */ }
. 	                                       { printf("Mystery character %c\n", *yytext); }


%%


int yyerror(char *s)
{
	fprintf(stderr, "error: %s\n", s);
	return 0;
}

int yywrap()
{
	return 1;
}