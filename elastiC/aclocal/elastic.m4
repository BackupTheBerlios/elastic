##  Based on an original work by Owen Taylor for glib.

dnl AM_PATH_ELASTIC([MINIMUM-VERSION, [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]]])
dnl Test for elastiC, and define ELASTIC_CFLAGS and ELASTIC_LIBS
dnl
AC_DEFUN(AM_PATH_ELASTIC,
[dnl 
dnl Get the cflags and libraries from the elastic-config script
dnl
AC_ARG_WITH(elastic-prefix,[  --with-elastic-prefix=PFX   Prefix where elastiC is installed (optional)],
            elastic_config_prefix="$withval", elastic_config_prefix="")
AC_ARG_WITH(elastic-exec-prefix,[  --with-elastic-exec-prefix=PFX Exec prefix where elastiC is installed (optional)],
            elastic_config_exec_prefix="$withval", elastic_config_exec_prefix="")
AC_ARG_ENABLE(elastictest, [  --disable-elastictest       Do not try to compile and run a test elastiC program],
		    , enable_elastictest=yes)

  if test x$elastic_config_exec_prefix != x ; then
     elastic_config_args="$elastic_config_args --exec-prefix=$elastic_config_exec_prefix"
     if test x${ELASTIC_CONFIG+set} != xset ; then
        ELASTIC_CONFIG=$elastic_config_exec_prefix/bin/elastic-config
     fi
  fi
  if test x$elastic_config_prefix != x ; then
     elastic_config_args="$elastic_config_args --prefix=$elastic_config_prefix"
     if test x${ELASTIC_CONFIG+set} != xset ; then
        ELASTIC_CONFIG=$elastic_config_prefix/bin/elastic-config
     fi
  fi

  AC_PATH_PROG(ELASTIC_CONFIG, elastic-config, no)
  min_elastic_version=ifelse([$1], ,0.0.37,$1)
  AC_MSG_CHECKING(for elastiC - version >= $min_elastic_version)
  no_elastic=""
  if test "$ELASTIC_CONFIG" = "no" ; then
    no_elastic=yes
  else
    ELASTIC_CFLAGS=`$ELASTIC_CONFIG $elastic_config_args --cflags`
    ELASTIC_LIBS=`$ELASTIC_CONFIG $elastic_config_args --libs`
    ELASTIC_CCSHARED=`$ELASTIC_CONFIG $elastic_config_args --ccshared`
    ELASTIC_LDSHARED=`$ELASTIC_CONFIG $elastic_config_args --ldshared`
    ELASTIC_LINKFORSHARED=`$ELASTIC_CONFIG $elastic_config_args --linkforshared`
    ELASTIC_SO=`$ELASTIC_CONFIG $elastic_config_args --so`
    ELASTIC_MKMODULE=`$ELASTIC_CONFIG $elastic_config_args --makemodule`
    ELASTIC_MKMODULEFLAGS=`$ELASTIC_CONFIG $elastic_config_args --makemoduleflags`
    ELASTIC_MKMODULELIBS=`$ELASTIC_CONFIG $elastic_config_args --makemodulelibs`
    elastic_config_major_version=`$ELASTIC_CONFIG $elastic_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    elastic_config_minor_version=`$ELASTIC_CONFIG $elastic_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    elastic_config_micro_version=`$ELASTIC_CONFIG $elastic_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    if test "x$enable_elastictest" = "xyes" ; then
      ac_save_CFLAGS="$CFLAGS"
      ac_save_LIBS="$LIBS"
      CFLAGS="$CFLAGS $ELASTIC_CFLAGS"
      LIBS="$ELASTIC_LIBS $LIBS"
dnl
dnl Now check if the installed elastiC is sufficiently new. (Also sanity
dnl checks the results of elastic-config to some extent
dnl
      rm -f conf.elastictest
      AC_TRY_RUN([
#include <elastic/elastic.h>
#include <elastic/cnf.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
  int major, minor, micro;
  EcInt ec_ver;
  char *tmp_version;

  system ("touch conf.elastictest");

  ec_ver = EcVersionNumber();

  /* HP/UX 9 (%@#!) writes to sscanf strings */
  tmp_version = ec_stringdup("$min_elastic_version");
  if (sscanf(tmp_version, "%d.%d.%d", &major, &minor, &micro) != 3) {
     printf("%s, bad version string\n", "$min_elastic_version");
     exit(1);
   }

  if ((EC_MAJOR(ec_ver) != $elastic_config_major_version) ||
      (EC_MINOR(ec_ver) != $elastic_config_minor_version) ||
      (EC_MICRO(ec_ver) != $elastic_config_micro_version))
    {
      printf("\n*** 'elastic-config --version' returned %d.%d.%d, but elastiC (%d.%d.%d)\n", 
             $elastic_config_major_version, $elastic_config_minor_version, $elastic_config_micro_version,
             EC_MAJOR(ec_ver), EC_MINOR(ec_ver), EC_MICRO(ec_ver));
      printf ("*** was found! If elastic-config was correct, then it is best\n");
      printf ("*** to remove the old version of elastiC. You may also be able to fix the error\n");
      printf("*** by modifying your LD_LIBRARY_PATH enviroment variable, or by editing\n");
      printf("*** /etc/ld.so.conf. Make sure you have run ldconfig if that is\n");
      printf("*** required on your system.\n");
      printf("*** If elastic-config was wrong, set the environment variable ELASTIC_CONFIG\n");
      printf("*** to point to the correct copy of elastic-config, and remove the file config.cache\n");
      printf("*** before re-running configure\n");
    } 
  else if ((EC_MAJOR(ec_ver) != EC_MAJOR_VERSION) ||
	   (EC_MINOR(ec_ver) != EC_MINOR_VERSION) ||
           (EC_MICRO(ec_ver) != EC_MICRO_VERSION))
    {
      printf("*** ELASTIC header files (version %d.%d.%d) do not match\n",
	     EC_MAJOR_VERSION, EC_MINOR_VERSION, EC_MICRO_VERSION);
      printf("*** library (version %d.%d.%d)\n",
	     EC_MAJOR(ec_ver), EC_MINOR(ec_ver), EC_MICRO(ec_ver));
    }
  else
    {
      if ((EC_MAJOR(ec_ver) > major) ||
        ((EC_MAJOR(ec_ver) == major) && (EC_MINOR(ec_ver) > minor)) ||
        ((EC_MAJOR(ec_ver) == major) && (EC_MINOR(ec_ver) == minor) && (EC_MICRO(ec_ver) >= micro)))
      {
        return 0;
       }
     else
      {
        printf("\n*** An old version of elastiC (%d.%d.%d) was found.\n",
               EC_MAJOR(ec_ver), EC_MINOR(ec_ver), EC_MICRO(ec_ver));
        printf("*** You need a version of elastiC newer than %d.%d.%d. The latest version of\n",
	       major, minor, micro);
        printf("*** elastiC is always available from http://www.elasticworld.org.\n");
        printf("***\n");
        printf("*** If you have already installed a sufficiently new version, this error\n");
        printf("*** probably means that the wrong copy of the elastic-config shell script is\n");
        printf("*** being found. The easiest way to fix this is to remove the old version\n");
        printf("*** of elastiC, but you can also set the ELASTIC_CONFIG environment to point to the\n");
        printf("*** correct copy of elastic-config. (In this case, you will have to\n");
        printf("*** modify your LD_LIBRARY_PATH enviroment variable, or edit /etc/ld.so.conf\n");
        printf("*** so that the correct libraries are found at run-time))\n");
      }
    }
  return 1;
}
],, no_elastic=yes,[echo $ac_n "cross compiling; assumed OK... $ac_c"])
       CFLAGS="$ac_save_CFLAGS"
       LIBS="$ac_save_LIBS"
     fi
  fi
  if test "x$no_elastic" = x ; then
     AC_MSG_RESULT(yes)
     ifelse([$2], , :, [$2])     
  else
     AC_MSG_RESULT(no)
     if test "$ELASTIC_CONFIG" = "no" ; then
       echo "*** The elastic-config script installed by ELASTIC could not be found"
       echo "*** If elastiC was installed in PREFIX, make sure PREFIX/bin is in"
       echo "*** your path, or set the ELASTIC_CONFIG environment variable to the"
       echo "*** full path to elastic-config."
     else
       if test -f conf.elastictest ; then
        :
       else
          echo "*** Could not run elastiC test program, checking why..."
          CFLAGS="$CFLAGS $ELASTIC_CFLAGS"
          LIBS="$LIBS $ELASTIC_LIBS"
          AC_TRY_LINK([
#include <elastic/elastic.h>
#include <stdio.h>
],      [ return EcVersionNumber(); ],
        [ echo "*** The test program compiled, but did not run. This usually means"
          echo "*** that the run-time linker is not finding elastiC or finding the wrong"
          echo "*** version of elastiC. If it is not finding elastiC, you'll need to set your"
          echo "*** LD_LIBRARY_PATH environment variable, or edit /etc/ld.so.conf to point"
          echo "*** to the installed location  Also, make sure you have run ldconfig if that"
          echo "*** is required on your system"
	  echo "***"
          echo "*** If you have an old version installed, it is best to remove it, although"
          echo "*** you may also be able to get things to work by modifying LD_LIBRARY_PATH" ],
        [ echo "*** The test program failed to compile or link. See the file config.log for the"
          echo "*** exact error that occured. This usually means elastiC was incorrectly installed"
          echo "*** or that you have moved elastiC since it was installed. In the latter case, you"
          echo "*** may want to edit the elastic-config script: $ELASTIC_CONFIG" ])
          CFLAGS="$ac_save_CFLAGS"
          LIBS="$ac_save_LIBS"
       fi
     fi
     ELASTIC_CFLAGS=""
     ELASTIC_LIBS=""
     ifelse([$3], , :, [$3])
  fi
  AC_SUBST(ELASTIC_CFLAGS)
  AC_SUBST(ELASTIC_LIBS)
  rm -f conf.elastictest
])
dnl AC_ELASTIC_CHECK_SIZEOF(TYPE [, CROSS-SIZE])
AC_DEFUN(AC_ELASTIC_CHECK_SIZEOF,
[dnl 
dnl Get the size of an elastiC type
dnl
AC_ARG_WITH(elastic-prefix,[  --with-elastic-prefix=PFX   Prefix where elastiC is installed (optional)],
            elastic_config_prefix="$withval", elastic_config_prefix="")
AC_ARG_WITH(elastic-exec-prefix,[  --with-elastic-exec-prefix=PFX Exec prefix where elastiC is installed (optional)],
            elastic_config_exec_prefix="$withval", elastic_config_exec_prefix="")
AC_ARG_ENABLE(elastictest, [  --disable-elastictest       Do not try to compile and run a test elastiC program],
		    , enable_elastictest=yes)

  if test x$elastic_config_exec_prefix != x ; then
     elastic_config_args="$elastic_config_args --exec-prefix=$elastic_config_exec_prefix"
     if test x${ELASTIC_CONFIG+set} != xset ; then
        ELASTIC_CONFIG=$elastic_config_exec_prefix/bin/elastic-config
     fi
  fi
  if test x$elastic_config_prefix != x ; then
     elastic_config_args="$elastic_config_args --prefix=$elastic_config_prefix"
     if test x${ELASTIC_CONFIG+set} != xset ; then
        ELASTIC_CONFIG=$elastic_config_prefix/bin/elastic-config
     fi
  fi
  AC_PATH_PROG(ELASTIC_CONFIG, elastic-config, no)

  no_elastic=""
  if test "$ELASTIC_CONFIG" = "no" ; then
    no_elastic=yes
  else
    ELASTIC_CFLAGS=`$ELASTIC_CONFIG $elastic_config_args --cflags`
    ELASTIC_LIBS=`$ELASTIC_CONFIG $elastic_config_args --libs`
    ELASTIC_CCSHARED=`$ELASTIC_CONFIG $elastic_config_args --ccshared`
    ELASTIC_LDSHARED=`$ELASTIC_CONFIG $elastic_config_args --ldshared`
    ELASTIC_LINKFORSHARED=`$ELASTIC_CONFIG $elastic_config_args --linkforshared`
    ELASTIC_SO=`$ELASTIC_CONFIG $elastic_config_args --so`
    ELASTIC_MKMODULE=`$ELASTIC_CONFIG $elastic_config_args --makemodule`
    ELASTIC_MKMODULEFLAGS=`$ELASTIC_CONFIG $elastic_config_args --makemoduleflags`
    ELASTIC_MKMODULELIBS=`$ELASTIC_CONFIG $elastic_config_args --makemodulelibs`
    elastic_config_major_version=`$ELASTIC_CONFIG $elastic_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    elastic_config_minor_version=`$ELASTIC_CONFIG $elastic_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    elastic_config_micro_version=`$ELASTIC_CONFIG $elastic_config_args --version | \
           sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    ac_save_CFLAGS="$CFLAGS"
    ac_save_LIBS="$LIBS"
    CFLAGS="$CFLAGS $ELASTIC_CFLAGS"
    LIBS="$ELASTIC_LIBS $LIBS"
  fi
changequote(<<, >>)dnl
dnl The name to #define.
define(<<AC_TYPE_NAME>>, translit(sizeof_$1, [a-z *], [A-Z_P]))dnl
dnl The cache variable name.
define(<<AC_CV_NAME>>, translit(ac_cv_sizeof_$1, [ *], [_p]))dnl
changequote([, ])dnl
AC_MSG_CHECKING(size of $1 in elastiC environment)
AC_CACHE_VAL(AC_CV_NAME,
[AC_TRY_RUN([#include <stdio.h>
#include <elastic/basic.h>
#include <elastic/elastic.h>
int main(void)
{
  FILE *f=fopen("conftestval", "w");
  if (!f) exit(1);
  fprintf(f, "%d\n", sizeof($1));
  exit(0);
}
], AC_CV_NAME=`cat conftestval`, AC_CV_NAME=0, ifelse([$2], , , AC_CV_NAME=$2))])dnl
AC_MSG_RESULT($AC_CV_NAME)
AC_TYPE_NAME=$AC_CV_NAME
dnl undefine([AC_TYPE_NAME])dnl
dnl undefine([AC_CV_NAME])dnl
CFLAGS="$ac_save_CFLAGS"
LIBS="$ac_save_LIBS"
])
