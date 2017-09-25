################################################
#
# CORSIKA configuration macros
#
# Author: Ralf Ulrich
# Date: Di 10. Jul 12:36:02 CEST 2007
# Version: $Id: acinclude.m4 6144 2017-06-02 13:54:26Z pierog $
#
################################################

# ----------------------------------------------
#     compiler and system related configuration
# ----------------------------------------------
#
# Put all compiler and system/architecture specific
# configuration here.
# Follow the naming scheme:
#        <PROGRAMNAME>_FFLAGS  fortran compiler options
#        <PROGRAMNAME>_CFLAGS  C compiler options
#      <PROGRAMNAME>_CXXFLAGS  C++ compiler options
#          <PROGRAMNAME>_LIBS  linker options
#
AC_DEFUN([CORSIKA_CONFIGURE_COMPILER],
[

  # ------- C compiler file for output > 2GB in all cases --------
  CFLAGS="$CFLAGS -D_FILE_OFFSET_BITS=64"
  CXXFLAGS="$CXXFLAGS -D_FILE_OFFSET_BITS=64"
  LDFLAGS="$LDFLAGS -D_FILE_OFFSET_BITS=64"
  # ------- fortran compiler ------------
  LF77=`echo $F77 | cut -d " " -f 1`
  if test "$(basename $LF77)" = "pgf77" ; then
     FFLAGS="$FFLAGS -Mlfs" # allow large file (>2 GB)
  fi
  USE_GFORTRAN=false
  USE_GFORTRANEW=false
  if test "$(basename $LF77)" = "gfortran" ; then
    FFLAGS="$FFLAGS -std=legacy" # fbounds-check not compatible with A(*) in gfortran ... so no check !
    gfortran_version1=`gfortran --version | grep -i -m 1 fortran | cut -f 4 -d " " | cut -f 1 -d "." `
    gfortran_version2=`gfortran --version | grep -i -m 1 fortran | cut -f 4 -d " " | cut -f 2 -d "." `
    if test $gfortran_version1 -eq 4 && test $gfortran_version2 -ge 2 ;then
       USE_GFORTRANEW=true
       FFLAGS="$FFLAGS -Wtabs"
    fi
    USE_GFORTRAN=true
    AC_DEFINE(__GFORTRAN__,1,[In case of GFORTRAN compiler (GCC > 4)])
    CORSIKA_APPEND_OPTION([GFORTRAN])
  fi
  USE_IFORT=false
  if test "$(basename $LF77)" = "ifort" ; then
     FFLAGS="$FFLAGS -assume byterecl -O0" # on ifort the record length for binary fortran output is in words not in bytes and force no optimization to avoid problem with linking of library
     AC_DEFINE(__IFORT__,1,[In case of IFORT compiler])
     USE_IFORT=true
     AR=xiar
  fi
  if test "$(basename $CXX)" = "g++" ; then
    gcc_version1=`gcc --version | grep -i -m 1 gcc | cut -f 4 -d " " | cut -f 1 -d "." `
    gcc_version2=`gcc --version | grep -i -m 1 gcc | cut -f 4 -d " " | cut -f 2 -d "." `
    if test $gcc_version1 -ge 4 && test $gcc_version2 -ge 6 && test "x$corsika_uname_system" != xDarwin;then
       CXXFLAGS="$CXXFLAGS -Wl,--no-as-needed"
       LDFLAGS="$LDFLAGS -Xlinker --no-as-needed"
    fi
    USE_GFORTRAN=true
    AC_DEFINE(__GFORTRAN__,1,[In case of GFORTRAN compiler (GCC > 4)])
    CORSIKA_APPEND_OPTION([GFORTRAN])
  fi

  INTT=
  PLIB1=
  PLIB2=
  BERNOS=
  USEROS="`uname`"
  NOTUSE32=false
  case $host in
       *86*64*linux* ) if test "$(basename $LF77)" = "g77" ; then
                          FFLAGS="$FFLAGS -fbounds-check"
                       fi
                       CORSIKA_FFLAGS="-O0"
                       BERNOS="Linux"
                       AC_MSG_CHECKING([32 bits compatibility])
                       AC_LANG_PUSH(Fortran 77)
                       _CORSIKA_SAVE_COMPILE_FLAGS
                       FFLAGS="$FFLAGS -m32"
                       AC_LINK_IFELSE(
                       [AC_LANG_PROGRAM([],[[]])],
                        [
                         NOTUSE32=false
                         AC_MSG_RESULT([yes])
                        ],[
                         AC_MSG_RESULT([no])
                         NOTUSE32=true
                        ])
                       _CORSIKA_RESTORE_COMPILE_FLAGS
                       AC_LANG_POP(Fortran 77)
                       ;;

       *86*linux*    ) if test "$(basename $LF77)" = "g77" ; then
                          FFLAGS="$FFLAGS -fbounds-check"
                       fi
                       CORSIKA_FFLAGS="-O0"
                       BERNOS="Linux"
                       ;;

       *darwin*	     ) if test "$(basename $LF77)" = "g77" ; then
                          FFLAGS="$FFLAGS -fbounds-check"
                       fi
                       CORSIKA_FFLAGS="-O0"
                       BERNOS="mac"
                       ;;

       *alpha*dec*   ) FFLAGS="$FFLAGS -check_bounds"
                       BERNOS="dec"
                       ;;

       *aix*	     ) FFLAGS="$FFLAGS -C -qextname -qsave -qmaxmem=-1"
                       BERNOS="aix"
                       ;;

       *hp*ux*	     ) FFLAGS="$FFLAGS -K +E1 +ppu +Dportable +Onolimit"
                       BERNOS="hpux"
                       ;;

       *sun*	     ) BERNOS="sun"
                       ;;

       *             ) BERNOS="`echo $host | cut -d- -f 3-`"
                       ;;
  esac

  case $host in
       *m68k*apple* ) CORSIKA_APPEND_OPTION([MACM68])
                      AC_DEFINE(__MAC__,1,[In case of Apple computer])
                      ;;

       *            ) AC_DEFINE(__UNIX__,1,[In case of UNIX computer])
                      case $host in        #RECL definition
	                    *dec* ) ;;
	                    *     ) AC_DEFINE(__BYTERECL__,1,[RECL parameter]) ;;

	              esac
                      ;;
  esac

  # -------- pre compiler --------------
  PRECOMPILER_FLAGS="-DHAVE_CONFIG_H -I\$(top_srcdir)/include"
  CAT_CMD="cat"
  case $BERNOS in
     aix   ) PRECOMPILER_FLAGS="$PRECOMPILER_FLAGS -traditional -P"
             CAT_CMD="cat -r"
             ;;
     dec   ) CAT_CMD="cat -r"
             ;;
     Linux ) PRECOMPILER_FLAGS="$PRECOMPILER_FLAGS -traditional-cpp"
             CAT_CMD="cat -s"
             ;;
     mac   ) PRECOMPILER_FLAGS="$PRECOMPILER_FLAGS -traditional"
             CAT_CMD="cat -s"
             ;;
     *     ) PRECOMPILER_FLAGS="$PRECOMPILER_FLAGS -traditional-cpp -P"
             CAT_CMD="cat"
             ;;
  esac
  AC_SUBST([CAT_CMD])
  AC_SUBST([PRECOMPILER_FLAGS])

  # ------ trapfe -------
  AM_CONDITIONAL([USE_TRAPFE], test "x$(basename $LF77)" = "xg77" && test "x$BERNOS" = "xLinux" )


  # ------ urqmd -------
  case $USEROS in
    AIX    ) URQMD_FFLAGS=$FO5 ' -qextname -qstrict -qipa=partition=large' ;;
    Linux  ) if $USE_IFORT ; then
               URQMD_FFLAGS=
             elif $USE_GFORTRAN ; then
               URQMD_FFLAGS="$FO3 -Wsurprising"
             else
               URQMD_FFLAGS="$FO3  -Wall -Wsurprising"
             fi ;;
    Intel  ) URQMD_FFLAGS=$FO3 ' -axMK -tpp6 -w95 -ipo' ;;
    HP-UX  ) URQMD_FFLAGS='-w' ;;
    IRIX64 ) URQMD_FFLAGS='-mips4 -64 -r10000' ;;
    OSF1   ) URQMD_FFLAGS='-C -align dcommons' ;;
    SunOS  ) URQMD_FFLAGS='-w' ;;
    *      ) URQMD_FFLAGS= ;;
  esac

  # ------ dpmjet ------
  if $USE_GFORTRAN ; then
    DPM_FFLAGS=
  else
    case $USEROS in
      Linux  ) DPM_FFLAGS="-C -fno-automatic -finit-local-zero -O -Wuninitialized -malign-double" ;;
      OSF1   ) DPM_FFLAGS="-C -fpe4 -check_underflow -check_overflow -g3 $FO1" ;;
      *      ) DPM_FFLAGS="" ;;
    esac
  fi

  # ------ bernlohr ----------
  AC_CHECK_HEADER([stdint.h],[],[
     case $host in
       *osf* ) BERN_CFLAGS="-std0" ;;
       *aix* ) BERN_CFLAGS="-qcpluscmt" ;;  #not enough to make it compile
     esac
  ])
  if test "$(basename $LF77)" = "g77" ; then
     BERN_CPPFLAGS="-D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE"
  fi

  # ----- baack ------------



  # ------- fluka ----------
  if test "$(basename $LF77)" = "pgf77" ; then
     FLUKA_FFLAGS="-g77libs"
  fi
  if test "$(basename $LF77)" = "gfortran" ; then
     FLUKA_FFLAGS="-lm"
  fi

])



# ----------------------------------------------
#             time library
# ----------------------------------------------

