/**
 * Name: elastic
 * Description: elastiC programming language.
 * Author: Marco Pantaleoni <panta@elasticworld.org>
 */

elastic_type_re =
/* Types.
   (build-re '(local static private public))
 */
  /\b(local|p(rivate|ublic)|static))\b/;


state elastic_method_line extends CHighlight
{
  /* Argument declarations after the method.
   $1      $2                       $3       $4 $5           $6      $7 */
  /([ \t]*)([A-Za-z_][A-Za-z_0-9]*)?(:[ \t]*)(\(([^)\n]+)\))?([ \t]*)([A-Za-z_][A-Za-z_0-9]*)/ {
    language_print ($1);

    if (length ($2) > 0)
      {
	function_name_face (true);
	language_print ($2);
	function_name_face (false);
      }

    language_print ($3);

    if (length ($4) > 0)
      {
	language_print ("(");
	type_face (true);
	language_print ($5);
	type_face (false);
	language_print (")");
      }

    language_print ($6);

    variable_name_face (true);
    language_print ($7);
    variable_name_face (false);
  }

  /\n/ {
    language_print ($0);
    return;
  }
}

/* This is *not* inherited from CHighlight. */
state elastic_method_continuation_line
{
  /* Method names and arguments on lines following the function declaration.
    $1      $2                       $3       $4 $5           $6      $7 */
  /^([ \t]*)([A-Za-z_][A-Za-z_0-9]*)?(:[ \t]*)(\(([^)\n]+)\))?([ \t]*)\
([A-Za-z_][A-Za-z_0-9]*)/ {
    language_print ($1);

    if (length ($2) > 0)
      {
        function_name_face (true);
        language_print ($2);
        function_name_face (false);
      }

    language_print ($3);

    if (length ($4) > 0)
      {
        language_print ("(");
        type_face (true);
        language_print ($5);
        type_face (false);
        language_print (")");
      }

    language_print ($6);

    variable_name_face (true);
    language_print ($7);
    variable_name_face (false);

    /* Highlight all remaining arguments on this line. */
    call (elastic_method_line);
  }

  /*
   * If the previous one didn't match, we'r done with this method
   * declaration.
   */
  /()/ {
    return;
  }
}

/* This is *not* inherited from CHighlight. */
state elastic_compiler_directive_line
{
  /([ \t:<(,]*)([A-Za-z_][A-Za-z_0-9]*)/ {
    language_print ($1);

    function_name_face (true);
    language_print ($2);
    function_name_face (false);
  }

  /*
   * If the previous one didn't match, we'r done with this directive.
   * Yes, that should match an empty string.
   */
  /()/ {
    return;
  }
}

/*
 * We inherit the Objective-C state from the C state.  This gives us
 * all the defaults, etc.  All we have to do here is to overwrite
 * things that are not implemented, or are broken.
 */
state elastic extends c
{
  BEGIN {
    /* See `c.st' for the comments on this one. */
    type_re = elastic_type_re;
  }

  /* One line comments. */
  /\/\// {
    comment_face (true);
    language_print ($0);
    call (eat_one_line);
    comment_face (false);
  }

  /* Compiler directives. */
  /(@)([A-Za-z][A-Za-z0-9]*)\>/ {
    /* Leading garbage. */
    language_print ($1);

    /* The directive. */
    keyword_face (true);
    language_print ($2);
    keyword_face (false);

    /* And all the remaining stuff on this line. */
    call (elastic_compiler_directive_line);
  }

  /* Keywords.  Basicly what's missing from C *but* not goto or case.
     (build-re '(self super _cmd id Class SEL IMP BOOL YES NO nil Nil))
   */
  /\b(BOOL|Class|IMP|N(O|il)|SEL|YES|_cmd|id|nil|s(elf|uper))\b/ {
    keyword_face (true);
    language_print ($0);
    keyword_face (false);
  }

  /* Types. */
 elastic_type_re {
    type_face (true);
    language_print ($0);
    type_face (false);
  }

  /* Method names.  First, on the same line as the function declaration.
   $1           $2        $3      $4 $5           $6      $7 */
  /(^[+-][ \t]*)(PRIVATE)?([ \t]*)(\(([^)\n]+)\))?([ \t]*)([A-Za-z_]\
[A-Za-z_0-9]*)/ {
    language_print ($1);

    if (length ($2) > 0)
      {
	type_face (true);
	language_print ($2);
	type_face (false);
      }

    language_print ($3);

    if (length ($4) > 0)
      {
	language_print ("(");
	type_face (true);
	language_print ($5);
	type_face (false);
	language_print (")");
      }

    language_print ($6);

    function_name_face (true);
    language_print ($7);
    function_name_face (false);

    /* Highlight arguments from the same line. */
    call (elastic_method_line);

    /*
     * Method names and arguments on lines following the function declaration.
     */
    call (elastic_method_continuation_line);
  }

  /*
   * Labels and case tags.  These must remain as a sole statement on a line,
   * otherwise we detect selectors.  Emacs accepts also bare numbers.
   */
  /^([ \t]*)([a-zA-Z0-9_]+)(:[ \t]*)$/ {
    language_print ($1);

    reference_face (true);
    language_print ($2);
    reference_face (false);

    language_print ($3);
  }
}


/*
Local variables:
mode: c
End:
*/
