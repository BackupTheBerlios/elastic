/**
 * Name: elastic
 * Description: elastiC programming language.
 * Author: Marco Pantaleoni <panta@elasticworld.org>
 */

state elastic extends HighlightEntry
{
  /* Comments. */
  /\/\*/ {
    comment_face (true);
    language_print ($0);
    call (c_comment);
    comment_face (false);
  }
  /\/\// {
    comment_face (true);
    language_print ($0);
    call (eat_one_line);
    comment_face (false);
  }

  /* String constants. */
  /\"/ {
    string_face (true);
    language_print ($0);
    call (c_string);
    string_face (false);
  }

  /* Character constants. */
  /'.'|'\\\\.'/ {
    string_face (true);
    language_print ($0);
    string_face (false);
  }

  /* Storage specifiers */
  /\b(local|p(ublic|rivate)|static)\b/ {
    type_face (true);
    language_print ($0);
    type_face (false);
  }

  /* @true and @false */
  /@(true|false)/ {
    reference_face (true);
    language_print ($0);
    reference_face (false);
  }

  /* Keywords.
     (build-re '(break catch class continue do else extends
	     for from function goto if import in local
         method package private public return self static super
         throw try while))
  */
  /\b(break|c(atch|lass|ontinue)|do|e(lse|xtends)|f(or|rom|unction)|goto\
|i(f|mport|n)|method\
|package|return\
|s(elf|uper)|t(hrow|ry)\
|while)\b/ {
    keyword_face (true);
    language_print ($0);
    keyword_face (false);
  }
}


/*
Local variables:
mode: c
End:
*/