AC_DEFUN([CORSIKA_CONFIGURE_TIME],
[
  AC_ARG_VAR(CORTIMELIB,
             AC_HELP_STRING([CORTIMELIB=<time-library>],
                            [which time library to use (default=TIMEAUTO)]))

  AC_MSG_CHECKING([time library])

  ######
  # conditionals for automake of src/Makefile.am
  AM_CONDITIONAL([USE_TIMERC], test "x$CORTIMELIB" = "xTIMERC" )

  ######
  # check for conflicts
  CORSIKA_CHECK_CONFLICT([$CORTIMELIB])
  CORSIKA_CACHE([$CORTIMELIB])

  ######
  # - set defines for CORSIKA pre-compiler (config.h)
  # - add necessary includes and libs for corsika compilation
  case $CORTIMELIB in

 	TIMEAUTO )
                 if test "$(basename $LF77)" = "pgf77" ; then
                   AC_DEFINE([__TIMERC__], 1, [timerc routine])
                   AC_MSG_RESULT([pgf77])
                 else
                   if test "$BERNOS" = "aix" ; then
                      AC_DEFINE([__IBMRISC__],1,[ibmrisc routine])
                      AC_MSG_RESULT([ibm/risc])
                   else
                      AC_LANG_PUSH(Fortran 77)
                      _CORSIKA_SAVE_COMPILE_FLAGS
                       AC_LINK_IFELSE(
                       [AC_LANG_SOURCE([[
                            CHARACTER*8      YYYYMMDD
                            CHARACTER*10     HHMMSS
                            CALL DATE_AND_TIME( YYYYMMDD, HHMMSS )
                        ]])],[
                         AC_MSG_RESULT([new])
                        ],[
                         AC_LANG_PUSH(C)
                         AC_COMPILE_IFELSE([
                           AC_LANG_PROGRAM([
                            #include <sys/types.h>
                            #include <sys/timeb.h>
                            ],[[
                            struct timeb Time_Struct;]])
	                 ],[
                            AC_DEFINE([__TIMERC__],1,[timerc routine])
                            AC_MSG_RESULT([rc])
                            AM_CONDITIONAL([USE_TIMERC], test x = x )
                         ],[
                            AC_LANG_PUSH(Fortran 77)
                            AC_LINK_IFELSE(
                            [AC_LANG_SOURCE([[
                               INTEGER IYEAR,MONTH,IDAY
                               CALL IDATE( MONTH,IDAY,IYEAR )
                            ]])],[
                             AC_DEFINE([__OLDDATE__],1,[old date and time routine])
                             AC_MSG_RESULT([old])
                            ],[
                             AC_MSG_RESULT([no])
                             CORSIKA_MISSING_REQUIRED_OPTION([CORTIMELIB])
                            ])
                            AC_LANG_POP(Fortran 77)
                         ])
                         AC_LANG_POP(C)
                        ])
                      _CORSIKA_RESTORE_COMPILE_FLAGS
                      AC_LANG_POP(Fortran 77)
                   fi
                  fi
	         ;;

 	TIMENEW )
                 AC_LANG_PUSH(Fortran 77)
                 _CORSIKA_SAVE_COMPILE_FLAGS
                 AC_LINK_IFELSE(
                  [AC_LANG_SOURCE([[
                     CHARACTER*8      YYYYMMDD
                     CHARACTER*10     HHMMSS
                     CALL DATE_AND_TIME( YYYYMMDD, HHMMSS )
                  ]])],[
                     AC_MSG_RESULT([new])
                 ],[
                     AC_MSG_RESULT([no])
                     CORSIKA_MISSING_REQUIRED_OPTION([CORTIMELIB])
                 ])
                 _CORSIKA_RESTORE_COMPILE_FLAGS
                 AC_LANG_POP(Fortran 77)
                 ;;

        TIMEOLD )
                 AC_LANG_PUSH(Fortran 77)
                 _CORSIKA_SAVE_COMPILE_FLAGS
                 AC_LINK_IFELSE(
                  [AC_LANG_SOURCE([[
                    INTEGER          IYEAR,MONTH,IDAY
                    CALL IDATE( MONTH,IDAY,IYEAR )
                  ]])],[
                     AC_DEFINE([__OLDDATE__],1,[old date and time routine])
                     AC_MSG_RESULT([old])
                 ],[
                     AC_MSG_RESULT([no])
                     CORSIKA_MISSING_REQUIRED_OPTION([CORTIMELIB])
                 ])
                 _CORSIKA_RESTORE_COMPILE_FLAGS
                 AC_LANG_POP(Fortran 77)
		 ;;

	TIMERC  )
                 AC_MSG_RESULT([rc])
                 AC_LANG_PUSH(C)
                 _CORSIKA_SAVE_COMPILE_FLAGS
                 AC_COMPILE_IFELSE([
                  AC_LANG_PROGRAM([
                    #include <sys/types.h>
                    #include <sys/timeb.h>
                    ],[[
                    struct timeb Time_Struct;]])
	         ],[
                    AC_DEFINE([__TIMERC__], 1, [timerc routine])
                    AM_CONDITIONAL([USE_TIMERC], test x = x )
                 ],[
                   AC_MSG_RESULT([no])
                   CORSIKA_MISSING_REQUIRED_OPTION([CORTIMELIB])
                 ])
                 _CORSIKA_RESTORE_COMPILE_FLAGS
                 AC_LANG_POP(C)
	 	 ;;

	TIMEIBM )
                 AC_DEFINE([__IBMRISC__],1,[ibmrisc routine])
                 AC_MSG_RESULT([ibm/risc])
		 ;;

	TIMEPGF77 )
                 if test "$(basename $LF77)" = "pgf77" ; then
                   AC_DEFINE(__OLDDATE2__,1,[old date and time routine for LINUX beta])
                   AC_MSG_RESULT([pgf77])
                 else
                   AC_MSG_RESULT([no])
                   CORSIKA_MISSING_REQUIRED_OPTION([CORTIMELIB])
                 fi
                 ;;

	*         )
                 AC_MSG_RESULT([!!!!!!! UNDEFINED or INVALID !!!!!!!!])
	         CORSIKA_MISSING_REQUIRED_OPTION([CORTIMELIB])
 		 ;;

   esac

])




# ----------------------------------------------
#             detector geometry
# ----------------------------------------------

AC_DEFUN([CORSIKA_CONFIGURE_DETECTOR],
[
  AC_ARG_VAR(CORDETECTOR,
             AC_HELP_STRING([CORDETECTOR=<detector-geometry>],
                            [which detector geometry to use (HORIZONTAL, VOLUMEDET, VOLUMECORR)]))

  AC_MSG_CHECKING([detector geometry])

  ######
  # check for conflicts
  CORSIKA_CHECK_CONFLICT([$CORDETECTOR])

  ######
  # - set defines for CORSIKA pre-compiler (config.h)
  # - add necessary includes and libs for corsika compilation
  case $CORDETECTOR in

 	HORIZONTAL )
                 AC_MSG_RESULT([horizontal])
                 CORSIKA_CACHE([HORIZONTAL])
                 ;;

 	VOLUMEDET )
                 AC_MSG_RESULT([volume])
                 AC_DEFINE([__VOLUMEDET__],1,[detector geometry (volume)])
                 CORSIKA_CACHE([VOLUMEDET])
                 ;;

 	VOLUMECORR )
                 AC_MSG_RESULT([vertical])
                 AC_DEFINE([__VOLUMECORR__],1,[detector geometry (vertical string)])
                 CORSIKA_APPEND_CONFLICTS([CERENKOV IACT VIEWCONE])
                 CORSIKA_CACHE([VOLUMECORR])
                 ;;

        *        )
                 AC_MSG_RESULT([!!!!!!! UNDEFINED or INVALID !!!!!!!!])
	         CORSIKA_MISSING_REQUIRED_OPTION([CORDETECTOR])
 		 ;;

   esac

])



# ----------------------------------------------
#             low energy model
# ----------------------------------------------

AC_DEFUN([CORSIKA_CONFIGURE_LEMODEL],
[
  AC_ARG_VAR(CORLEMODEL,
             AC_HELP_STRING([CORLEMODEL=<low energy model>],
                            [which hadronic low energy model to use (GHEISHA, URQMD, FLUKA)]))

  AC_MSG_CHECKING([low energy model])

  ######
  # conditionals for automake of src/Makefile.am
  AM_CONDITIONAL([USE_GHEISHA], test "x$CORLEMODEL" = "xGHEISHA" )
  AM_CONDITIONAL([USE_URQMD], test "x$CORLEMODEL" = "xURQMD")
  AM_CONDITIONAL([USE_FLUKA], test "x$CORLEMODEL" = "xFLUKA")

  ######
  # check for conflicts
  CORSIKA_CHECK_CONFLICT([$CORLEMODEL])

  ######
  # - set defines for CORSIKA pre-compiler (config.h)
  # - add necessary includes and libs for corsika compilation
  case $CORLEMODEL in

      GHEISHA    )
                 AC_MSG_RESULT([gheisha])
                 LOMODEL="_gheisha" # for suffix generation for corsika binary/source name
                 AC_DEFINE([__GHEISHAD__],[1],[include GHEISHA program])
                 CORSIKA_CACHE([GHEISHA])
                 ;;

      URQMD      )
                 AC_MSG_RESULT([urqmd])
                 LOMODEL="_urqmd" # for suffix generation for corsika binary/source name
                 CORSIKA_SUBDIRS="urqmd $CORSIKA_SUBDIRS"  # include for compilation
                 CORSIKA_FFLAGS="$CORSIKA_FFLAGS -I\$(top_srcdir)/urqmd"
                 MODEL_LIBS="$MODEL_LIBS -lurqmd"
                 URQMD_FFLAGS="$URQMD_FFLAGS -I\$(srcdir)"
                 AC_DEFINE([__URQMD__],[1],[include UrQMD 1.3 program])
                 AC_SUBST([URQMD_FFLAGS])
                 CORSIKA_CACHE([URQMD])
                 ;;

      FLUKA      )
                 AC_MSG_RESULT([fluka])
                 LOMODEL="_fluka" # for suffix generation for corsika binary/source name
                 CORSIKA_REQUIRE_FLUKA
                 CORSIKA_FFLAGS="$CORSIKA_FFLAGS $FLUKA_FFLAGS"
                 MODEL_LIBS="$MODEL_LIBS $FLUKA_LIBS"
                 AC_DEFINE([__FLUKA__],[1],[include FLUKA 2011 program])
                 CORSIKA_APPEND_CONFLICTS([DPMJET])
                 CORSIKA_CACHE([FLUKA])
                 ;;

        *        )
                 AC_MSG_RESULT([!!!!!!! UNDEFINED or INVALID !!!!!!!!])
	         CORSIKA_MISSING_REQUIRED_OPTION([CORLEMODEL])
 		 ;;

   esac

])





# ----------------------------------------------
#             high energy model
# ----------------------------------------------

AC_DEFUN([CORSIKA_CONFIGURE_HEMODEL],
[
  AC_ARG_VAR(CORHEMODEL,
             AC_HELP_STRING([CORHEMODEL=<high energy model>],
                            [which hadronic high energy model to use (QGSJET01, QGSJETII, SIBYLL, VENUS, EPOS, DPMJET, NEXUS)]))

  AC_MSG_CHECKING([high energy model])

  ######
  # conditionals for automake of src/Makefile.am
  AM_CONDITIONAL([USE_QGSJET01], test "x$CORHEMODEL" = "xQGSJET01")
  AM_CONDITIONAL([USE_QGSJETII], test "x$CORHEMODEL" = "xQGSJETII")
  AM_CONDITIONAL([USE_SIBYLL], test "x$CORHEMODEL" = "xSIBYLL")
  AM_CONDITIONAL([USE_VENUS], test "x$CORHEMODEL" = "xVENUS")
  AM_CONDITIONAL([USE_EPOS], test "x$CORHEMODEL" = "xEPOS")
  AM_CONDITIONAL([USE_DPMJET], test "x$CORHEMODEL" = "xDPMJET")
  AM_CONDITIONAL([USE_NEXUS], test "x$CORHEMODEL" = "xNEXUS")

  ######
  # check for conflicts
  CORSIKA_CHECK_CONFLICT([$CORHEMODEL])

  ######
  # - set defines for CORSIKA pre-compiler (config.h)
  # - add necessary includes and libs for corsika compilation
  case $CORHEMODEL in

      QGSJET01  )
                 AC_MSG_RESULT([qgsjet01])
                 HIMODEL="_QGSJET" # for suffix generation for corsika binary/source name
                 AC_DEFINE([__QGSJETOLD__],[1],[include QGSJET 01C program])
                 AC_DEFINE([__QGSJET__],[1],[include QGSJET01/QGSJET II program])
                 CORSIKA_CACHE([QGSJET01])
                 ;;

      QGSJETII    )
                 AC_MSG_RESULT([qgsjetII])
                 HIMODEL="_QGSII" # for suffix generation for corsika binary/source name
                 AC_DEFINE([__QGSII__],[1],[include QGSJET II program])
                 AC_DEFINE([__QGSJET__],[1],[include QGSJET01/QGSJET II program])
                 CORSIKA_CACHE([QGSJETII])
                 ;;

      SIBYLL    )
                 AC_MSG_RESULT([sibyll])
                 HIMODEL="_SIBYLL" # for suffix generation for corsika binary/source name
                 AC_DEFINE([__SIBYLL__],[1],[include Sybill 2.3 program])
                 CORSIKA_CACHE([SIBYLL])
                ;;

      NEXUS    )
                 AC_MSG_RESULT([nexus])
                 HIMODEL="_NEXUS" # for suffix generation for corsika binary/source name
                 CORSIKA_SUBDIRS="nexus $CORSIKA_SUBDIRS"  # include for compilation
                 MODEL_LIBS="$MODEL_LIBS -lnexus"
                 NEXUS_FFLAGS="-O0 -I\$(srcdir)"
                 AC_SUBST([NEXUS_FFLAGS])
                 AC_DEFINE([__NEXUS__],[1],[include neXus 3.97 program])
                 CORSIKA_CACHE([NEXUS])
                 ;;

      DPMJET    )
                 AC_MSG_RESULT([dpmjet])
                 HIMODEL="_DPMJET" # for suffix generation for corsika binary/source name
                 CORSIKA_SUBDIRS="dpmjet $CORSIKA_SUBDIRS" # include for compilation
                 MODEL_LIBS="$MODEL_LIBS -ldpm"
                 AC_SUBST([DPM_FFLAGS])
                 AC_DEFINE([__DPMJET__],[1],[include DPMJET 2.55 program])
	         CORSIKA_APPEND_CONFLICTS([FLUKA GFORTRAN])
                 CORSIKA_CACHE([DPMJET])
                 ;;

      VENUS    )
                 AC_MSG_RESULT([venus])
                 HIMODEL="_VENUS" # for suffix generation for corsika binary/source name
                 AC_DEFINE([__VENUS__],[1],[include VENUS 4.12 program])
                 CORSIKA_CACHE([VENUS])
                 ;;

      EPOS    )
                 AC_MSG_RESULT([epos])
                 HIMODEL="_EPOS" # for suffix generation for corsika binary/source name
                 CORSIKA_SUBDIRS="epos $CORSIKA_SUBDIRS"  # include for compilation
                 MODEL_LIBS="$MODEL_LIBS -lepos"
                 EPOS_FFLAGS="-O0 -I\$(srcdir)"
                 AC_SUBST([EPOS_FFLAGS])
                 AC_DEFINE([__EPOS__],[1],[include EPOS program])
                 CORSIKA_CACHE([EPOS])
                 ;;

        *        )
                 AC_MSG_RESULT([!!!!!!! UNDEFINED or INVALID !!!!!!!!])
	         CORSIKA_MISSING_REQUIRED_OPTION([CORHEMODEL])
 		 ;;

   esac

])



