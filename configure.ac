AC_PREREQ([2.65])

AC_INIT([Baack_API], [1.0], [dominik.baack@udo.edu])

AC_CONFIG_AUX_DIR(config)
AC_PREFIX_DEFAULT(.)             #set default_prefix to .
AC_PREFIX_PROGRAM()              #set prefix=default_prefix

#test if correct directory
AC_CONFIG_SRCDIR([dynstack/export.cpp])



AM_INIT_AUTOMAKE([foreign])

AC_PROG_CXX
AC_LANG_PUSH([C++])


AC_PROG_CXX_C_O
AC_PROG_RANLIB
#AC_PROG_LIBTOOL


#Check for doxygen
AC_CHECK_PROGS([DOXYGEN], [doxygen])
if test -z "$DOXYGEN";
   then AC_MSG_WARN([Doxygen not found - continuing without Doxygen support])
fi
AM_CONDITIONAL([HAVE_DOXYGEN],
[test -n "$DOXYGEN"])AM_COND_IF([HAVE_DOXYGEN], [AC_CONFIG_FILES([docs/Doxyfile])])

#Check if called from coconut
AC_ARG_WITH([test], AS_HELP_STRING([--with-test], [Compile all Tests]))
AC_ARG_WITH([bench], AS_HELP_STRING([--with-bench], [Compile with benchmark]))
AC_ARG_ENABLE([nococo], AS_HELP_STRING([--enable-nococo], [Enable compile without coconut]))

AC_ARG_WITH([dynstack], AS_HELP_STRING([--with-dynstack], [Compile dynstack library]))
AC_ARG_WITH([remotecontrol], AS_HELP_STRING([--with-remotecontrol], [Compile remotecontrol library]))


AS_IF([test "x$with_test" = "xyes"], [
   AM_CONDITIONAL([BAACK_WITHTEST], [test x = x])
], [
   AM_CONDITIONAL([BAACK_WITHTEST], [test x != x])
])

AS_IF([test "x$with_bench" = "xyes"], [
   AM_CONDITIONAL([BAACK_WITHBENCH], [test x = x])
], [
   AM_CONDITIONAL([BAACK_WITHBENCH],[test x != x])
])


AS_IF([test "x$with_dynstack" = "xyes"], [
   AM_CONDITIONAL([BAACK_WITH_DYNSTACK], [test x = x])
], [
   AM_CONDITIONAL([BAACK_WITH_DYNSTACK], [test x != x])
])

AS_IF([test "x$with_remotecontrol" = "xyes"], [
  AM_CONDITIONAL([BAACK_WITH_REMOTECONTROL], [test x = x])
], [
  AM_CONDITIONAL([BAACK_WITH_REMOTECONTROL], [test x != x])
])


AS_IF([test "x$enable_nococo" = "xyes"], [
  AC_DEFINE(_NOCOCO_, 1)
  AC_DEFINE(__DYNDEFAULT__, 1)
  
  BAACK_CPPFLAGS=" "
  BAACK_CPPFLAGS="$BAACK_CPPFLAGS -std=c++14"
  AC_SUBST([BAACK_CPPFLAGS])
  
  AC_MSG_NOTICE([Activate Dynstack])
  AM_CONDITIONAL([BAACK_WITH_DYNSTACK], [test x = x])
  AC_MSG_NOTICE([Activate Remotecontrol])
  AM_CONDITIONAL([BAACK_WITH_REMOTECONTROL], [test x = x])
])



AC_CHECK_LIB([pthread], [pthread_mutex_init], [
		echo "pthread library is missing but required for dynstack"])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h netdb.h netinet/in.h stdint.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([gethostbyname inet_ntoa memset select socket strerror])

#AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])


AC_OUTPUT
