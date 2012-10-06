/* Adwaita - a GTK+ engine
 *
 * Copyright (C) 2012 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Authors: Cosimo Cecchi <cosimoc@gnome.org>
 */

#include <glib.h>
#include <gmodule.h>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>

void g_module_unload  (GModule *module);
int  gtk_module_init (int *argc, char** argv[]);

static gulong settings_id = 0;
static guint wm_watch_id = 0;
static gboolean initialized = FALSE;

static void
maybe_reset_styles (void)
{
  /* don't reset styles at startup */
  if (initialized)
    gtk_rc_reset_styles (gtk_settings_get_default ());
  initialized = TRUE;
}

static void
fallback_style_remove (void)
{
  gtk_rc_parse_string ("widget_class \"*<GtkMenu>\" style \"menu_unframed_box\"");
  maybe_reset_styles ();
}

static void
fallback_style_add (void)
{
  gtk_rc_parse_string ("widget_class \"*<GtkMenu>\" style \"menu_framed_box\"");
  maybe_reset_styles ();
}

static void
wm_changed (void)
{
  const gchar *name;
  name = gdk_x11_screen_get_window_manager_name (gdk_screen_get_default ());

  if (g_strcmp0 (name, "GNOME Shell") != 0)
    fallback_style_add ();
  else
    fallback_style_remove ();
}

static void
wm_watch_remove (void)
{
  if (wm_watch_id != 0)
    {
      g_signal_handler_disconnect (gdk_screen_get_default (), wm_watch_id);
      wm_watch_id = 0;
    }
}

static void
wm_watch_install (void)
{
  if (wm_watch_id == 0)
    {
      GdkScreen *screen = gdk_screen_get_default ();

#ifdef GDK_WINDOWING_X11
      wm_watch_id = g_signal_connect (screen, "window-manager-changed",
                                      G_CALLBACK (wm_changed), NULL);
      wm_changed ();
#endif
    }
}

void
g_module_unload (GModule *module)
{
  wm_watch_remove ();

  if (settings_id != 0)
    {
      g_signal_handler_disconnect (gtk_settings_get_default (), settings_id);
      settings_id = 0;
    }
}

static void
maybe_watch_wm (void)
{
  GtkSettings *settings;
  gchar *theme_name;

  settings = gtk_settings_get_default ();
  g_object_get (settings, "gtk-theme-name", &theme_name);

  if (g_strcmp0 (theme_name, "Adwaita") == 0)
    {
      wm_watch_install ();
    }
  else
    {
      wm_watch_remove ();
      fallback_provider_remove ();
    }

  g_free (theme_name);
}

int
gtk_module_init (int *argc, char** argv[])
{
  maybe_watch_wm ();
  settings_id = g_signal_connect (gtk_settings_get_default (), "notify::gtk-theme-name",
                                  G_CALLBACK (maybe_watch_wm), NULL);

  return 0;
}
