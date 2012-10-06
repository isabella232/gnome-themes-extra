moduledir = $(libdir)/gtk-3.0/modules
module_LTLIBRARIES = libshellwatcher.la

libshellwatcher_la_SOURCES = shell-watcher-gtk3.c
libshellwatcher_la_CFLAGS = -I. $(WATCHER_MODULE_GTK3_CFLAGS)
libshellwatcher_la_LIBADD = $(WATCHER_MODULE_GTK3_LIBS)
libshellwatcher_la_LDFLAGS = -no-undefined -module -avoid-version -rpath $(moduledir)
