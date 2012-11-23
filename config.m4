dnl $Id$
dnl config.m4 for extension consistent

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(consistent, for consistent support,
Make sure that the comment is aligned:
[  --with-consistent             Include consistent support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(consistent, whether to enable consistent support,
dnl Make sure that the comment is aligned:
dnl [  --enable-consistent           Enable consistent support])

if test "$PHP_CONSISTENT" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-consistent -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/consistent.h"  # you most likely want to change this
  dnl if test -r $PHP_CONSISTENT/$SEARCH_FOR; then # path given as parameter
  dnl   CONSISTENT_DIR=$PHP_CONSISTENT
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for consistent files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       CONSISTENT_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$CONSISTENT_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the consistent distribution])
  dnl fi

  dnl # --with-consistent -> add include path
  dnl PHP_ADD_INCLUDE($CONSISTENT_DIR/include)

  dnl # --with-consistent -> check for lib and symbol presence
  dnl LIBNAME=consistent # you may want to change this
  dnl LIBSYMBOL=consistent # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $CONSISTENT_DIR/lib, CONSISTENT_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_CONSISTENTLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong consistent lib version or lib not found])
  dnl ],[
  dnl   -L$CONSISTENT_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(CONSISTENT_SHARED_LIBADD)

  PHP_NEW_EXTENSION(consistent, consistent.c conhash.c, $ext_shared)
fi