#################################################################################
#             corsika options
#################################################################################
#
# use of macro
#  _CORSIKA_ENABLE_OPTION(name, conflicts, default, help, define, if-enabled)
#             name:  name of option (--enable-<name>)
#        conflicts:  list of option name conflicting with this one
#         requires:  list of option absolutely required for this option
#          default:  default value: yes|no
#             help:  help string
#           define:  CORSIKA pre-compiler define
#         if-found:  optional code to be executed, if option is used
#
# ----------------------------------------------


AC_DEFUN([CORSIKA_ENABLE_CERENKOV],
[_CORSIKA_ENABLE_OPTION([CERENKOV],
                        [VOLUMECORR INTTEST ANAHIST AUGERHIST MUONHIST AUGCERLONG COMPACT ICECUBE1 ICECUBE2], [],
                        [no], [for Cherenkov grid detector], [__CERENKOV__],[])])

AC_DEFUN([CORSIKA_ENABLE_IACT],
[_CORSIKA_ENABLE_OPTION([IACT],
                        [VOLUMECORR INTTEST ANAHIST AUGERHIST MUONHIST AUGCERLONG COMPACT], [CERENKOV],
                        [no], [for Cherenkov telescope detector], [__IACT__],
    [
    CORSIKA_LIBS="$CORSIKA_LIBS -liact"
    CORSIKA_REQUIRE_BERNLOHR
    BERNLIBS="$BERNLIBS libiact.a libtestlib.a"
    ])
])

AC_DEFUN([CORSIKA_ENABLE_IACTEXT],
[_CORSIKA_ENABLE_OPTION([IACTEXT], [], [], [no], [for external IACT Cherenkov file], [__IACTEXT__],
    [
    BERNIACTEXT_CPPFLAGS="-DIACTEXT"
    ])
    AC_SUBST([BERNIACTEXT_CPPFLAGS])
])

AC_DEFUN([CORSIKA_ENABLE_INTCLONG],
[_CORSIKA_ENABLE_OPTION([INTCLONG], [], [], [no], [new Cherenkov counting], [__INTCLONG__], [])])

AC_DEFUN([CORSIKA_ENABLE_NOCLONG],
[_CORSIKA_ENABLE_OPTION([NOCLONG], [], [], [no], [no cherenkov longitudinla profile], [__NOCLONG__], [])])

AC_DEFUN([CORSIKA_ENABLE_CERWLEN],
[_CORSIKA_ENABLE_OPTION([CERWLEN], [], [], [no], [Cherenkov wavelength dependent], [__CERWLEN__], [])])

AC_DEFUN([CORSIKA_ENABLE_CEFFIC],
[_CORSIKA_ENABLE_OPTION([CEFFIC],
                        [INTTEST CURVED AUGCERLONG], [],
                        [no], [simulate telescope detector], [__CEFFIC__], [])])


AC_DEFUN([CORSIKA_ENABLE_ATMEXT],
[_CORSIKA_ENABLE_OPTION([ATMEXT], [INTTEST], [], [no], [external atmosphere], [__ATMEXT__],
    [
    CORSIKA_REQUIRE_BERNLOHR
    CORSIKA_LIBS="$CORSIKA_LIBS -latmext"
    BERNLIBS="$BERNLIBS libatmext.a"
    BERNNOATMEXT_CPPFLAGS=""
    ])
  if test x$corsika_cv_enable_ATMEXT = x || test x$corsika_cv_enable_ATMEXT = xno ; then
     BERNNOATMEXT_CPPFLAGS="-DNO_EXTERNAL_ATMOSPHERES"
  fi
  AC_SUBST([BERNNOATMEXT_CPPFLAGS])
])

AC_DEFUN([CORSIKA_ENABLE_THIN],
[_CORSIKA_ENABLE_OPTION([THIN], [INTTEST], [], [no], [for thinning], [__THIN__], [INTT="_thin"])])

AC_DEFUN([CORSIKA_ENABLE_MULTITHIN],
[_CORSIKA_ENABLE_OPTION([MULTITHIN], [ANAHIST AUGERHIST INTTEST THIN ICECUBE1 ICECUBE2 ], [], [no], [for multiple thinning], [__MULTITHIN__], [], [])])

AC_DEFUN([CORSIKA_ENABLE_NEUTRINO],
[_CORSIKA_ENABLE_OPTION([NEUTRINO], [INTTEST], [], [no], [neutrino primaries], [__NEUTRINO__], [])])

AC_DEFUN([CORSIKA_ENABLE_ICECUBE1],
[_CORSIKA_ENABLE_OPTION([ICECUBE1], [INTTEST PARALLEL PARALLELIB MULTITHIN CERENKOV COAST COASTUSERLIB COMPACT], [], [no], [ICECUBE FiFo], [__ICECUBE1__], [])])

AC_DEFUN([CORSIKA_ENABLE_ICECUBE2],
[_CORSIKA_ENABLE_OPTION([ICECUBE2], [INTTEST PARALLEL PARALLELIB MULTITHIN CERENKOV COAST COASTUSERLIB COMPACT], [], [no], [ICECUBE gzip/pipe output], [__ICECUBE2__], [])])

AC_DEFUN([CORSIKA_ENABLE_BAACKC0X],
[_CORSIKA_ENABLE_OPTION([BAACKC0X], [], [], [no], [BAACK modules with C++0x option],[__BAACKC0X__],
    [
    BAACK_CPPFLAGS="$BAACK_CPPFLAGS -std=c++0x"
    AC_SUBST([BAACK_CPPFLAGS])
    ])])

AC_DEFUN([CORSIKA_ENABLE_BAACKC11],
[_CORSIKA_ENABLE_OPTION([BAACKC11], [], [], [no], [BAACK modules with C++11 option],[__BAACKC11__],
    [
    BAACK_CPPFLAGS="$BAACK_CPPFLAGS -std=c++11"
    AC_SUBST([BAACK_CPPFLAGS])
    ])])

AC_DEFUN([CORSIKA_ENABLE_BAACKC14],
[_CORSIKA_ENABLE_OPTION([BAACKC14], [], [], [no], [BAACK modules with C++14 option],[__BAACKC14__],
    [
    BAACK_CPPFLAGS="$BAACK_CPPFLAGS -std=c++14"
    AC_SUBST([BAACK_CPPFLAGS])
    ])])

AC_DEFUN([CORSIKA_ENABLE_BAACKC1Z],
[_CORSIKA_ENABLE_OPTION([BAACKC1Z], [], [], [no], [BAACK modules with C++1z option],[__BAACKC1Z__],
    [
    BAACK_CPPFLAGS="$BAACK_CPPFLAGS -std=c++17"
    AC_SUBST([BAACK_CPPFLAGS])
    ])])


AC_DEFUN([CORSIKA_ENABLE_DYNSTACK],
[_CORSIKA_ENABLE_OPTION([DYNSTACK],
                        [INTTEST PARALLEL PARALLELLIB COMPACT], [],
                        [no], [DYNamic particle STACK], [__DYNSTACK__],
    [
    CORSIKA_LIBS="$CORSIKA_LIBS -lDynstack -lBaack"
    CORSIKA_REQUIRE_BAACK
    BAACKLIBS="$BAACKLIBS libDynstack.a libBaack.a"
    AM_CONDITIONAL([BAACK_WITH_DYNSTACK], [test x = x])
    ])
  if test x$corsika_cv_enable_DYNSTACK = x || test x$corsika_cv_enable_DYNSTACK = xno ; then
     AM_CONDITIONAL([BAACK_WITH_DYNSTACK], [test x != x])
  fi
])

AC_DEFUN([CORSIKA_ENABLE_DYNDEFAULT],
[_CORSIKA_ENABLE_OPTION([DYNDEFAULT], [], [DYNSTACK], [no], [emulation of normal stack], [__DYNDEFAULT__], [])])

AC_DEFUN([CORSIKA_ENABLE_DYNOUTPUT],
[_CORSIKA_ENABLE_OPTION([DYNOUTPUT], [], [DYNSTACK], [no], [stores every saved particle to file], [__DYNOUTPUT__], [])])

AC_DEFUN([CORSIKA_ENABLE_DYNICECUBE],
[_CORSIKA_ENABLE_OPTION([DYNICECUBE], [], [DYNSTACK], [no], [reimplementation of the ICECUBE1 module], [__DYNICECUBE__], [])])

AC_DEFUN([CORSIKA_ENABLE_DYNCUSTOM],
[_CORSIKA_ENABLE_OPTION([DYNCUSTOM], [], [DYNSTACK], [no], [user defined intermediate particle stack], [__DYNCUSTOM__], [])])

AC_DEFUN([CORSIKA_ENABLE_REMOTECONTROL],
[_CORSIKA_ENABLE_OPTION([REMOTECONTROL],
                        [PARALLEL PARALLELLIB], [],
                        [no], [Remote acces to corsika], [__REMOTECONTROL__],
    [
    CORSIKA_LIBS="$CORSIKA_LIBS -lRemotecontrol -lBaack"
    CORSIKA_REQUIRE_BAACK
    BAACKLIBS="$BAACKLIBS libRemotecontrol.a libBaack.a"
    FFLAGS="$FFLAGS -pthread"
    AM_CONDITIONAL([BAACK_WITH_REMOTECONTROL], [test x = x])
    ])
if test x$corsika_cv_enable_REMOTECONTROL = x || test x$corsika_cv_enable_REMOTECONTROL = xno ; then
    AM_CONDITIONAL([BAACK_WITH_REMOTECONTROL], [test x != x])
fi
])

AC_DEFUN([CORSIKA_ENABLE_CORPP],
[_CORSIKA_ENABLE_OPTION([CORPP],
                        [PARALLEL PARALLELLIB COAST], [],
                        [no], [Compile corsika as library for custom executor], [__CORPP__],
    [
    CORSIKA_LIBS="$CORSIKA_LIBS -lCorPP"
    CORSIKA_REQUIRE_BAACK
    BAACKLIBS="$BAACKLIBS libCorPP.a"
    AM_CONDITIONAL([BAACK_WITH_CORPP], [test x = x])
    ])
  if test x$corsika_cv_enable_CORPP = x || test x$corsika_cv_enable_CORPP = xno ; then
     AM_CONDITIONAL([BAACK_WITH_CORPP], [test x != x])
  fi
])

AC_DEFUN([CORSIKA_ENABLE_CORPPMAIN],
[_CORSIKA_ENABLE_OPTION([CORPPMAIN], [], [CORPP], [no], [Execute Corsika with a modern C++ system], [__CORPPMAIN__], [])])

AC_DEFUN([CORSIKA_ENABLE_CORPPLIBONLY],
[_CORSIKA_ENABLE_OPTION([CORPPLIBONLY], [], [CORPP], [no], [Compile corsika as static library], [__CORPPLIBONLY__], [])])


AC_DEFUN([CORSIKA_ENABLE_PLOTSH],
[_CORSIKA_ENABLE_OPTION([PLOTSH], [INTTEST ANAHIST AUGERHIST MUONHIST AUGCERLONG], [],
[no], [plot shower], [__PLOTSH__], [PLOTTRACKS=plottracks])
AC_SUBST([PLOTTRACKS])
])

