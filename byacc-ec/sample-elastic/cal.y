%{
private SYMBOL_TABLE = %[];

public result = @nil;
%}


%token NUMBER SYMBOL

%%
program
: stm.list { result = $1; }
;

stm.list
: stm              { $$ = $1; basic.print("statement value: ", $1, "\n"); }
| stm.list stm     { $$ = $2; basic.print("statement value: ", $2, "\n"); }
;

stm
: expr             { $$ = $1; }
| SYMBOL '=' expr  { SYMBOL_TABLE[$1] = $3; }
;

expr
: term '+' term    { $$ = [self plus $1, $3];  }
| term '-' term    { $$ = [self minus $1, $3]; }
| term             { $$ = $1;                  }
;

term
: NUMBER           { $$ = yylval; }
| SYMBOL {
           if (SYMBOL_TABLE[$1])
              $$ = SYMBOL_TABLE[$1];
           else {
              basic.print("can't find symbol\n");
              /* raise(RuntimeError,"can't find symbol"); */
           }
         }
;
%%
