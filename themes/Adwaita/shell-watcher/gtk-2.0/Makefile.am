moduledir = $(libdir)/gtk-2.0/modules
module_LTLIBRARIES = libshellwatcher.la

libshellwatcher_la_SOURCES = shell-watcher-gtk2.c
libshellwatcher_la_CFLAGS = -I. $(WATCHER_MODULE_GTK2_CFLAGS)
libshellwatcher_la_LIBADD = $(WATCHER_MODULE_GTK2_LIBS)
libshellwatcher_la_LDFLAGS = -no-undefined -module -avoid-version -rpath $(moduledir)