AC_DEFUN([CORSIKA_ENABLE_PLOTSH2],
[_CORSIKA_ENABLE_OPTION([PLOTSH2], [INTTEST ANAHIST AUGERHIST MUONHIST AUGCERLONG], [],
                        [no], [plot shower C], [__PLOTSH2__], [
AC_CHECK_MAP2PNG(lmap2pgn)
if $lmap2pgn ; then
   echo '"map2pgn" properly installed in run/, continue ...'
   MAP2PNG=
else
   error_png=false
   AC_CHECK_LIB([png], [png_init_io], [
	AC_CHECK_FILE(${prefix}/run/map2png,[
		        error_png=true
			echo ' '
			echo 'The "map2png" utility to convert .map file to .png is in run/'
			echo "but it doesnot run ... (may be not compiled for $USEROS)"
			echo 'If you want to recompile it, remove the "run/map2png" file and start the installation again.'
	],[
	AC_CHECK_FILE(${prefix}/src/utils/map2png.c,[],[
		        error_png=true
			echo ' '
			echo "Warning : map2pgn.c not available in src/"
	   ])
	])
   ],[
	error_png=true
	echo ' '
	echo 'Warning : The "libpng" library is not available on your system !'
	echo 'Please install the path to this shared library in your $LD_LIBRARY_PATH to compile and run "map2png"'
   ])
if $error_png ; then
   MAP2PNG=
   echo ' '
   echo 'The utility program "map2png" to convert .map file to .png can not be installed in run/'
   echo "This program is needed to visualize the showers produced by CORSIKA with option PLOTSH2."
   echo "Please contact CORSIKA authors if you want to use this program."
   echo "Note that the PLOTSH2 option is selected anyway."
   echo "(press 'Enter' to continue ...)"
   read
else
   MAP2PNG=map2png
fi
fi
])
AC_SUBST([MAP2PNG])
])

AC_DEFUN([CORSIKA_ENABLE_INTTEST],
[_CORSIKA_ENABLE_OPTION([INTTEST],
                        [MACM68 CERENKOV IACT CEFFIC ATMEXT THIN FLUKA NEUTRINO PLOTSH PLOTSH2 CURVED UPWARD VIEWCONE ANAHIST AUGERHIST MUONHIST AUGCERLONG PRESHOWER COMPACT HERWIG STACKIN CONEX ICECUBE1 ICECUBE2], [],
                        [no], [interaction test], [__INTTEST__],
    [
    CORSIKA_REQUIRE_CERNLIB
    INTT="_int" # for suffix generation for corsika binary/source name
    ])
])

AC_DEFUN([CORSIKA_ENABLE_SLANT],
[_CORSIKA_ENABLE_OPTION([SLANT], [INTTEST], [], [no], [slant profile], [__SLANT__], [])])

AC_DEFUN([CORSIKA_ENABLE_CURVED],
[_CORSIKA_ENABLE_OPTION([CURVED], [CEFFIC INTTEST AUGERHIST], [],
                        [no], [curved atmosphere], [__CURVED__], [])])

AC_DEFUN([CORSIKA_ENABLE_UPWARD],
[_CORSIKA_ENABLE_OPTION([UPWARD], [INTTEST TRAJECT], [], [no], [upward particles], [__UPWARD__], [])])

AC_DEFUN([CORSIKA_ENABLE_VIEWCONE],
[_CORSIKA_ENABLE_OPTION([VIEWCONE], [VOLUMECORR INTTEST], [], [no], [viewcone], [__VIEWCONE__], [])])

AC_DEFUN([CORSIKA_ENABLE_ANAHIST],
[_CORSIKA_ENABLE_OPTION([ANAHIST],
                        [CERENKOV IACT PLOTSH  PLOTSH2 INTTEST COREAS INCLINED], [THIN],
                        [no], [produce analysis histograms], [__ANAHIST__],
    INTT=$INTT"_hbook" # for suffix generation for corsika binary/source name
    [CORSIKA_REQUIRE_CERNLIB])])

AC_DEFUN([CORSIKA_ENABLE_AUGERINFO],
[_CORSIKA_ENABLE_OPTION([AUGERINFO], [INTTEST], [], [no], [augerinfo], [__AUGERINFO__], [])])

AC_DEFUN([CORSIKA_ENABLE_AUGERHIT],
[_CORSIKA_ENABLE_OPTION([AUGERHIT], [CERENKOV,INTTEST,PLOTSH,PLOTSH2], [], [no], [augerhit], [__AUGERHIT__], [])])

AC_DEFUN([CORSIKA_ENABLE_AUGERHIST],
[_CORSIKA_ENABLE_OPTION([AUGERHIST],
                        [IACT PLOTSH PLOTSH2 INTTEST CURVED UPWARD COREAS INCLINED], [AUGERINFO THIN],
                        [no], [augerhist], [__AUGERHIST__],
    INTT=$INTT"_hbook" # for suffix generation for corsika binary/source name
    [CORSIKA_REQUIRE_CERNLIB])])

AC_DEFUN([CORSIKA_ENABLE_MUONHIST],
[_CORSIKA_ENABLE_OPTION([MUONHIST],
                        [IACT PLOTSH PLOTSH2 INTTEST UPWARD COREAS INCLINED], [],
                        [no], [muonhist], [__MUONHIST__],
    INTT=$INTT"_hbook" # for suffix generation for corsika binary/source name
    [CORSIKA_REQUIRE_CERNLIB])])

AC_DEFUN([CORSIKA_ENABLE_AUGCERLONG],
[_CORSIKA_ENABLE_OPTION([AUGCERLONG],
                        [CERENKOV IACT CEFFIC INTTEST], [], [no], [cerenkov longitudinal profile], [__AUGCERLONG__], [])])

AC_DEFUN([CORSIKA_ENABLE_PRESHOWER],
[_CORSIKA_ENABLE_OPTION([PRESHOWER],
                        [INTTEST STACKIN HERWIG], [], [no], [photon preshower], [__PRESHOWER__],
    [
    PRESHOWER_CFLAGS=
    AC_CHECK_FILES([/etc/issue],[
        red_hat=`$CAT /etc/issue | grep Red | cut -d 7 -f 2 | cut -c 2`
        if test "$red_hat" = "" ; then
  	 :
        else
           if test $red_hat -le 2 ; then
	      echo 'select special RedHat 7.x'
	      PRESHOWER_CFLAGS="-D__REDHAT__"
           fi
          red_hat=
        fi
      ],[])
    ])
  AC_SUBST([PRESHOWER_CFLAGS])
])

AC_DEFUN([CORSIKA_ENABLE_MUPROD],
[_CORSIKA_ENABLE_OPTION([MUPROD],
                        [INTTEST], [],
                        [no], [MUPROD], [__MUPROD__], [])])

AC_DEFUN([CORSIKA_ENABLE_COMPACT],
[_CORSIKA_ENABLE_OPTION([COMPACT],
                        [CERENKOV IACT INTTEST ROOTOUT MACHINEINDEPENDENT ICECUBE1 ICECUBE2], [],
                        [no], [compact output], [__COMPACT__], [])])

# annitest does not exist anymore !!!!!!!!!!!!!!!
AC_DEFUN([CORSIKA_ENABLE_ANNITEST],
[_CORSIKA_ENABLE_OPTION([ANNITEST], [], [], [no], [annitest], [__TEST__], [])])

AC_DEFUN([CORSIKA_ENABLE_LPM],
[_CORSIKA_ENABLE_OPTION([LPM], [], [], [no], [LPM effect], [__LPM__], [])])

AC_DEFUN([CORSIKA_ENABLE_PARALLEL],
[_CORSIKA_ENABLE_OPTION([PARALLEL], [ANAHIST AUGERHIST MUONHIST CERENKOV IACT COMPACT INTTEST MAC PLOTSH PLOTSH2 ROOTOUT CEFFIC ICECUBE1 ICECUBE2], [], [no], [parallel computation], [__PARALLEL__], [
    PLIB2=$PLIB2"_parallel" # for suffix generation for corsika binary/source name
])])

AC_DEFUN([CORSIKA_ENABLE_PARALLELIB],
[_CORSIKA_ENABLE_OPTION([PARALLELIB], [ANAHIST AUGERHIST MUONHIST CERENKOV COMPACT INTTEST MAC PLOTSH PLOTSH2 ROOTOUT SIMTEL], [PARALLEL], [no], [parallel computation with MPI],
[__PARALLELIB__], [
                    CORSIKA_REQUIRE_MPIRUNNER_LIB
                    PLIB1="mpi_";
                    PLIB2=$PLIB2"_runner"
])])

AC_DEFUN([CORSIKA_ENABLE_STACKIN],
[_CORSIKA_ENABLE_OPTION([STACKIN], [INTTEST PRESHOWER], [], [no], [stack input], [__STACKIN__], [])])

AC_DEFUN([CORSIKA_ENABLE_EHISTORY],
[_CORSIKA_ENABLE_OPTION([EHISTORY], [INTTEST], [], [no], [particle history], [__EHISTORY__], [])])

AC_DEFUN([CORSIKA_ENABLE_TRAJECT],
[_CORSIKA_ENABLE_OPTION([TRAJECT], [INTTEST CURVED UPWARD], [], [no], [trajectory], [__TRAJECT__], [])])

AC_DEFUN([CORSIKA_ENABLE_EFIELD],
[_CORSIKA_ENABLE_OPTION([EFIELD],
                        [INTTEST], [], [no], [electrical field], [__EFIELD__],
    [
    EFIELD_CFLAGS=
    AC_CHECK_FILES([/etc/issue],[
        red_hat=`$CAT /etc/issue | grep Red | cut -d 7 -f 2 | cut -c 2`
        if test "$red_hat" = "" ; then
        :
        else
           if test $red_hat -le 2 ; then
             echo 'select special RedHat 7.x'
             EFIELD_CFLAGS="-D__REDHAT__"
           fi
          red_hat=
        fi
      ],[])
    ])
  AC_SUBST([EFIELD_CFLAGS])
])

AC_DEFUN([CORSIKA_ENABLE_RIGIDITY],
[_CORSIKA_ENABLE_OPTION([RIGIDITY], [INTTEST], [], [no], [rigidity], [__RIGIDITY__], [])])

AC_DEFUN([CORSIKA_ENABLE_HERWIG],
[_CORSIKA_ENABLE_OPTION([HERWIG], [INTTEST PRESHOWER STACKIN], [], [no] ,[herwig neutrino production (NUPRIM)], [__NUPRIM__],
    [CORSIKA_REQUIRE_HERWIG])])

AC_DEFUN([CORSIKA_ENABLE_CHARM],
[_CORSIKA_ENABLE_OPTION([CHARM], [EPOS NEXUS QGSJETII VENUS], [], [no] ,[pythia charm production], [__CHARM__],
    if test "x$CORHEMODEL" != "xDPMJET" ; then
      [CORSIKA_REQUIRE_PYTHIA]
    fi)])

AC_DEFUN([CORSIKA_ENABLE_CONEX],
[_CORSIKA_ENABLE_OPTION([CONEX], [DPMJET VENUS INTTEST CERENKOV], [CURVED SLANT UPWARD], [no] ,[CONEX cascade equation (CONEX)], [__CONEX__],
    [
    CORSIKA_REQUIRE_CONEX
    INTT=$INTT"_conex" # for suffix generation for corsika binary/source name
     ])
])

AC_DEFUN([CORSIKA_ENABLE_TAULEP],
[_CORSIKA_ENABLE_OPTION([TAULEP], [CHARM], [], [no] ,[pythia tau lepton production], [__TAULEP__],
    if test "x$CORHEMODEL" != "xDPMJET" ; then
      [CORSIKA_REQUIRE_PYTHIA]
    fi)])


AC_DEFUN([CORSIKA_ENABLE_ROOTOUT],
[_CORSIKA_ENABLE_OPTION([ROOTOUT], [COMPACT COASTUSERLIB COREAS MACHINEINDEPENDENT INCLINED], [], [no], [particle output in root file], [__COAST__],
                        [
			CORSIKA_REQUIRE_ROOT
                        CORSIKA_REQUIRE_COAST
                        COAST_INTERFACE_LIBS="-lCorsikaROOTOut"
			COAST_LIBS="-lCorsikaToROOT -lCorsikaROOT $COAST_LIBS"
                        PLIB2="_root" # for suffix generation for corsika binary/source name
                        ])])

