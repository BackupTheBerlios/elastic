/* ==========================================================================
 * elastic.l
 * ==========================================================================
 * Scanner for elastiC language.
 * --------------------------------------------------------------------------
 *   AUTHOR:  Marco Pantaleoni         E-mail: panta@elasticworld.org
 *
 *   Created: 1997
 *
 *   $Id: elastic_s.l,v 1.6 2005/03/18 10:39:40 panta Exp $
 * --------------------------------------------------------------------------
 *    Copyright (C) 1997-2002 Marco Pantaleoni. All rights reserved.
 *
 *  The contents of this file are subject to the elastiC License version 1.0
 *  (the "elastiC License"); you may not use this file except in compliance
 *  with the elastiC License. You may obtain a copy of the elastiC License at
 *  http://www.elasticworld.org/LICENSE
 *
 *  IN NO EVENT SHALL THE AUTHOR OR DISTRIBUTORS BE LIABLE TO ANY PARTY
 *  FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 *  ARISING OUT OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY
 *  DERIVATIVES THEREOF, EVEN IF THE AUTHOR HAS BEEN ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  THE AUTHOR AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE
 *  IS PROVIDED ON AN "AS IS" BASIS, AND THE AUTHOR AND DISTRIBUTORS HAVE
 *  NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR
 *  MODIFICATIONS.
 *
 *  See the elastiC License for the specific language governing rights and
 *  limitations under the elastiC License.
 * ==========================================================================
 */

%{
#include "basic.h"
#include "ast.h"
#include "elastic_p.h"
#include "private.h"
#include "compat.h"

static void start( void );
static void count( void );

#define BUFLEN 4096
static char buffer[BUFLEN], *buf = 0;
static long strl = 0;

/* Textual position of tokens */
extern YYLTYPE yylloc;

#define YY_INPUT(buf, result, maxsize)	\
do {										\
	ssize_t nread;								\
	ec_stream_exception_clear(PRIVATE(yyin));				\
	nread = ec_stream_read(PRIVATE(yyin), (buf), (ssize_t)(maxsize));	\
	if (EC_ERRORP(PRIVATE(yyin)->exc)) {					\
		result = -1;							\
		YY_FATAL_ERROR( "input in flex scanner failed" );		\
	} else {								\
		result = (nread > 0) ? nread : YY_NULL;				\
	}									\
} while(0)

#define ECHO \
do {										\
	if (ec_stream_stdout()) /* we could add PRIVATE(yyout) */		\
		ec_stream_write( ec_stream_stdout(), yytext, yyleng );		\
} while(0)

static int local_fprintf(void *fh, const char *fmt, ...)
{
	va_list ap;
	EcInt   rv;

	va_start(ap, fmt);
	rv = ec_stderr_vprintf(fmt, ap);
	rv += ec_stderr_printf("\n");
	va_end(ap);
	return rv;
}

#define fprintf local_fprintf
%}

LBRACE  	"{"
RBRACE  	"}"
DIGIT   	[0-9]
HEXDIGIT	[a-fA-F0-9]
ID      	[a-zA-Z_][a-zA-Z0-9_]*
KEYWORD 	[a-zA-Z0-9_]+":"
CSYM    	[a-zA-Z_][a-zA-Z0-9_:-]*
WS		[ \t\n]

%option nounput
%option noyywrap

%x comment
%x lcomment
%x string

%%


0[xX]{HEXDIGIT}+                        { unsigned long i; start(); count(); sscanf(yytext+2, "%lx", &i ); yylval.icnst = i; return T_INUMBER; }
0{DIGIT}+                               { unsigned long i; start(); count(); sscanf(yytext+1, "%lx", &i ); yylval.icnst = i; return T_INUMBER; }
{DIGIT}+				{
						long i;
						start(); count();
						sscanf(yytext, "%ld", &i); yylval.icnst = (EcInt)i;
						return T_INUMBER;
					}

{DIGIT}*"."{DIGIT}+			|
{DIGIT}+"."{DIGIT}*			|
{DIGIT}*"."{DIGIT}+[eE][+-]?{DIGIT}+	|
{DIGIT}+"."{DIGIT}*[eE][+-]?{DIGIT}+	{
						double f;
						start(); count();
						sscanf(yytext, "%lf", &f); yylval.fcnst = (EcFloat)f;
						return T_FNUMBER;
					}

