resource_files = $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=$(srcdir) --generate-dependencies $(srcdir)/gtk.gresource.xml)
gtk.gresource: gtk.gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir)  $<

themedir = $(datadir)/themes/HighContrast/gtk-3.0
theme_DATA = \
	gtk.css \
	gtk.gresource \
	settings.ini

EXTRA_DIST = \
	$(resource_files) \
	gtk.css \
	gtk.gresource.xml \
	settings.ini

CLEANFILES = \
	gtk.gresource
