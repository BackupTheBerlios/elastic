% This file defines syntax highlighting for elastiC.
% AUTHOR: Marco Pantaleoni <panta@elasticworld.org>

% Mode customizable variables

variable EC_CONTINUED_OFFSET = 4;
variable EC_Colon_Offset = 0;
variable EC_Comment_Column = 50;
variable EC_INDENT = 4;
variable EC_BRACE = 0;
variable EC_BRA_NEWLINE = 1;


% elastiC-mode indentation routines

autoload ("ec_make_comment",     "./ecmisc");
autoload ("ec_format_paragraph", "./ecmisc");
autoload ("ec_paragraph_sep",    "./ecmisc");
autoload ("ec_comment_region",   "./ecmisc");
autoload ("ec_top_of_function",  "./ecmisc");
autoload ("ec_end_of_function",  "./ecmisc");
autoload ("ec_mark_function",    "./ecmisc");
%autoload ("ec_indent_buffer", "krconv");

define ec_bskip_over_comment (skip_pp)
{
   variable num_up;
   
   forever 
     {
	bskip_chars (" \t\n");
	if (bobp ())
	  return;

	if (skip_pp)
	  {
	     push_mark ();
	     %num_up = 0;
	     while (up_1 ())
	       {
		  %eol (); -- up(1) leaves point at eol
		  !if (blooking_at ("\\")) 
		    {
		       go_down_1 ();
		       break;
		    }
		  %num_up++;
	       }

	     bol_skip_white ();
%	     if (looking_at_char ('#'))
%	       {
%		  pop_mark_0 ();
%		  continue;
%	       }
	     pop_mark_1 ();
	  }
	
	!if (blooking_at ("*/"))
	  {
	     push_mark ();
	     if (bfind ("//")) 
	       {
		  if (0 == parse_to_point ())
		    {
		       % Not in a comment or string
		       pop_mark_0 ();
		       continue;
		    }
	       }
	     bol ();
	     !if (bobp ())
	       {
%		  if (skip_pp and looking_at_char ('#'))
%		    {
%		       pop_mark_0 ();
%		       continue;
%		    }
	       }
	     pop_mark_1 ();
	     break;
	  }
	!if (bsearch ("/*")) break;
     }
}

static define ec_looking_at (token)
{
   variable cse = CASE_SEARCH, ret = 0;
   CASE_SEARCH = 1;
   
   if (looking_at(token))
     {
	push_spot ();
	go_right(strlen(token));
	POINT;
	skip_chars ("\t :({");
	ret = (POINT - ()) or eolp();
	pop_spot ();
     }
   CASE_SEARCH = cse;
   ret;
}

static define ec_indent_to (n)
{
   bol ();
   % Force a reindent if the line does not contain tabs followed by spaces.
   skip_chars ("\t");
   skip_chars (" ");

   if ((what_column != n)
       or (POINT != (skip_white (), POINT)))
     {
	bol_trim ();
	n--;
	whitespace (n);
     }
}

