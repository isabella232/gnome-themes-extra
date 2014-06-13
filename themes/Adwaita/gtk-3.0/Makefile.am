themedir = $(datadir)/themes/Adwaita/gtk-3.0
theme_DATA = 		\
	gtk.gresource	\
	gtk.css 	\
	gtk-dark.css	\
	settings.ini

resource_files = $(shell $(GLIB_COMPILE_RESOURCES) --sourcedir=$(srcdir) --generate-dependencies $(srcdir)/gtk.gresource.xml)
gtk.gresource: gtk.gresource.xml $(resource_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --target=$@ --sourcedir=$(srcdir)  $<

EXTRA_DIST = \
	$(resource_files) \
	gtk.css \
	gtk-dark.css \
	gtk.gresource.xml \
	settings.ini

CLEANFILES = \
	gtk.gresource
