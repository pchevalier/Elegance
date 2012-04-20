AUTOMAKE_OPTIONS     = 1.4 foreign
MAINTAINERCLEANFILES = Makefile.in

EDJE_CC = @edje_cc@
EDJE_FLAGS = -v -id $(top_srcdir)/data/themes -fd $(top_srcdir)/data/themes
filesdir = $(datadir)/elegance/themes
files_DATA = elegance.edj \
	plus.png \
	 flip_base.png  \
  	 flip_base_shad.png  \
         flip_shad.png  \
         flip_0t.png  \
         flip_0b.png  \
         flip_1t.png  \
         flip_1b.png  \
         flip_2t.png  \
         flip_2b.png  \
         flip_3t.png  \
         flip_3b.png  \
         flip_4t.png  \
         flip_4b.png  \
         flip_5t.png  \
         flip_5b.png  \
         flip_6t.png  \
         flip_6b.png  \
         flip_7t.png  \
         flip_7b.png  \
         flip_8t.png  \
         flip_8b.png  \
         flip_9t.png  \
         flip_9b.png  \
         arrow_up.png  \
         arrow_down.png

EXTRA_DIST = \
elegance.edc

elegance.edj: Makefile $(EXTRA_DIST)
	$(EDJE_CC) $(EDJE_FLAGS) \
	$(top_srcdir)/data/themes/elegance.edc \
	$(top_builddir)/data/themes/elegance.edj


clean-local:
	rm -f *.edj