AC_DEFUN([CORSIKA_ENABLE_MACHINEINDEPENDENT],
[_CORSIKA_ENABLE_OPTION([MACHINEINDEPENDENT], [COMPACT ROOTOUT COASTUSERLIB COREAS INCLINED], [], [no], [machine independent output], [__COAST__],
                        [
                        CORSIKA_REQUIRE_COAST
                        COAST_INTERFACE_LIBS="-lMachineIndependent"
                        ])])

AC_DEFUN([CORSIKA_ENABLE_COASTUSERLIB],
[_CORSIKA_ENABLE_OPTION([COASTUSERLIB], [COMPACT ROOTOUT COREAS MACHINEINDEPENDENT INCLINED], [SLANT], [no], [External COAST user library], [__COASTUSERLIB__],
                        [
#			CORSIKA_REQUIRE_ROOT
			CORSIKA_REQUIRE_COAST
                        CORSIKA_REQUIRE_COASTUSERLIB
                        if test x$corsika_cv_root_root = xno ; then
			COAST_LIBS="$COAST_LIBS"
                        else
			COAST_LIBS="-lCorsikaToROOT -lCorsikaROOT $COAST_LIBS"
                        fi
                        COAST_INTERFACE_LIBS="-L$COAST_USER_LIB -lCOAST"
                        PLIB2="_coast" # for suffix generation for corsika binary/source name
                        ])])

AC_DEFUN([CORSIKA_ENABLE_COREAS],
[_CORSIKA_ENABLE_OPTION([COREAS], [COMPACT ROOTOUT COASTUSERLIB MACHINEINDEPENDENT INCLINED ANAHIST AUGERHIST MUONHIST INTTEST], [SLANT], [no], [CoREAS radio simulations], [__COASTUSERLIB__],
                        [
			CORSIKA_REQUIRE_COAST
                        COAST_INTERFACE_LIBS="-lCoREAS"
                        PLIB2="_coreas" # for suffix generation for corsika binary/source name
                        ])])

AC_DEFUN([CORSIKA_ENABLE_INCLINED],
[_CORSIKA_ENABLE_OPTION([INCLINED], [COMPACT ROOTOUT COASTUSERLIB MACHINEINDEPENDENT COREAS ANAHIST AUGERHIST MUONHIST INTTEST], [], [no], [Inclined observation level], [__COASTUSERLIB__],
                        [
			CORSIKA_REQUIRE_COAST
                        COAST_INTERFACE_LIBS="-lInclinedPlane"
                        PLIB2="_inclined" # for suffix generation for corsika binary/source name
                        ])])

AC_DEFUN([CORSIKA_ENABLE_KEEPSOURCE],
[_CORSIKA_ENABLE_OPTION([KEEPSOURCE], [], [], [no], [to keep the CORSIKA compilefile], [__SAVEDCORS__], [])])

AC_DEFUN([CORSIKA_ENABLE_NOCOMPILE],
[_CORSIKA_ENABLE_OPTION([NOCOMPILE], [], [], [no], [do not compile binaries, just extract CORSIKA compilefile], [__NOCOMPILE__], [])])

AC_DEFUN([CORSIKA_ENABLE_M32],
[_CORSIKA_ENABLE_OPTION([M32], [], [], [no], [explicitly use the -m32 flag], [__M32__],
                        [
                        if $NOTUSE32 ; then
                          AC_MSG_NOTICE([M32 not compatible with your system...])
                          if $USE_GFORTRANEW ; then
                             AC_MSG_NOTICE([Output in 32 bits compatible with your system ... continue])
 			     FFLAGS="$FFLAGS -frecord-marker=4"
                          else
                             AC_MSG_NOTICE([Please use proper option in "coconut" to force compilation in 64 bits mode])
                             CORSIKA_ABORT
                          fi
                        else
                           CFLAGS="$CFLAGS -m32 -D_FILE_OFFSET_BITS=64"
 			   FFLAGS="$FFLAGS -m32"
 			   CXXFLAGS="$CXXFLAGS -m32 -D_FILE_OFFSET_BITS=64"
 			   LDFLAGS="$LDFLAGS -m32 -D_FILE_OFFSET_BITS=64"
                        fi
                        ])]
)

AC_DEFUN([CORSIKA_ENABLE_OUT32],
[_CORSIKA_ENABLE_OPTION([OUT32], [], [GFORTRAN], [no], [explicitly use the -frecord-marker=4 flag], [__OUT32__],
                        [
                        if $USE_GFORTRANEW ; then
 			   FFLAGS="$FFLAGS -frecord-marker=4"
                        else
                          AC_MSG_NOTICE([OUT32 option only for gfortran 4.2 and newer !])
                          CORSIKA_ABORT
                        fi
                        ])]
)

AC_DEFUN([CORSIKA_ENABLE_DIST],
[_CORSIKA_ENABLE_OPTION([DIST], [], [], [no], [to make dist], [__DIST__],[])])

#reset all flags to null and but all optimizations to O0
AC_DEFUN([CORSIKA_ENABLE_DEV],
[_CORSIKA_ENABLE_OPTION([DEV], [], [], [no], [to compile without optimisation and system flags], [__DEV__],
 [
  FO1=-O0
  FO3=-O0
  FO5=-O0
  FFLAGS=
  CFLAGS=
  CXXFLAGS=
  LDFLAGS=
  ])
])




#################################################################################
#    subprogram configuration
#################################################################################


# ----------------------------------------------
#                 cernlib
# ----------------------------------------------

# CORSIKA_WITH_ROOT
#   Check for presence of the ROOT package, either from command line
#   or by searching.
#
#   If found, provide substitution variables:
#
AC_DEFUN([CORSIKA_WITH_CERNLIB],
[_CORSIKA_WITH_PACKAGE([cernlib],
  [AC_HELP_STRING([--with-cernlib],
                  [use CERNLIB (default at CERN_ROOT)])],
  [$CERN_ROOT/lib $CERNLIB /cern/pro/lib],
  [libkernlib.a])
if test x$corsika_cv_root_cernlib != xno
then
   CERNLIB_FFLAGS=
   CERNLIB_LIBS="-L$corsika_cv_root_cernlib -lmathlib -lpacklib -lkernlib"
   _CORSIKA_SAVE_COMPILE_FLAGS
   FFLAGS="$FFLAGS $CERNLIB_FFLAGS"
   LIBS="$CERNLIB_LIBS"
   AC_MSG_CHECKING(whether cernlib can be used)
   AC_LANG_PUSH(Fortran 77)
   AC_LINK_IFELSE(
     [AC_LANG_PROGRAM([],[[call HLIMIT(1000)]])],
                       [
                         AC_MSG_RESULT(yes)
         #               AC_DEFINE([HAVE_CERNLIB], 1)
	               ],[
                         AC_MSG_RESULT(no)
                         corsika_cv_root_cernlib="no"
                       ])
   _CORSIKA_RESTORE_COMPILE_FLAGS
   AC_LANG_POP(Fortran 77)
   AH_TEMPLATE([HAVE_CERNLIB], [Define to 1 if CERNLIB is available on the system])
fi
])

# CORSIKA_REQUIRE_CERNLIB
#   Require that CERNLIB is installed, if not, configuration fails
#
AC_DEFUN([CORSIKA_REQUIRE_CERNLIB],
[AC_REQUIRE([CORSIKA_WITH_CERNLIB])
  if test x$corsika_cv_root_cernlib = xno; then
    CORSIKA_MISSING_REQUIRED([CERNLIB])
  else
    CORSIKA_LIBS="$CORSIKA_LIBS -all-static $CERNLIB_LIBS"
  fi
])

# ----------------------------------------------
#                 baack routines
#-----------------------------------------------
AC_DEFUN([CORSIKA_WITH_BAACK],
[
_CORSIKA_WITH_PACKAGE([BAACK],
  [AC_HELP_STRING([--with-baack],[Baack package contains multiple different modules (default $PWD/baack)])],
  [$PWD/baack],
  [LICENSE])
if test x$corsika_cv_root_baack != xno ; then
   : # placeholder
   BAACK_CPPFLAGS="$BAACK_CPPFLAGS $CXXFLAGS"
   AC_SUBST([BAACK_CPPFLAGS])
fi
])

AC_DEFUN([CORSIKA_REQUIRE_BAACK],
[
AC_REQUIRE([CORSIKA_WITH_BAACK])
  if test x$corsika_cv_root_baack = xno ; then
    CORSIKA_MISSING_REQUIRED([BAACK])
  else
      CORSIKA_SUBDIRS="baack $CORSIKA_SUBDIRS"  # include for compilation
      BAACKLIBS="$BAACKLIBS"
      CORSIKA_LIBS="$CORSIKA_LIBS -lm -lstdc++"
      AC_DEFINE([HAVE_BAACK], 1)
      corsika_require_baack="yes"
  fi
  AH_TEMPLATE([HAVE_BAACK], [Define to 1 if BAACK package can be compiled on the system])
])


# ----------------------------------------------
#                 bernlohr routines
# ----------------------------------------------

# CORSIKA_WITH_BERNLOHR
#   Checks for bernlohr code and updates all important variables
#
#   If found, provide substitution variables:
#
#     BERN_CFLAGS
#     BERN_CPPFLAGS
#     BERN_LIBS
#
AC_DEFUN([CORSIKA_WITH_BERNLOHR],
[
_CORSIKA_WITH_PACKAGE([bernlohr],
  [AC_HELP_STRING([--with-bernlohr],[use bernlohr external atmosphere routines (default $PWD/bernlohr)])],
  [$PWD/bernlohr],
  [Copyright])
if test x$corsika_cv_root_bernlohratm != xno ; then
   : # placeholder
   AC_SUBST([BERN_CFLAGS])
   AC_SUBST([BERN_CPPFLAGS])
 #
# create links with Makefile !!!! not here !!!!
#  AC_CHECK_FILE([${bindir}/atmprof1.dat],[],[echo 'Installed';
#    $as_ln_s ${prefix}/bernlohr/atmprof1.dat ${bindir}/atmprof1.dat ])
#  AC_CHECK_FILE([${bindir}/atmprof2.dat],[],[echo 'Installed';
#    $as_ln_s ${prefix}/bernlohr/atmprof2.dat ${bindir}/atmprof2.dat ])
#  AC_CHECK_FILE([${bindir}/atmprof3.dat],[],[echo 'Installed';
#    $as_ln_s ${prefix}/bernlohr/atmprof3.dat ${bindir}/atmprof3.dat ])
#  AC_CHECK_FILE([${bindir}/atmprof4.dat],[],[echo 'Installed';
#    $as_ln_s ${prefix}/bernlohr/atmprof4.dat ${bindir}/atmprof4.dat ])
#  AC_CHECK_FILE([${bindir}/atmprof5.dat],[],[echo 'Installed';
#    $as_ln_s ${prefix}/bernlohr/atmprof5.dat ${bindir}/atmprof5.dat ])
#  AC_CHECK_FILE([${bindir}/atmprof6.dat],[],[echo 'Installed';
#   $as_ln_s ${prefix}/bernlohr/atmprof6.dat ${bindir}/atmprof6.dat ])
#  AC_CHECK_FILE([${bindir}/atmprof9.dat],[],[echo 'Installed';
#    $as_ln_s ${prefix}/bernlohr/atmprof9.dat ${bindir}/atmprof9.dat ])
#
fi
])

# CORSIKA_REQUIRE_BERNLOHR
#   Require that BERNLOHR is installed, if not, configuration fails
#
AC_DEFUN([CORSIKA_REQUIRE_BERNLOHR],
[
 AC_REQUIRE([CORSIKA_WITH_BERNLOHR])
 if test x$corsika_cv_root_bernlohratm = xno ; then
   CORSIKA_MISSING_REQUIRED([BERNLOHR])
 else
   if test x$corsika_require_bernlohr != xyes ; then
      CORSIKA_SUBDIRS="bernlohr $CORSIKA_SUBDIRS"  # include for compilation
      BERNLIBS="$BERNLIBS libbern.a"
      CORSIKA_LIBS="$CORSIKA_LIBS -lbern"
      AC_DEFINE([HAVE_BERNLOHR], 1)
      corsika_require_bernlohr="yes"
   fi
 fi
 AH_TEMPLATE([HAVE_BERNLOHR], [Define to 1 if BERNLOHR can be compiled on the system])
])


