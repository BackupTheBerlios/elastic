package cal_driver;

import array;
import basic;
import Token;
import cal;

public class CalParser extends cal.Parser
{
	local token_stack;

	method init()
	{
		[super init];
		//[self set_yydebug @true];

		//token_stack = #[Token.NUMBER, 100, '+', Token.NUMBER, 200, '-', Token.NUMBER];
		token_stack = #[Token.NUMBER, 30, Token.SYMBOL, "abc", '=', Token.NUMBER, 40, Token.SYMBOL, "abc", '+', Token.NUMBER, 12.2];
	}

	method yylex()
	{
		local token;

		if ([self stack_empty])
			token = 0;
		else
			token = array.unshift(token_stack);
		if (token == Token.NUMBER)
			[self set_yylval array.unshift(token_stack)];
		else if (token == Token.SYMBOL)
			[self set_yylval array.unshift(token_stack)];
		return token;
	}

	method stack_empty()
	{
		local al;

		al = array.length(token_stack);
		return (al <= 0);
	}

	method plus(x, y)
	{
		return x + y;
	}
	method minus(x, y)
	{
		return x - y;
	}
}

public myparser = [CalParser new];

while ((! [myparser stack_empty]) &&
	   ([myparser yyparse] == 0))
	basic.print("result: ", cal.result, "\n");