"@true"                        { start(); yylval.objcnst = EcTrueObject;  return T_OBJECT; }
"@false"                       { start(); yylval.objcnst = EcFalseObject; return T_OBJECT; }
"@nil"                         { start(); yylval.objcnst = EC_NIL;        return T_OBJECT; }

"\""                           { start(); count(); strl = 0; buf = buffer; BEGIN(string); }
<string>"\\\""                 { count(); *buf++ = '"'; strl++;       }
<string>"\\\\"                 { count(); *buf++ = '\\'; strl++;      }
<string>"\\n"                  { count(); *buf++ = '\n'; strl++;      }
<string>"\\t"                  { count(); *buf++ = '\t'; strl++;      }
<string>"\\b"                  { count(); *buf++ = '\b'; strl++;      }
<string>"\\r"                  { count(); *buf++ = '\r'; strl++;      }
<string>"\\f"                  { count(); *buf++ = '\f'; strl++;      }
<string>"\\v"                  { count(); *buf++ = '\v'; strl++;      }
<string>"\\a"                  { count(); *buf++ = '\a'; strl++;      }
<string>"\\?"                  { count(); *buf++ = '\?'; strl++;      }
<string>"\\'"                  { count(); *buf++ = '\''; strl++;      }
<string>"\\0"                  { count(); *buf++ = '\0'; strl++;      }
<string>[^\\\"]                { count(); *buf++ = yytext[0]; strl++; }
<string>"\""{WS}*"\""          { count(); }
<string>"\""                   { count(); *buf++ = '\0'; yylval.strcnst = ec_stringdup( buffer ); BEGIN(INITIAL); return T_STRING; }

'\\\\'                         { start(); count(); yylval.ccnst = '\\'; return T_CHAR; }
'\\n'                          { start(); count(); yylval.ccnst = '\n'; return T_CHAR; }
'\\t'                          { start(); count(); yylval.ccnst = '\t'; return T_CHAR; }
'\\b'                          { start(); count(); yylval.ccnst = '\b'; return T_CHAR; }
'\\r'                          { start(); count(); yylval.ccnst = '\r'; return T_CHAR; }
'\\f'                          { start(); count(); yylval.ccnst = '\f'; return T_CHAR; }
'\\v'                          { start(); count(); yylval.ccnst = '\v'; return T_CHAR; }
'\\a'                          { start(); count(); yylval.ccnst = '\a'; return T_CHAR; }
'\\?'                          { start(); count(); yylval.ccnst = '\?'; return T_CHAR; }
'\\\''                         { start(); count(); yylval.ccnst = '\''; return T_CHAR; }
'\\0'                          { start(); count(); yylval.ccnst = '\0'; return T_CHAR; }
'[^\\']'                       { start(); count(); yylval.ccnst = yytext[1]; return T_CHAR; }

"&&"                           { start(); count(); return T_LAND;     }
"||"                           { start(); count(); return T_LOR;      }
"if"                           { start(); count(); return T_IF;       }
"else"                         { start(); count(); return T_ELSE;     }
"while"                        { start(); count(); return T_WHILE;    }
"do"                           { start(); count(); return T_DO;       }
"for"                          { start(); count(); return T_FOR;      }
"in"                           { start(); count(); return T_IN;       }
"break"                        { start(); count(); return T_BREAK;    }
"continue"                     { start(); count(); return T_CONTINUE; }
"goto"                         { start(); count(); return T_GOTO;     }
"try"                          { start(); count(); return T_TRY;      }
"catch"                        { start(); count(); return T_CATCH;    }
"throw"                        { start(); count(); return T_THROW;    }
"return"                       { start(); count(); return T_RETURN;   }
"function"                     { start(); count(); return T_FUNCTION; }
"class"                        { start(); count(); return T_CLASS;    }
"extends"                      { start(); count(); return T_EXTENDS;  }
"method"                       { start(); count(); return T_METHOD;   }
"super"                        { start(); count(); return T_SUPER;    }
"local"                        { start(); count(); return T_LOCAL;    }
"static"                       { start(); count(); return T_STATIC;   }
"private"                      { start(); count(); return T_PRIVATE;  }
"public"                       { start(); count(); return T_PUBLIC;   }

