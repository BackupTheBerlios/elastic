/* File : example.i */
%module example
%{
/* Put headers and other declarations here */
%}

extern double My_variable;

extern int    fact(int);
extern int    mod(int n, int m);
extern char  *get_time(void);

%inline %{
/* Some constants and variables */
#define PI       3.1415926
#define FOO      42
#define NAME     "Dave"
const void *Null = 0;

int    intvar;
char  *charvar = 0;

%}