static define ec_indent_continued_comment (col)
{
   push_spot ();
   col++;			       %  add 1 so the we indent under * in /*
   ec_indent_to (col);
   
   if (looking_at ("*")
       or not (eolp ()))
     pop_spot ();
   else
     {
	insert ("* ");
	pop_spot ();
	if (what_column () <= col)
	  {
	     goto_column (col + 2);
	  }
     }
}

static define ec_mode_if_bol_skip_white ()
{
   push_mark ();
   bskip_white ();
   1;
   if (bolp ()) 
     {
	pop ();
	skip_white ();
	0;
     }
   pop_mark (());		       %  take argument from stack
}


static define continued_statement_bol ()
{
   while (up_1 ())
     {
	!if (blooking_at ("\\"))
	  {
	     go_down_1 ();
	     break;
	  }
     }
   
   bol ();
}

define ec_indent_line ()
{
   variable val, col, extra_indent = 0;
   variable prep_indent = 0, prep_line = 0;
   variable match_char, match_indent, this_char;
   variable match_mark;
   variable skip_pp = 1;
   variable is_continuation = 0;

   push_spot ();
   bol_skip_white ();
   this_char = what_char ();

   if (-2 == parse_to_point ())
     {
	% In a comment.  Indent it at level of matching /* string
	() = bsearch ("/*");
	col = what_column ();
	pop_spot ();
	ec_indent_continued_comment (col);
	ec_mode_if_bol_skip_white ();
	return;
     }
   
   EXIT_BLOCK
     {
	bol_trim ();
	pop_spot ();
     }
   
%   if (looking_at_char ('#')) 
%     {
%	ec_indent_preprocess_line ();
%	return;
%     }
   
   push_spot ();
   if (up_1 ())
     {
	eol ();
	if (blooking_at ("\\") and not (blooking_at ("\\\\")))
	  {
	     continued_statement_bol ();
%	     !if (looking_at_char ('#'))
%	       {
%		  pop_spot ();
%		  bol_trim ();
%		  pop_spot ();
%		  return;
%	       }
	     !if (0)
	       {
		  pop_spot ();
		  bol_trim ();
		  pop_spot ();
		  return;
	       }
	     %extra_indent += EC_INDENT;
	     prep_indent = EC_INDENT;
	     skip_pp = 0;
	     prep_line = what_line ();
	  }
     }
   pop_spot ();
   
   EXIT_BLOCK
     {
	ec_mode_if_bol_skip_white ();
     }
   
   
   if (orelse
	{ ec_looking_at("case") }
	{ ec_looking_at("default") }
       )
     {
	if (ffind_char (':'))
	  {
	     extra_indent -= EC_INDENT;
	     extra_indent += EC_Colon_Offset;
	  }
	bol ();
     }
   else 
     {
	forever
	  {
	     ec_bskip_over_comment (skip_pp);
	     if (eolp() and (blooking_at ("\\")))
	       {
		  go_left_1 ();
		  continue;
	       }

	     !if (orelse
		   { blooking_at (";") }
		   { blooking_at ("{") }
		   { blooking_at ("}") }
		   { blooking_at ("),") }
		   { blooking_at ("},") }
		   { blooking_at (":") }
		   { bobp () }
		  )
	       {
		  if (skip_pp or is_continuation)
		    extra_indent += EC_CONTINUED_OFFSET;
		  else
		    {
		       push_spot ();
		       bol_skip_white ();
%		       !if (looking_at ("#"))
%			 extra_indent += EC_CONTINUED_OFFSET;
		       !if (0)
			 extra_indent += EC_CONTINUED_OFFSET;
		       pop_spot ();
		    }

		  is_continuation++;
	       }
	     !if (blooking_at (")")) break;
	     push_mark ();
	     go_left_1 ();
	     if (1 != find_matching_delimiter (')'))
	       {
		  pop_mark_1 ();
		  break;
	       }
	     ec_bskip_over_comment (skip_pp);
	     push_spot ();
	     if ((1 == find_matching_delimiter (')')), pop_spot ())
	       {
		  pop_mark_1 ();
		  break;
	       }
	     
	     pop_mark_0 ();
	     bol ();
	  }
     }

   val = find_matching_delimiter (')');
   col = what_column ();
   if ((val < 0) and looking_at ("/*")) val = -2;
   
   match_mark = create_user_mark ();
     
   match_char = what_char();
   bol_skip_white ();
   match_indent = what_column ();
   if (what_line () < prep_line)
     {
	match_char = 0;
     }

   pop_spot ();

   variable notCcomment;

#iftrue
   % Added 04/06/98 MDJ to facilitate C++ style comments
   if (val == 0)
     {
	push_spot();
	bol_skip_white();
	if (eolp())
	  {
	     go_up_1();
	     bol_skip_white();
	     notCcomment = "// ";
	     
	     if (looking_at(notCcomment))
	       {
		  val = -3;
		  col = what_column();
	       }
	  }
	pop_spot();
     }
#endif

   switch (val)
     {
      case 0:			       %  mismatch

	if (match_char == '{')
	  {
	     push_spot ();
	     goto_user_mark (match_mark);
	     bskip_white ();

	     if (blooking_at (")"))
	       {
		  go_left_1 ();
		  if (1 == find_matching_delimiter (')'))
		    {
		       bol_skip_white ();
		       match_indent = what_column ();
		    }
	       }
	     pop_spot ();
	     col = match_indent;
	     col += EC_INDENT;
	     prep_indent = 0;
	  }
	else if (match_char == '[')
	  {
	     ec_indent_to (col + 1);
	     return;
	  }
	else
	  {
	     push_spot ();
	     bol_skip_white ();
	     if (skip_pp and looking_at_char ('{'))
	       extra_indent = 0;
	     extra_indent++;
	     ec_indent_to (extra_indent + prep_indent);
	     pop_spot ();
	     return;
	  }
     }
     {
      case 1:
	extra_indent = 1;	       %  match found
     }
     {
      case -2:			       %  inside comment
	if (this_char != '\\') col++;
	ec_indent_continued_comment (col);
	return;
     }
     {
      case 2:
	push_spot_bol ();
	trim ();
	pop_spot ();
	return;
     }
     {
#iftrue
      case -3: 				% Inside C++ comment
	!if ((looking_at(notCcomment)) or not(eolp()))
	  {
	     goto_column(col);
	     insert(notCcomment);
	  }
	return;
     }
#endif
   
   
   switch (this_char)
     {
      case '}':
	col -= EC_INDENT;
	!if (skip_pp) col += extra_indent;
     }
     {
      case '{':
	col += EC_BRACE;
	if (is_continuation)
	  col -= EC_CONTINUED_OFFSET;
	col += extra_indent;
	%!if (skip_pp) col += extra_indent;
     }
     {
	col += extra_indent;
     }
   
   push_spot ();
   ec_indent_to (col + prep_indent);
   pop_spot ();
}

% This function returns zero if the line does not begin with "* @ "
% or returns the indent column if it does.  This combination of characters
% will most certainly appear in a comment.  See the c file jed/src/intrin.c
% for examples of how it is exploited.
static define ec_is_comment_example ()
{
   push_spot ();
   bol_skip_white ();
   0;
   if (looking_at ("* @ "))
     {
	pop ();
	what_column ();
     }
   pop_spot ();
}


define ec_newline_and_indent ()
{
   variable notCcomment = "//";

#iftrue
   push_spot_bol();
   if (looking_at(notCcomment), pop_spot())
     {
	newline();
	insert(notCcomment);
	return;
     }
#endif

   variable col = ec_is_comment_example ();
   newline ();
   if (col)
     {
	ec_indent_to (col);
	insert ("* @ ");
     }
   else ec_indent_line ();
}

static define ec_parse_to_point ()
{
   parse_to_point () or ec_is_comment_example ();
}

define ec_insert_bra ()
{
   if (ec_parse_to_point ())
     insert_char ('{');
   else
     {
	push_spot ();
	ec_bskip_over_comment (0);
	if (blooking_at (","), pop_spot ())
	  {
	     insert_char ('{');
	  }
	else
	  { 
	     push_spot ();
	     skip_white ();
	     if (eolp ())
	       {
		  bskip_white ();
		  if (not (bolp ()) and EC_BRA_NEWLINE, pop_spot ()) newline ();
		  push_spot ();
		  bskip_white ();
		  bolp ();	       %  on stack
		  pop_spot ();
		  insert_char ('{');
		  if ( () ) ec_indent_line ();   %  off stack
		  eol ();
		  if (EC_BRA_NEWLINE) ec_newline_and_indent ();
	       }
	     else 
	       {
		  pop_spot ();
		  insert_char ('{');
	       }
	  }
     }
}



define ec_insert_ket ()
{
   variable status = ec_parse_to_point ();

   push_spot ();
   skip_white ();
   push_spot ();
   if (status 
       or not (eolp ())
       or (bol_skip_white (), looking_at_char ('{')), pop_spot ())
     {
	pop_spot ();
	insert_char ('}');
	blink_match ();
	return;
     }

   bskip_white ();
   if (bolp (), pop_spot ())
     {
	insert_char ('}');
	trim ();
     }
   else 
     {	
	eol ();
	insert ("\n}");
     }
   ec_indent_line ();
   eol (); 
   blink_match ();
   if (EC_BRA_NEWLINE) ec_newline_and_indent ();
}

define ec_insert_colon ()
{
   insert_char (':');
   !if (ec_parse_to_point ())
     ec_indent_line ();
}

% Keymap

$1 = "elastic";

!if (keymap_p ($1)) make_keymap ($1);
%definekey ("indent_line", "\t", $1);
%definekey ("ec_newline_and_indent", "\r", $1);
definekey ("ec_insert_bra", "{", $1);
definekey ("ec_insert_ket", "}", $1);
definekey ("ec_insert_colon", ":", $1);
definekey ("ec_make_comment", "\e;", $1);
definekey ("ec_comment_region", "^X;", $1);
definekey ("ec_format_paragraph", "\eq", $1);
definekey ("ec_top_of_function", "\e^A", $1);
definekey ("ec_end_of_function", "\e^E", $1);
definekey ("ec_mark_function", "\e^H", $1);

% Create and initialize the syntax tables.

$1 = "elastic";

create_syntax_table ($1);
define_syntax ("//", "",        '%', $1);	% comments
define_syntax ("/*", "*/",      '%', $1);	% comments
define_syntax ("([{", ")]}",    '(', $1);	% delimiters
define_syntax ('"', '"',             $1);	% quoted strings
define_syntax ('\'', '\'',           $1);	% quoted characters
define_syntax ('\\', '\\',           $1);	% continuations
define_syntax ("0-9a-zA-Z_",    'w', $1);	% words
define_syntax ("-+0-9a-fA-FxX", '0', $1);   	% numbers
define_syntax (",;.:",          ',', $1);	% punctuation
define_syntax ("%-+/&*=<>|!~^", '+', $1);	% operators
set_syntax_flags ($1, 1);			% keywords ARE case-sensitive
% set_syntax_flags ($1, 4);

#ifdef HAS_DFA_SYNTAX
enable_highlight_cache("elastic.dfa", $1);
define_highlight_rule("#.*$", "comment", $1);
define_highlight_rule("([\\$%&@\\*]|\\$#)[A-Za-z_0-9]+", "normal", $1);
define_highlight_rule(strcat("\\$([_\\./,\"\\\\#\\*\\?\\]\\[;!@:\\$<>\\(\\)",
			     "%=\\-~\\^\\|&`'\\+]|\\^[A-Z])"), "normal", $1);
define_highlight_rule("[A-Za-z_][A-Za-z_0-9]*", "Knormal", $1);
define_highlight_rule("[0-9]+(\\.[0-9]+)?([Ee][\\+\\-]?[0-9]*)?", "number",
		      $1);
define_highlight_rule("0[xX][0-9A-Fa-f]*", "number", $1);
define_highlight_rule("[\\(\\[\\{\\<\\>\\}\\]\\),;\\.\\?:]", "delimiter", $1);
define_highlight_rule("[%\\-\\+/&\\*=<>\\|!~\\^]", "operator", $1);
define_highlight_rule("-[A-Za-z]", "keyword0", $1);
define_highlight_rule("'[^']*'", "string", $1);
define_highlight_rule("'[^']*$", "string", $1);
define_highlight_rule("\"([^\"\\\\]|\\\\.)*\"", "string", $1);
define_highlight_rule("\"([^\"\\\\]|\\\\.)*\\\\?$", "string", $1);
define_highlight_rule("m?/([^/\\\\]|\\\\.)*/[gio]*", "string", $1);
define_highlight_rule("m/([^/\\\\]|\\\\.)*\\\\?$", "string", $1);
define_highlight_rule("s/([^/\\\\]|\\\\.)*(/([^/\\\\]|\\\\.)*)?/[geio]*",
		      "string", $1);
define_highlight_rule("s/([^/\\\\]|\\\\.)*(/([^/\\\\]|\\\\.)*)?\\\\?$",
		      "string", $1);
define_highlight_rule("(tr|y)/([^/\\\\]|\\\\.)*(/([^/\\\\]|\\\\.)*)?/[cds]*",
		      "string", $1);
define_highlight_rule("(tr|y)/([^/\\\\]|\\\\.)*(/([^/\\\\]|\\\\.)*)?\\\\?$",
		      "string", $1);
define_highlight_rule(".", "normal", $1);
build_highlight_table ($1);
#endif

() = define_keywords ($1, "doifin", 2); % all keywords of length 2
() = define_keywords ($1, "fortry", 3); % all keywords of length 3
() = define_keywords ($1, "elsefromgoto", 4); % all keywords of length 4
() = define_keywords ($1, "breakcatchclasslocalsuperthrowwhile", 5); % all keywords of length 5
() = define_keywords ($1, "importmethodpublicreturnstatic", 6); % all keywords of length 6
() = define_keywords ($1, "extendspackageprivate", 7); % all keywords of length 7
() = define_keywords ($1, "continuefunction", 8); % all keywords of length 8


define ec_init_menu (menu)
{
   menu_append_item (menu, "&Comment Region",  "ec_comment_region");
   menu_append_item (menu, "&Top of Function", "ec_top_of_function");
   menu_append_item (menu, "&End of Function", "ec_end_of_function");
   menu_append_item (menu, "&Mark Function",   "ec_mark_function");
   menu_append_item (menu, "&Format Buffer",   "ec_indent_buffer");
}

%!%+
%\function{elastic_mode}
%\synopsis{elatic_mode}
%\usage{Void elasticmode ();}
%\description
% This is a mode that is dedicated to facilitate the editing of elastiC language files.  
% Functions that affect this mode include:
%#v+
%  function:             default binding:
%  ec_insert_bra              {
%  ec_insert_ket              }
%  newline_and_indent         RETURN
%  indent_line                TAB
%  goto_match                 Ctrl-\ 
%  ec_make_comment            ESC ;
%  ec_format_paragraph        ESC q
%  ec_top_of_function         ESC Ctrl-A
%  ec_end_of_function         ESC Ctrl-E
%  ec_mark_function           ESC Ctrl-H
%#v-
% Variables affecting indentation include:
%#v+
%  EC_INDENT
%  EC_BRACE
%  EC_BRA_NEWLINE
%  EC_CONTINUED_OFFSET
%  EC_Comment_Column (used by ec_make_comment)
%#v-
% Hooks: \var{elastic_mode_hook}
%!%-
define elastic_mode ()
{
   variable elastic = "elastic";

   TAB = 4;
   C_INDENT = 4;
   C_CONTINUED_OFFSET = 4;
   C_Colon_Offset = 0;
   C_BRA_NEWLINE = 1;
   set_mode(elastic, 2); % flag value of 2 is C language mode
   use_keymap(elastic);
   use_syntax_table(elastic);

   set_buffer_hook ("par_sep",             "ec_paragraph_sep");
   set_buffer_hook ("indent_hook",         "ec_indent_line");
   set_buffer_hook ("newline_indent_hook", "ec_newline_and_indent");

   mode_set_mode_info (elastic, "init_mode_menu", &ec_init_menu);

   runhooks("elastic_mode_hook");
}