"package"                      { start(); count(); return T_PACKAGE;  }
"import"                       { start(); count(); return T_IMPORT;   }
"from"                         { start(); count(); return T_FROM;     }


{ID}                           { start(); count(); yylval.strcnst = ec_stringdup(yytext); return T_SYMBOL; }
{KEYWORD}                      { start(); count(); yylval.sym = EcInternSymbol( yytext ); return T_KEYWORD; }
"'"{CSYM}                      { start(); count(); yylval.sym = EcInternSymbol( yytext + 1 ); return T_CONSTSYM; }
"#"{CSYM}                      { start(); count(); yylval.sym = EcInternSymbol( yytext + 1 ); return T_CONSTSYM; }

"<="                           { start(); count(); return T_LE;      }
">="                           { start(); count(); return T_GE;      }
"=="                           { start(); count(); return T_EQ;      }
"!="                           { start(); count(); return T_NE;      }
"!"                            { start(); count(); return yytext[0]; }
"+"|"-"|"*"|"/"|"%"|"="	       { start(); count(); return yytext[0]; }
"&"|"^"|"|"|"~"		       { start(); count(); return yytext[0]; }
"**"                           { start(); count(); return T_POW;  }
"<<"                           { start(); count(); return T_LSHIFT;  }
">>"                           { start(); count(); return T_RSHIFT;  }
"?"|":"		               { start(); count(); return yytext[0]; }

"**="                          { start(); count(); return T_POWASSIGN;    }
"*="                           { start(); count(); return T_MULASSIGN;    }
"/="                           { start(); count(); return T_DIVASSIGN;    }
"%="                           { start(); count(); return T_MODASSIGN;    }
"+="                           { start(); count(); return T_ADDASSIGN;    }
"-="                           { start(); count(); return T_SUBASSIGN;    }
"<<="                          { start(); count(); return T_LSHIFTASSIGN; }
">>="                          { start(); count(); return T_RSHIFTASSIGN; }
"&="                           { start(); count(); return T_ANDASSIGN;    }
"^="                           { start(); count(); return T_XORASSIGN;    }
"|="                           { start(); count(); return T_ORASSIGN;     }

"++"			       { start(); count(); return T_INC;     }
"--"			       { start(); count(); return T_DEC;     }

"#["			       { start(); count(); return T_ARRAYCONS; }
"%["			       { start(); count(); return T_HASHCONS; }

"/*"                           { start(); count(); BEGIN(comment);               }
<comment>[^*\n]*               { count(); /* eat anything that's not a '*' */    }
<comment>"*"+[^*/\n]*          { count(); /* eat up '*'s not followed by '/'s */ }
<comment>\n                    { count();                                        }
<comment>"*"+"/"               { count(); BEGIN(INITIAL);                        }

"//"                           { start(); count(); BEGIN(lcomment);      }
<lcomment>[^\n]*               { count(); /* Eat up one line comments */ }
<lcomment>\n                   { count(); BEGIN(INITIAL);                }

"..."                          { start(); count(); return T_ELLIPSIS; }

"."                            { start(); count(); return T_QUALIFIER; }

{WS}+                          { count(); /* eat up whitespace & newlines */ }

.                              { start(); count(); return yytext[0]; }

%%

#include "private.h"

static void start( void )
{
	PRIVATE(line_start)    = PRIVATE(line);
	PRIVATE(col_start)     = PRIVATE(column);
	PRIVATE(charnum_start) = PRIVATE(charnum);
}

static void count( void )
{
    int i;

    for (i = 0; yytext[i] != '\0'; i++)
    {
        if (yytext[i] == '\n')
        {
            PRIVATE(line)++;
            PRIVATE(column) = 0;
            PRIVATE(charnum)++;
/*            PRIVATE(charnum) = 0;*/
        }
        else if (yytext[i] == '\t')
        {
            PRIVATE(column) += 8 - (PRIVATE(column) % 8);
            PRIVATE(charnum)++;
        }
        else
        {
            PRIVATE(column)++;
            PRIVATE(charnum)++;
        }
    }

	yylloc.first_line   = PRIVATE(line_start);
	yylloc.first_column = PRIVATE(col_start);
	yylloc.last_line    = PRIVATE(line);
	yylloc.last_column  = PRIVATE(column);
}