# ----------------------------------------------
#                 herwig routines
# ----------------------------------------------
# CORSIKA_WITH_HERWIG
#   Checks for herwig and updates all important variables
#
#      HERWIG_DIR
#
AC_DEFUN([CORSIKA_WITH_HERWIG],
[
_CORSIKA_WITH_PACKAGE([herwig],
  [AC_HELP_STRING([--with-herwig],[use herwig routines (default $PWD/herwig)])],
  [$PWD/herwig],
  [HERWIG_C.INC])
if test x$corsika_cv_root_herwig != xno ; then
    if test "x$BERNOS" = "xaix" ; then
      AC_MSG_NOTICE([HERWIG cannot compile on AIX])
      corsika_cv_root_herwig=no
      CORSIKA_MISSING([HERWIG])
    else
      HERWIG_DIR=$corsika_cv_root_herwig
      AC_SUBST([HERWIG_DIR])
      AC_DEFINE([HAVE_HERWIG], 1)
   fi
   AH_TEMPLATE([HAVE_HERWIG], [Define to 1 if HERWIG is available on the system])
fi
])

# CORSIKA_REQUIRE_HERWIG
#   Require that HERWIG is installed, if not, configuration fails
#
AC_DEFUN([CORSIKA_REQUIRE_HERWIG],
[
AC_REQUIRE([CORSIKA_WITH_HERWIG])
  if test x$corsika_cv_root_herwig = xno ; then
    CORSIKA_MISSING_REQUIRED([HERWIG])
  else
    CORSIKA_SUBDIRS="herwig $CORSIKA_SUBDIRS"  # include for compilation
    CORSIKA_FFLAGS="$CORSIKA_FFLAGS -I\$(HERWIG_DIR)"
    CORSIKA_LIBS="$CORSIKA_LIBS -lherwig"
  fi
])

# ----------------------------------------------
#                 pythia  routines
# ----------------------------------------------
# CORSIKA_WITH_PYTHIA
#   Checks for pythia and updates all important variables
#
#      PYTHIA_DIR
#      PYTHIA_LIB
#
AC_DEFUN([CORSIKA_WITH_PYTHIA],
[
_CORSIKA_WITH_PACKAGE([pythia],
  [AC_HELP_STRING([--with-pythia],[use pythia routines (default $PWD/pythia)])],
  [$PWD/pythia],
  [commons60.f])
if test x$corsika_cv_root_pythia != xno ; then
    PYTHIA_DIR=$corsika_cv_root_pythia
    PYTHIA_FFLAGS=""
    PYTHIA_LIB="-lpythia"
    AC_SUBST([PYTHIA_DIR])
    AC_SUBST([PYTHIA_LIB])
    AC_SUBST([PYTHIA_FFLAGS])
    AC_DEFINE([HAVE_PYTHIA], 1)
    AH_TEMPLATE([HAVE_PYTHIA], [Define to 1 if PYTHIA is available on the system])
fi
])

# CORSIKA_REQUIRE_PYTHIA
#   Require that PYTHIA is installed, if not, configuration fails
#
AC_DEFUN([CORSIKA_REQUIRE_PYTHIA],
[
AC_REQUIRE([CORSIKA_WITH_PYTHIA])
if test x$corsika_cv_root_pythia = xno ; then
  CORSIKA_MISSING_REQUIRED([PYTHIA])
else
  CORSIKA_SUBDIRS="pythia $CORSIKA_SUBDIRS"  # include for compilation
  CORSIKA_FFLAGS="$CORSIKA_FFLAGS -I\$(top_srcdir)/pythia"
  CORSIKA_LIBS="$CORSIKA_LIBS -lpythia"
fi
])

# ----------------------------------------------
#                 CONEX  routines
# ----------------------------------------------
# CORSIKA_WITH_CONEX
#   Checks for conex and updates all important variables
#
#      CONEX_DIR
#      CONEX_LIB
#
AC_DEFUN([CORSIKA_WITH_CONEX],
[
_CORSIKA_WITH_PACKAGE([conex],
  [AC_HELP_STRING([--with-conex],[use conex routines (default $PWD/conex)])],
  [$PWD/conex],
  [conex_cors.F])
if test x$corsika_cv_root_conex != xno ; then
    CONEX_DIR=$corsika_cv_root_conex
#    CONEX_FFLAGS="-O0 -I\$(srcdir)"
    CONEX_FFLAGS=
    CONEX_LIB="-lconex"
    CONEX_CPPFLAGS="-D__CXSUB__ -D__CXCORSIKA__ -D__MC3D__ -D__CXDEBUG__ -D__SORT_FOR_ENERGY__"
    if test x$corsika_cv_enable_CURVED = xyes ; then
       CONEX_CPPFLAGS="$CONEX_CPPFLAGS"
    else
       CONEX_CPPFLAGS="$CONEX_CPPFLAGS -D__SAVEMEMO__"
    fi
    case x$CORHEMODEL in
       xQGSJET01 ) CONEX_CPPFLAGS="$CONEX_CPPFLAGS -D__QGSJET__"
                   CONEX_PARAM=conex_qgsjet ;;

       xSIBYLL ) CONEX_CPPFLAGS="$CONEX_CPPFLAGS -D__SIBYLL21__"
                   CONEX_PARAM=conex_sibyll ;;

       xQGSJETII ) CONEX_CPPFLAGS="$CONEX_CPPFLAGS -D__QGSJETII__"
                   CONEX_PARAM=conex_qgsjetII ;;
       xNEXUS    ) CONEX_CPPFLAGS="$CONEX_CPPFLAGS -D__NEXUS__"
                   CONEX_FFLAGS="$CONEX_FFLAGS -I\$(top_srcdir)/nexus"
                   CONEX_PARAM=conex_nexus ;;
       xEPOS     ) CONEX_CPPFLAGS="$CONEX_CPPFLAGS -D__EPOS__"
                   CONEX_FFLAGS="$CONEX_FFLAGS -I\$(top_srcdir)/epos"
                   CONEX_PARAM=conex_epos ;;
       *         ) if test "x$corsika_cv_enable_dist" = "xno" ; then
		      AC_MSG_NOTICE([Hadronic model $CORHEMODEL cannot be used with CONEX])
                      CORSIKA_ABORT
		   fi;;
    esac
    case x$CORLEMODEL in
       xGHEISHA ) CONEX_CPPFLAGS="$CONEX_CPPFLAGS -D__GHEISHA__"
                   CONEX_PARAM="$CONEX_PARAM"_gheisha ;;

       xURQMD ) CONEX_CPPFLAGS="$CONEX_CPPFLAGS -D__URQMD__"
                CONEX_FFLAGS="$CONEX_FFLAGS -I\$(top_srcdir)/urqmd"
                   CONEX_PARAM="$CONEX_PARAM"_urqmd ;;

       xFLUKA ) CONEX_CPPFLAGS="$CONEX_CPPFLAGS -D__FLUKA__"
                CONEX_FFLAGS="$CONEX_FFLAGS -I$FLUPRO/flukapro"
                   CONEX_PARAM="$CONEX_PARAM"_fluka ;;

       *         ) if test "x$corsika_cv_enable_dist" = "xno" ; then
		      AC_MSG_NOTICE([Hadronic model $CORLEMODEL cannot be used with CONEX])
                      CORSIKA_ABORT
		   fi;;
    esac
    if test x$corsika_cv_enable_PRESHOWER = xyes; then
      CONEX_CPPFLAGS="$CONEX_CPPFLAGS -D__PRESHOW__"
    fi
# __COAST__ defined CORSIKA_REQUIRE_COAST only for CORSIKA, do it here for CONEX
    if test x$corsika_cv_enable_COASTUSERLIB = xyes ; then
      CONEX_CPPFLAGS="$CONEX_CPPFLAGS -D__COAST__"
    fi
    echo $CONEX_CPPFLAGS > $CONEX_DIR/cxcomp.cache.tmp
    AC_CHECK_FILE([$CONEX_DIR/cxcomp.cache],[
                   if test "x`diff $CONEX_DIR/cxcomp.cache.tmp $CONEX_DIR/cxcomp.cache`" != x ; then rm -f $CONEX_DIR/cxcompfile.f ; fi
                ],[
                   rm -f $CONEX_DIR/cxcompfile.f])
    mv $CONEX_DIR/cxcomp.cache.tmp $CONEX_DIR/cxcomp.cache
    CXCOMP_FILE="\$(CPP) \$(DEFS) \$(DEFAULT_INCLUDES) \$(INCLUDES) \$(AM_CPPFLAGS) \$(PRECOMPILER_FLAGS) \$(srcdir)/conex_cors.F > cxcompfile.f"
    AC_SUBST([CONEX_DIR])
    AC_SUBST([CONEX_LIB])
    AC_SUBST([CONEX_FFLAGS])
    AC_SUBST([CONEX_PARAM])
    AC_SUBST([CXCOMP_FILE])
    AC_SUBST([CONEX_CPPFLAGS])
    AC_DEFINE([HAVE_CONEX], 1)
    AH_TEMPLATE([HAVE_CONEX], [Define to 1 if CONEX is available on the system])
fi
])

# CORSIKA_REQUIRE_CONEX
#   Require that CONEX is installed, if not, configuration fails
#
AC_DEFUN([CORSIKA_REQUIRE_CONEX],
[
AC_REQUIRE([CORSIKA_WITH_CONEX])
if test x$corsika_cv_root_conex = xno ; then
  CORSIKA_MISSING_REQUIRED([CONEX])
else
  CORSIKA_SUBDIRS="conex $CORSIKA_SUBDIRS"  # include for compilation
  CORSIKA_FFLAGS="$CORSIKA_FFLAGS -I\$(top_srcdir)/conex"
  PRECOMPILER_FLAGS=" -I\$(top_srcdir)/conex $PRECOMPILER_FLAGS $CONEX_CPPFLAGS"
fi
])


# ----------------------------------------------
#                 PLOTSH2
# ----------------------------------------------
AC_DEFUN([AC_CHECK_MAP2PNG],[
	     $1=false
	     AC_CHECK_FILE(${prefix}/run/map2png,[
	        ${prefix}/run/map2png > test_map2png_tmp.txt],[])
	     if test -f test_map2png_tmp.txt ; then
	       AC_MSG_CHECKING(whether map2png runs on your system)
	       test_string=`cat test_map2png_tmp.txt | grep "Usage: map2png" | cut -c9-15`
	       if test "$test_string" = "map2png" ; then
	        $1=true
		AC_MSG_RESULT(yes)
	       else
		$1=false
		AC_MSG_RESULT(no)
	       fi
	       rm -f test_map2png_tmp.txt
	     fi
])


# ----------------------------------------------
#                 ROOT
# ----------------------------------------------


