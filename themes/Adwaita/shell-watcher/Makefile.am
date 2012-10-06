SUBDIRS = $(NULL)
settings_daemon_desktop_files = adwaita-shell-watcher.desktop

if GTK_MODULES
SUBDIRS += gtk-2.0 gtk-3.0

settings_daemon_modulesdir = $(libdir)/gnome-settings-daemon-3.0/gtk-modules/
settings_daemon_modules_DATA = $(settings_daemon_desktop_files)
endif

EXTRA_DIST = $(settings_daemon_desktop_files)
