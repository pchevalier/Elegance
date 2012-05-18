AUTOMAKE_OPTIONS     = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in

INCLUDES = \
-I$(top_srcdir) \
-I$(top_srcdir)/src/bin \
-DPACKAGE_DATA_DIR=\"$(datadir)/$(PACKAGE)\" \
@EFL_CFLAGS@ @ELEMENTARY_CFLAGS@

bin_PROGRAMS = elegance

elegance_SOURCES = \
 main.c \
 main.h \
 design.c \
 design.h \
 toolbar.c \
 toolbar.h \
 palette.c \
 palette.h \
 snippets.c \
 snippets.h \
 status.c \
 status.h \
 panel.c \
 panel.h \
 tree.c \
 tree.h \
 view.c \
 view.h \
 dnd.c \
 dnd.h \
 serialize.c \
 serialize.h \
specificity.c \
specificity.h

elegancedir = $(libdir)
elegance_LDADD = @EFL_LIBS@ @ELEMENTARY_LIBS@
elegance_CFLAGS =

### used for building a a quicklaunch mode version. not used/needed here
#elegance_CFLAGS = -DELM_LIB_QUICKLAUNCH=1
#elegance_la_SOURCES = $(elegance_SOURCES)
#elegance_LTLIBRARIES = elegance.la
#elegance_la_LIBADD = @ELEMENTARY_LIBS@
#elegance_la_CFLAGS =
#elegance_la_LDFLAGS = -module -avoid-version -no-undefined