# CORSIKA_WITH_ROOT
#   Check for presence of the ROOT package, either from command line
#   or by searching.
#
#   If found, provide substitution variables:
#      ROOTSYS
#      ROOT_LIBS
#      ROOT_GLIBS
#      ROOT_CXXFLAGS
#      ROOTCINT
#
AC_DEFUN([CORSIKA_WITH_ROOT],
[_CORSIKA_WITH_PACKAGE([root],
  [AC_HELP_STRING([--with-root],
                  [use ROOT (default ROOTSYS=ARG or use ROOTSYS or search)])],
  [$ROOTSYS /usr /usr/local /opt/local /usr/local/root /usr/local/root/pro/root $ROOTSYS],
  [bin/root-config])
if test x$corsika_cv_root_root != xno ; then
  ROOTSYS=$corsika_cv_root_root
  export ROOTSYS
  AC_SUBST(ROOTSYS)
  ROOT_LIBS=`$ROOTSYS/bin/root-config --libs`
  AC_SUBST(ROOT_LIBS)
  ROOT_GLIBS=`$ROOTSYS/bin/root-config --glibs`
  AC_SUBST(ROOT_GLIBS)
  ROOT_LIBDIR=`$ROOTSYS/bin/root-config --libdir 2>/dev/null` || \
  ROOT_LIBDIR=`$ROOTSYS/bin/root-config --libs | $AWK '{print substr($[]1, 3)}'`
  AC_SUBST(ROOT_LIBDIR)
  ROOT_CXXFLAGS=`$ROOTSYS/bin/root-config --cflags`
  AC_SUBST(ROOT_CXXFLAGS)
  ROOTCINT=$ROOTSYS/bin/rootcint
  AC_SUBST(ROOTCINT)

  AC_LANG_PUSH([C++])
  _CORSIKA_SAVE_COMPILE_FLAGS
  CXXFLAGS="$CXXFLAGS $ROOT_CXXFLAGS"
  LDFLAGS="$LDFLAGS $ROOT_LIBS"
  AC_MSG_CHECKING([whether root can be used])
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([#include <TCanvas.h>],[TCanvas* c = new TCanvas("test");])],
      [
        AC_MSG_RESULT([yes])
        AC_DEFINE([HAVE_ROOT], 1)
      ],[
        AC_MSG_RESULT([no - not binary compatible to selection])
        corsika_cv_root_root="no"
      ])
  _CORSIKA_RESTORE_COMPILE_FLAGS
  AC_LANG_POP([C++])
  AH_TEMPLATE([HAVE_ROOT], [Define to 1 if ROOT is available on the system])
#  AM_CONDITIONAL([HAVE_ROOT], test x$corsika_cv_root_root != xno)
fi
])



# CORSIKA_REQUIRE_ROOT([version])
#   Require that ROOT is installed, if not, configuration fails
#   If version argument present, require that ROOT as at least
#   this version.
#
AC_DEFUN([CORSIKA_REQUIRE_ROOT],
[AC_REQUIRE([CORSIKA_WITH_ROOT])
if test x$corsika_cv_root_root = xno ; then
  CORSIKA_MISSING_REQUIRED([ROOT])
fi
m4_if($1,[],[dnl],
  [AC_LANG_PUSH([C++])
   _CORSIKA_SAVE_COMPILE_FLAGS
  CPPFLAGS="$CPPFLAGS $ROOT_CPPFLAGS"
  LDFLAGS="$LDFLAGS $ROOT_LIBS"
  LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$ROOT_LIBDIR"
  export LD_LIBRARY_PATH
  AC_RUN_IFELSE([#include <RVersion.h>
                 int main ()
                 {
                   if (ROOT_VERSION_CODE >=
                       ROOT_VERSION(_CORSIKA_NORMALISE_VERSION_3($1))) return 0;
                   else return 1;
                 }], [], [AC_MSG_NOTICE([ROOT > $1 required], 1)
                          CORSIKA_MISSING_REQUIRED([ROOT])])
  _CORSIKA_RESTORE_COMPILE_FLAGS
  AC_LANG_POP([C++])
  ])
])



# ----------------------------------------------
#                 COAST
# ----------------------------------------------

# CORSIKA_WITH_COAST
#
#   Set the substitution variables:
#      COAST_DIR
#      COAST_LIBS
#
AC_DEFUN([CORSIKA_WITH_COAST],
[ COAST_DIR=coast # probalby useless     $corsika_cv_root_coast
  AC_SUBST([COAST_DIR])
dnl  if test x$ax_cv_boost_program_options != xno ; then
    if test x$HAVE_BOOST_PROGRAM_OPTIONS != x; then
     AC_MSG_NOTICE([Without boost libraries, the COAST example executables will not be built])
     AC_MSG_NOTICE([Install boost program_options if you need them.])
    fi
    AC_SUBST([BOOST_PROGRAM_OPTIONS_LIB])
dnl    AC_MSG_NOTICE([Using: $BOOST_PROGRAM_OPTIONS_LIB])
])

# CORSIKA_REQUIRE_COAST
#
#   Require that COAST is installed, if not, configuration fails
#
AC_DEFUN([CORSIKA_REQUIRE_COAST],
[AC_REQUIRE([CORSIKA_WITH_COAST])
    AC_DEFINE([__COAST__],[1],[ENABLE COAST INTERFACE])
    COAST_LIBS="-lCorsikaIntern -lCorsikaFileIO -lstdc++"
    AC_SUBST([COAST_LIBS])
    CORSIKA_SUBDIRS="coast $CORSIKA_SUBDIRS"  # include for compilation
])



# ----------------------------------------------
#                 COASTUSERLIB external path
# ----------------------------------------------


# CORSIKA_WITH_COASTUSERLIB
#   Check for presence of the external COAST user library from command line
#   or by searching
#
#   If found, provide substitution variables:
#      COAST_USER_LIB
#
AC_DEFUN([CORSIKA_WITH_COASTUSERLIB],
[_CORSIKA_WITH_PACKAGE([COASTUSERLIB],
  [AC_HELP_STRING([--with-COASTUSERLIB],
                  [use external COAST user-library interface ])],
  [$COAST_USER_LIB],
  [libCOAST.so])
if test x$corsika_cv_root_COASTUSERLIB != xno ; then
  COAST_USER_LIB="$corsika_cv_root_COASTUSERLIB"
  # does not work in general, since it want to build an executable
#  AC_LANG_PUSH([C++])
#  _CORSIKA_SAVE_COMPILE_FLAGS
#  CXXFLAGS="$CXXFLAGS $ROOT_CXXFLAGS -I$COAST_USER_LIB"
#  LDFLAGS="$LDFLAGS $ROOT_LIBS -L$COAST_USER_LIB"
#  AC_MSG_CHECKING([whether user coast lib can be used])
#  AC_CHECK_LIB([COAST], [cloda_],
#        [
#          AC_MSG_RESULT([yes])
#          AC_DEFINE([HAVE_COASTUSERLIB], 1)
#        ],[
#          AC_MSG_RESULT([no - not binary compatible to selection])
#          corsika_cv_root_COASTUSERLIB="no"
#        ])
#  _CORSIKA_RESTORE_COMPILE_FLAGS
#  AC_LANG_POP([C++])
  AC_SUBST([COAST_USER_LIB])
  CORSIKA_CACHE([COAST_LIB], $COAST_USER_LIB)
  AH_TEMPLATE([HAVE_COASTUSERLIB], [Define to 1 if external COAST user library is available on the system])
fi
])

# CORSIKA_REQUIRE_COASTUSERLIB
#
#   Require that external COAST user lib is installed, if not, configuration fails
#
AC_DEFUN([CORSIKA_REQUIRE_COASTUSERLIB],
[AC_REQUIRE([CORSIKA_WITH_COASTUSERLIB])
if test x$corsika_cv_root_COASTUSERLIB = xno ; then
  CORSIKA_MISSING_REQUIRED([COASTUSERLIB])
else
  CORSIKA_REQUIRE_COAST
fi
])

# ----------------------------------------------
#                 MPIRUNNER
# ----------------------------------------------

# CORSIKA_WITH_MPIRUNNER_LIB
#   Check for presence of the Mpirunner code and system environment
#
#   If found, provide substitution variables:
#      MPIRUNNER_LIB
#
AC_DEFUN([CORSIKA_WITH_MPIRUNNER_LIB],
[_CORSIKA_WITH_PACKAGE([mpirunner_lib],
  [AC_HELP_STRING([--with-mpirunner-lib],
                  [use Mpirunner lib])],
  [$PWD/src/parallel],
  [mpi_runner.c])
  if test x$corsika_cv_root_mpirunner_lib != xno ; then
    AX_MPI([
    MPIRUNNER_LIBS="$MPILIBS -lm -L$srcdir/parallel -L$srcdir -lcorsika $CORSIKA_LIBS $CONEX_LIB $MODEL_LIBS $COAST_INTERFACE_LIBS $COAST_LIBS $FLIBS"
    CC="$MPICC"
    CXX=mpic++
    CCLD=mpic++
    AC_SUBST([CCLD])
    CORSIKA_CACHE([MPIRUNNER_LIB], $MPIRUNNER_LIB)

    ],[
      corsika_cv_root_mpirunner_lib="no"
      CCLD="$CC"
      AC_SUBST([CCLD])
    ])
  else
      CCLD="$CC"
      AC_SUBST([CCLD])
  fi
  AH_TEMPLATE([HAVE_MPIRUNNER_LIB], [Define to 1 if Mpirunner is available on the system])
])

# CORSIKA_REQUIRE_MPIRUNNER
#   Require that Mpirunner user lib is installed, if not, configuration fails
#
AC_DEFUN([CORSIKA_REQUIRE_MPIRUNNER_LIB],
[AC_REQUIRE([CORSIKA_WITH_MPIRUNNER_LIB])
if test x$corsika_cv_root_mpirunner_lib = xno ; then
  CORSIKA_MISSING_REQUIRED([MPIRUNNER_LIB])
fi
])




# ----------------------------------------------
#                 FLUKA
# ----------------------------------------------

# CORSIKA_WITH_FLUKA
#   Check for presence of the FLUKA package
#
#   If found, provide substitution variables:
#      FLUPRO
#      FLUKA_FFLAGS
#      FLUKA_LIB
#
AC_DEFUN([CORSIKA_WITH_FLUKA],
[_CORSIKA_WITH_PACKAGE([fluka],
  [AC_HELP_STRING([--with-fluka],
                  [use fluka (default at FLUPRO)])],
  [$FLUPRO],
  ["flukapro/(FLKCMP)"])
if test x$corsika_cv_root_fluka != xno ; then
   FLUPRO=$corsika_cv_root_fluka
   FLUKA_FFLAGS="$FLUKA_FFLAGS -I$FLUPRO/flukapro"
   FLUKA_LIBS="$FLUKA_LIBS -L$FLUPRO -lflukahp"
   _CORSIKA_SAVE_COMPILE_FLAGS
   FFLAGS="$FFLAGS $FLUKA_FFLAGS"
   LIBS="$LIBS $FLUKA_LIBS"
   AC_MSG_CHECKING(whether FLUKA can be used)
   AC_LANG_PUSH(Fortran 77)
   AC_LINK_IFELSE(
      [AC_LANG_PROGRAM([],[[
                            include '(IOUNIT)'
                            dimension test(LUNIN)
                            call ZEROIN
                        ]])],
                        [
                          AC_MSG_RESULT(yes)
                          AC_DEFINE([__LINUX__],[1],[FLUKA 2011 for Linux])
                        ],
                        [
                          AC_MSG_RESULT(no)
                          corsika_cv_root_fluka="no"
                        ])
   _CORSIKA_RESTORE_COMPILE_FLAGS
   AC_LANG_POP(Fortran 77)
   AC_SUBST([FLUKA_FFLAGS])
   AC_SUBST([FLUKA_LIBS])
   AC_SUBST([FLUPRO])
   AC_DEFINE([HAVE_FLUKA], 1)
   AH_TEMPLATE([HAVE_FLUKA], [Define to 1 if FLUKA is available on the system])
fi
])


# CORSIKA_REQUIRE_FLUKA
#   Require that FLUKA is installed, if not, configuration fails
#
AC_DEFUN([CORSIKA_REQUIRE_FLUKA],
[AC_REQUIRE([CORSIKA_WITH_FLUKA])
if test x$corsika_cv_root_fluka = xno ; then
  CORSIKA_MISSING_REQUIRED([FLUKA])
fi
])





# ----------------------------------------------
#              finish
# ----------------------------------------------

# CORSIKA_FINISH
#   Final code after all tests for external packages
#   Aborts execution if we are missing any required packages or options.
#
AC_DEFUN([CORSIKA_FINISH],
[
#cat >auger-missing-packages <<EOF
##! /bin/sh
#if test x\$[]1 = x--required
#then
#  echo $corsika_missing_required
#else
#  echo $corsika_missing
#fi
#EOF
#chmod a+x auger-missing-packages

echo '- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -'

if test "x$corsika_missing" != x ; then
  AC_MSG_NOTICE([Optional packages not found or deactivated on this system: $corsika_missing])
fi
if test "x$corsika_option_conflict" != x ; then
  AC_MSG_NOTICE([Options with conflict: $corsika_option_conflict])
  CORSIKA_ABORT
fi
if test "x$corsika_missing_required" != x ; then
  AC_MSG_NOTICE([Missing required packages: $corsika_missing_required])
  CORSIKA_ABORT
fi
if test "x$corsika_missing_required_options" != x ; then
  AC_MSG_NOTICE([Missing required options: $corsika_missing_required_options])
  AC_MSG_NOTICE([Please use the \"coconut\" installer script])
  CORSIKA_ABORT
fi
])




###########################################################################
# ----------------------------------------------
#              general purpose macros
# ----------------------------------------------

AC_DEFUN([CORSIKA_ABORT],
[
echo '- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -'
AC_MSG_ERROR([Aborting configuration])
])


# CORSIKA_ENABLE_DOCUMENTATION
#   Enable or disable the generation of documentation
#
AC_DEFUN([CORSIKA_ENABLE_DOCUMENTATION],
[AC_ARG_ENABLE([documentation],
               AC_HELP_STRING([--enable-documentation],
                              [generate documentation (default=yes)]),
               [corsika_cv_enable_documentation=$enableval],
               [corsika_cv_enable_documentation='yes'])
AC_CACHE_CHECK([whether to generate documentation],
               [corsika_cv_enable_documentation],
               [corsika_cv_enable_documentation=yes])
AM_CONDITIONAL([DOCUMENTATION], test x$corsika_cv_enable_documentation = xyes)
])

# does not really work because -g is added automatically by configure !
AC_DEFUN([CORSIKA_ENABLE_DEBUG],
[AC_ARG_ENABLE([debug],
               AC_HELP_STRING([--enable-debug],
                              [enables debug symbols (default=yes)]),
               [corsika_cv_enable_debug=$enableval],
               [corsika_cv_enable_debug='yes'])
AC_CACHE_CHECK([whether to generate debug],
               [corsika_cv_enable_debug],
               [corsika_cv_enable_debug=yes])

 if test "x$corsika_cv_enable_debug" = xyes ; then
         if ! echo $CXXFLAGS | grep -q -- -Wall ; then
         CXXFLAGS="$CXXFLAGS -Wall"
         fi
         CFLAGS="$CFLAGS -g"
         FFLAGS="$FFLAGS -g"
 fi
#AM_CONDITIONAL([DOCUMENTATION], test x$corsika_cv_enable_documentation = xyes)
])


# CORSIKA_CHECK_DARWIN
# Check for Darwin
#
AC_DEFUN([CORSIKA_CHECK_DARWIN],
[
 corsika_uname_system=`uname -s`
 if test "x$corsika_uname_system" = xDarwin ; then
   corsika_dylib_extension=dylib
   echo Configuring for Darwin
 else
   corsika_dylib_extension=so
 fi
])


# CORSIKA_INIT
#   Initial code for extra scripts to generate
#
AC_DEFUN([CORSIKA_INIT],
[
  CORSIKA_SUBDIRS="src ."  # subdir list for compilation
  CORSIKA_LIBS="-L$libdir"
  CORSIKA_LIB_DIR="$libdir"
#  CORSIKA_LIBS=
  MODEL_LIBS=
  CONEX_LIB=
  FO1=-O1
  FO3=-O3
  FO5=-05
  corsika_missing=
  corsika_missing_required=
  corsika_missing_required_options=
  corsika_all_options_conflicts=
  corsika_option_conflict=
  corsika_required_options=
  corsika_all_options=
])


# CORSIKA_MISSING
#   Deal with missing pacakges
#
AC_DEFUN([CORSIKA_MISSING],
[corsika_missing="$corsika_missing $1"
])

# CORSIKA_MISSING_REQUIRED
#   Deal with missing pacakges
#
AC_DEFUN([CORSIKA_MISSING_REQUIRED],
[corsika_missing_required="$corsika_missing_required $1"
])

# CORSIKA_MISSING_REQUIRED_OPTION
#   Deal with missing required options
#
AC_DEFUN([CORSIKA_MISSING_REQUIRED_OPTION],
[corsika_missing_required_options="$corsika_missing_required_options $1"
])


# CORSIKA_CHECK_MISSING_REQUIRED_PACKAGES
#   Code to test if we are missing any required packages.
#   Calls CORSIKA_FINISH if this is the case.
#
AC_DEFUN([CORSIKA_CHECK_MISSING_REQUIRED_PACKAGES],
[
if test "x$corsika_missing_required" != "x" ; then
  CORSIKA_FINISH
fi
if test "x$corsika_missing_required_options" != "x" ; then
  CORSIKA_FINISH
fi
])


# CORSIKA_CHECK_CONFLICTS
#   Code to test if there were any configuration conflicts
#   Calls CORSIKA_FINISH if this is the case.
#
AC_DEFUN([CORSIKA_CHECK_CONFLICTS],
[
if test "x$corsika_option_conflict" != "x" ; then
  CORSIKA_FINISH
fi
])


# CORSIKA_CACHE(option, value)
#   Adds option as "__CACHE_option__" to config.h
#   this enables coconut to read back the configuration.
#
AC_DEFUN([CORSIKA_CACHE],
[
  AC_DEFINE([__CACHE_$1__], $2)
  AH_TEMPLATE([__CACHE_$1__], [coconut configration cache for option $1])
])


# CORSIKA_APPEND_OPTION(option)
#   Adds option to global list of used options
#
AC_DEFUN([CORSIKA_APPEND_OPTION],
[corsika_all_options="$corsika_all_options $1"])


# CORSIKA_APPEND_CONFLICTS(options)
#   Adds option to list of conflict options
#
AC_DEFUN([CORSIKA_APPEND_CONFLICTS],
[corsika_all_options_conflicts="$corsika_all_options_conflicts $1"])


# CORSIKA_APPEND_REQUIRES(options)
#   Adds option to list of required options
#
AC_DEFUN([CORSIKA_APPEND_REQUIRES],
[corsika_required_options="$corsika_required_options $1"])


# CORSIKA_REMOVE_OPTION(options)
#   Removed option from total list of used options
#
AC_DEFUN([CORSIKA_REMOVE_OPTION],
[for option in $1
 do
   corsika_all_options=`echo $corsika_all_options | sed -e s/$option//g`
 done
])


# CORSIKA_REMOVE_CONFLICTS(options)
#   Removed option from list of conflict options
#
AC_DEFUN([CORSIKA_REMOVE_CONFLICTS],
[for option in $1
 do
   corsika_all_options_conflicts=`echo $corsika_all_options_conflicts | sed -e s/$option//g`
 done
])

# CORSIKA_REMOVE_REQUIRES(options)
#   Removed option from list of required options
#
AC_DEFUN([CORSIKA_REMOVE_REQUIRES],
[for option in $1
 do
   corsika_required_options=`echo $corsika_required_options | sed -e s/$option//g`
 done
])

# CORSIKA_CHECK_CONFLICT(option)
#   Code to test if there is a conflic with option $1
#
AC_DEFUN([CORSIKA_CHECK_CONFLICT],
[for optionConflict in $corsika_all_options_conflicts
 do
   if test "x$1" = "x$optionConflict" ; then
     corsika_option_conflict="$corsika_option_conflict $1"
   fi
 done
])

# CORSIKA_CHECK_REQUIRED
#   Code to test if all required options are used
#
AC_DEFUN([CORSIKA_CHECK_REQUIRED],
[for optionRequired in $corsika_required_options
 do
   test_required=""
   for option in $corsika_all_options
   do
     if test "x$option" = "x$optionRequired" ; then
       test_required="$test_required $option"
     fi
   done
   if test "x$test_required" = "x" ; then
     CORSIKA_MISSING_REQUIRED_OPTION([$optionRequired])
   fi
 done
])

# _CORSIKA_WITH_PACKAGE(name, help, path, file)
#   Wrapper for AC_ARG_WITH and AC_CACHE_CHECK
#   Provides a --with-name option with help.
#   If package location not given, try to find `file' in `path' and
#   keep the *last* match
#
AC_DEFUN([_CORSIKA_WITH_PACKAGE],
[
AC_ARG_WITH([$1], [$2], [_corsika_with_$1=$withval])
AC_CACHE_CHECK([for $1],
               [corsika_cv_root_$1],
               [if test x$_corsika_with_$1 = x
                then
                  corsika_cv_root_$1=no
                else
                  if test x$_corsika_with_$1 = xyes
                     then
                     for p in $3
                     do
                        test -f $p/$4 && corsika_cv_root_$1=$p
                     done
                     test x$corsika_cv_root_$1 = x && corsika_cv_root_$1=no
                   else
                     corsika_cv_root_$1=$_corsika_with_$1
                   fi
                fi
               ])
])

# _CORSIKA_ENABLE_OPTION(name, conflicts, requires, default, help, define, if-enabled)
#   Wrapper for CORSIKA options
#
#             name:  name of option (--enable-<name>)
#        conflicts:  list of option name conflicting with this one
#         requires:  list of option absolutely required for this option
#          default:  default value: yes|no
#             help:  help string
#           define:  CORSIKA pre-compiler define
#         if-found:  optional code to be executed, if option is used

AC_DEFUN([_CORSIKA_ENABLE_OPTION],
[
  AC_ARG_ENABLE([$1],
                [AC_HELP_STRING([--enable-$1],
                                [$5 (default=$4)])],
                [corsika_cv_enable_$1=$enableval],
                [corsika_cv_enable_$1=$4])

  AC_CACHE_CHECK([$5],
                 [corsika_cv_enable_$1],
                 [corsika_cv_enable_$1=$4])

  if test "x$corsika_cv_enable_$1" = "xyes" ; then

    CORSIKA_APPEND_OPTION([$1])
    CORSIKA_CHECK_CONFLICT([$1])
    CORSIKA_APPEND_CONFLICTS([$2])
    CORSIKA_APPEND_REQUIRES([$3])
    CORSIKA_CACHE([$1])

    AC_DEFINE([$6], [1])
    $7

  else

    CORSIKA_REMOVE_OPTION([$1])
    CORSIKA_REMOVE_CONFLICTS([$2])
    CORSIKA_REMOVE_REQUIRES([$3])

  fi

  AH_TEMPLATE([$6], [$5])
  AM_CONDITIONAL([USE_$1], test "x$corsika_cv_enable_$1" = "xyes")
])



##########################################################
#
#     M4 macro section
#


# _CORSIKA_BASENAME
#  M4 analogue to the basename shell utility
#
m4_define([_CORSIKA_BASENAME], [m4_bregexp($1, [\(.*/\|\)\(.+\)], [\2])])

# _CORSIKA_NORMALISE_VERSION_3
#   Convert dot-separated version number into a
#   comma-separated list for individual processing
#   Up to 3 components accepted, missing componented
#   replaced by 0.
#
m4_define([_CORSIKA_NORMALISE_VERSION_3],
          [_CORSIKA_NORMALISE_VERSION_AUX_3(m4_bpatsubst($1, [\.], [,]))])

m4_define([_CORSIKA_NORMALISE_VERSION_AUX_3],
  [ifelse($#, 3, [$1, $2, $3], $#, 2, [$1, $2, 0], $#, 1, [$1, 0, 0], [AC_FATAL([Maximal 3 components in version expected - got $*])])])

# _CORSIKA_SAVE_COMPILE_FLAGS
#   Save flags for compilation so we can modify them for test compilation
#   NB: This macro cannot be nested
#
m4_define([_CORSIKA_SAVE_COMPILE_FLAGS],
[_corsika_save_CXXFLAGS="$CXXFLAGS"
_corsika_save_CPPFLAGS="$CPPFLAGS"
_corsika_save_FFLAGS="$FFLAGS"
_corsika_save_CFLAGS="$CFLAGS"
_corsika_save_LIBS="$LIBS"
_corsika_save_LDFLAGS="$LDFLAGS"
_corsika_save_LD_LIBRARY_PATH="$LD_LIBRARY_PATH"
])

# _CORSIKA_SAVE_RESTORE_FLAGS
#   Restore flags after compilation. so we can modify them for test compilation
#
m4_define([_CORSIKA_RESTORE_COMPILE_FLAGS],
[CXXFLAGS="$_corsika_save_CXXFLAGS"
CPPFLAGS="$_corsika_save_CPPFLAGS"
FFLAGS="$_corsika_save_FFLAGS"
CFLAGS="$_corsika_save_CFLAGS"
LIBS="$_corsika_save_LIBS"
LDFLAGS="$_corsika_save_LDFLAGS"
LD_LIBRARY_PATH="$_corsika_save_LD_LIBRARY_PATH"
])
