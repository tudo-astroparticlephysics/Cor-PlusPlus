## Makefile.am -- Process this file with automake to get Makefile.in

srcdir = @srcdir@
VPATH = @srcdir@

if USE_NOCOMPILE
nocompile: corsikacompilefile.f
endif
bin_PROGRAMS = @MAP2PNG@ @PLOTTRACKS@

if USE_PARALLELIB
lib_LIBRARIES = libcorsika.a
bin_PROGRAMS += mpi_runner

else

if USE_CORPP
lib_LIBRARIES = libcorsika.a

else
bin_PROGRAMS += corsika

endif
endif
# because of problem with AIX
## _SOURCES = corsika.F

######################################
# CORSIKA main program
#

# pre-compilation step
corsikacompilefile.f : corsika.F $(top_srcdir)/include/config.h # dependency checking
	$(CPP) $(CPPFLAGS) @PRECOMPILER_FLAGS@ corsika.F > $@
if USE_PARALLELIB
  @ sed -i "/\# / s/\# /\* /" corsikacompilefile.f ;
endif
if USE_CORPP
  #@ sed -i "/\# / s/\# /\* /" corsikacompilefile.f ;
endif

if USE_KEEPSOURCE
	@ @CAT_CMD@ corsikacompilefile.f > @CORSIKA_NAME@_compilefile.f ; \
	echo " " ;  \
	echo "--> src/CORSIKA.F preprocessed and saved as src/@CORSIKA_NAME@_compilefile.f" ; \
	echo " " ;
endif

#compilation as library

AM_CFLAGS =

if USE_PARALLELIB
nodist_libcorsika_a_SOURCES = corsikacompilefile.f tobuf.c
libcorsika_a_FFLAGS = @CORSIKA_FFLAGS@

mpi_runner_LDADD = @MPIRUNNER_LIBS@

if USE_TRAPFE
nodist_libcorsika_a_SOURCES += trapfpe.c
endif

# choose HE model code
if USE_QGSJET01
nodist_libcorsika_a_SOURCES += qgsjet01d.f
endif
if USE_QGSJETII
nodist_libcorsika_a_SOURCES += qgsjet-II-04.f
endif
if USE_SIBYLL
nodist_libcorsika_a_SOURCES += sibyll2.3c.f
endif
if USE_VENUS
nodist_libcorsika_a_SOURCES += venus.f
endif

# choose LE model code
if USE_GHEISHA
nodist_libcorsika_a_SOURCES += gheisha_2002d.f
endif

if USE_EFIELD
nodist_libcorsika_a_SOURCES += elfield.c
#elfield_o_FFLAGS =
AM_CFLAGS += @EFIELD_CFLAGS@
endif

if USE_PRESHOWER
nodist_libcorsika_a_SOURCES += preshower2.0-rev/preshw.c preshower2.0-rev/utils.c preshower2.0-rev/veto.c
#preshw_o_FFLAGS =
AM_CFLAGS += @PRESHOWER_CFLAGS@
endif

if USE_TIMERC
nodist_libcorsika_a_SOURCES += timerc.c
endif

else
# CORPP Library compilation
if USE_CORPP
nodist_libcorsika_a_SOURCES = corsikacompilefile.f tobuf.c
libcorsika_a_FFLAGS = @CORSIKA_FFLAGS@

if USE_TRAPFE
nodist_libcorsika_a_SOURCES += trapfpe.c
endif

# choose HE model code
if USE_QGSJET01
nodist_libcorsika_a_SOURCES += qgsjet01d.f
endif
if USE_QGSJETII
nodist_libcorsika_a_SOURCES += qgsjet-II-04.f
endif
if USE_SIBYLL
nodist_libcorsika_a_SOURCES += sibyll2.3c.f
endif
if USE_VENUS
nodist_libcorsika_a_SOURCES += venus.f
endif

# choose LE model code
if USE_GHEISHA
nodist_libcorsika_a_SOURCES += gheisha_2002d.f
endif

if USE_EFIELD
nodist_libcorsika_a_SOURCES += elfield.c
#elfield_o_FFLAGS =
AM_CFLAGS += @EFIELD_CFLAGS@
endif

if USE_PRESHOWER
nodist_libcorsika_a_SOURCES += preshower2.0-rev/preshw.c preshower2.0-rev/utils.c preshower2.0-rev/veto.c
#preshw_o_FFLAGS =
AM_CFLAGS += @PRESHOWER_CFLAGS@
endif

if USE_TIMERC
nodist_libcorsika_a_SOURCES += timerc.c
endif

#normal compilation
else

nodist_corsika_SOURCES = corsikacompilefile.f tobuf.c
corsika_FFLAGS = @CORSIKA_FFLAGS@
corsika_LDADD = @CORSIKA_LIBS@ @CONEX_LIB@ @MODEL_LIBS@ @COAST_LIBS@ @ROOT_LIBS@

if USE_ROOTOUT # This is of general interest for all dynamic linking with CORSIKA
corsika_LDFLAGS = -Wl,--rpath-link=@CORSIKA_LIB_DIR@
endif
#
# path to libCOAST hardcoded for CoREAS but not for COAST_USER_LIB
#
if USE_COREAS
corsika_LDFLAGS = -Wl,--rpath-link=@CORSIKA_LIB_DIR@
endif

if USE_TRAPFE
nodist_corsika_SOURCES += trapfpe.c
endif

# choose HE model code
if USE_QGSJET01
nodist_corsika_SOURCES += qgsjet01d.f
endif
if USE_QGSJETII
nodist_corsika_SOURCES += qgsjet-II-04.f
endif
if USE_SIBYLL
nodist_corsika_SOURCES += sibyll2.3c.f
endif
if USE_VENUS
nodist_corsika_SOURCES += venus.f
endif

# choose LE model code
if USE_GHEISHA
nodist_corsika_SOURCES += gheisha_2002d.f
endif

if USE_EFIELD
nodist_corsika_SOURCES += elfield.c
#elfield_o_FFLAGS =
AM_CFLAGS += @EFIELD_CFLAGS@
endif

if USE_PRESHOWER
nodist_corsika_SOURCES += preshower2.0-rev/preshw.c preshower2.0-rev/utils.c preshower2.0-rev/veto.c
#preshw_o_FFLAGS =
AM_CFLAGS += @PRESHOWER_CFLAGS@
endif

if USE_TIMERC
nodist_corsika_SOURCES += timerc.c
endif

endif

endif


#corsika_DEPENDENCY = corsikacompilefile.f
#EXTRA_corsika_SOURCES = trapfpe.c

######################################
# CORSIKA utilities
#

EXTRA_PROGRAMS = map2png plottracks

#corsikaread_SOURCES = utils/corsikaread.f
#corsikaread_thin_SOURCES = utils/corsikaread_thin.f
#corsikaread_history_SOURCES = utils/corsikaread_history.f
mpi_runner_SOURCES = parallel/mpi_runner.c
#showparallel_SOURCES = parallel/showparallel.f

map2png_SOURCES = utils/map2png.c
map2png_LDADD = -lpng -lm

plottracks_SOURCES = utils/plottracks3c.f

EXTRA_DIST = corsika.F corsika.h \
	     gheisha_2002d.f qgsjet01d.f qgsjet-II-04.f sibyll2.3c.f venus.f \
	     timerc.c trapfpe.c elfield.c tobuf.c \
             parallel/* utils/* preshower2.0-rev/*

clean-generic:
	rm -f $(srcdir)/corsikacompilefile.f

distclean-local:
	rm -f *.dvi *.aux *.log *~
