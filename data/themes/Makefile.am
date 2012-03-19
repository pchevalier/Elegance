AUTOMAKE_OPTIONS     = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in

EDJE_CC = @edje_cc@
EDJE_FLAGS = -v -id $(top_srcdir)/data/themes -fd $(top_srcdir)/data/themes
filesdir = $(datadir)/elegance/themes
files_DATA = elegance.edj

EXTRA_DIST = \
elegance.edc

elegance.edj: Makefile $(EXTRA_DIST)
	$(EDJE_CC) $(EDJE_FLAGS) \
	$(top_srcdir)/data/themes/elegance.edc \
	$(top_builddir)/data/themes/elegance.edj


clean-local:
	rm -f *.edj

