/* Adwaita - a GTK+ engine
 *
 * Copyright (C) 2011 Carlos Garnacho <carlosg@gnome.org>
 * Copyright (C) 2011 Red Hat, Inc.
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
 * Authors: Carlos Garnacho <carlosg@gnome.org>
 *          Cosimo Cecchi <cosimoc@gnome.org>
 *
 * Project contact: <gnome-themes-list@gnome.org>
 */

#include <gtk/gtk.h>
#include <gmodule.h>
#include <math.h>
#include <cairo-gobject.h>

#ifdef GDK_WINDOWING_X11
#include <gdk/gdkx.h>
#endif

typedef struct _AdwaitaEngine AdwaitaEngine;
typedef struct _AdwaitaEngineClass AdwaitaEngineClass;

struct _AdwaitaEngine
{
  GtkThemingEngine parent_object;

  guint wm_watch_id;
  GtkCssProvider *fallback_provider;
};

struct _AdwaitaEngineClass
{
  GtkThemingEngineClass parent_class;
};

#define ADWAITA_TYPE_ENGINE		 (adwaita_engine_get_type ())
#define ADWAITA_ENGINE(object)		 (G_TYPE_CHECK_INSTANCE_CAST ((object), ADWAITA_TYPE_ENGINE, AdwaitaEngine))
#define ADWAITA_ENGINE_CLASS(klass)	 (G_TYPE_CHECK_CLASS_CAST ((klass), ADWAITA_TYPE_ENGINE, AdwaitaEngineClass))
#define ADWAITA_IS_ENGINE(object)	 (G_TYPE_CHECK_INSTANCE_TYPE ((object), ADWAITA_TYPE_ENGINE))
#define ADWAITA_IS_ENGINE_CLASS(klass)	 (G_TYPE_CHECK_CLASS_TYPE ((klass), ADWAITA_TYPE_ENGINE))
#define ADWAITA_ENGINE_GET_CLASS(obj)	 (G_TYPE_INSTANCE_GET_CLASS ((obj), ADWAITA_TYPE_ENGINE, AdwaitaEngineClass))

GType adwaita_engine_get_type	    (void) G_GNUC_CONST;
void  adwaita_engine_register_types (GTypeModule *module);

G_DEFINE_DYNAMIC_TYPE (AdwaitaEngine, adwaita_engine, GTK_TYPE_THEMING_ENGINE)

void
adwaita_engine_register_types (GTypeModule *module)
{
  adwaita_engine_register_type (module);
}

static void
fallback_provider_remove (AdwaitaEngine *self)
{
  GdkScreen *screen;

  if (self->fallback_provider == NULL)
    return;

  screen = gdk_screen_get_default ();
  gtk_style_context_remove_provider_for_screen
    (screen, GTK_STYLE_PROVIDER (self->fallback_provider));
  g_clear_object (&self->fallback_provider);
}

static void
fallback_provider_add (AdwaitaEngine *self)
{
  GFile *resource;
  GtkCssProvider *provider;
  GError *error = NULL;
  GdkScreen *screen;

  if (self->fallback_provider != NULL)
    return;

  resource = g_file_new_for_uri ("resource:///org/gnome/adwaita/gtk-fallback.css");
  provider = gtk_css_provider_new ();
  gtk_css_provider_load_from_file (provider, resource, &error);
  g_object_unref (resource);

  if (error != NULL)
    {
      g_warning ("Can't load fallback CSS resource: %s", error->message);
      g_error_free (error);
      g_object_unref (provider);
      return;
    }

  screen = gdk_screen_get_default ();
  gtk_style_context_add_provider_for_screen
    (screen, GTK_STYLE_PROVIDER (provider), GTK_STYLE_PROVIDER_PRIORITY_THEME);
  self->fallback_provider = provider;
}

static void
adwaita_engine_wm_changed (AdwaitaEngine *self)
{
  gboolean is_fallback = TRUE;

#ifdef GDK_WINDOWING_X11
  const gchar *name;
  GdkScreen *screen = gdk_screen_get_default ();
  if (GDK_IS_X11_SCREEN (screen))
    {
      name = gdk_x11_screen_get_window_manager_name (screen);
      is_fallback = (g_strcmp0 (name, "GNOME Shell") != 0);
    }
#endif

  if (is_fallback)
    fallback_provider_add (self);
  else
    fallback_provider_remove (self);
}

static void
adwaita_engine_finalize (GObject *obj)
{
  AdwaitaEngine *self = ADWAITA_ENGINE (obj);

  if (self->wm_watch_id != 0)
    {
      g_signal_handler_disconnect (gdk_screen_get_default (), self->wm_watch_id);
      self->wm_watch_id = 0;
    }

  fallback_provider_remove (self);

  G_OBJECT_CLASS (adwaita_engine_parent_class)->finalize (obj);
}

static void
adwaita_engine_init (AdwaitaEngine *self)
{
#ifdef GDK_WINDOWING_X11
  GdkScreen *screen = gdk_screen_get_default ();

  if (GDK_IS_X11_SCREEN (screen))
    {
      self->wm_watch_id =
	g_signal_connect_swapped (screen, "window-manager-changed",
				  G_CALLBACK (adwaita_engine_wm_changed), self);
    }
#endif

  adwaita_engine_wm_changed (self);
}

static void
adwaita_engine_class_init (AdwaitaEngineClass *klass)
{
  GObjectClass *oclass = G_OBJECT_CLASS (klass);

  oclass->finalize = adwaita_engine_finalize;
}

static void
adwaita_engine_class_finalize (AdwaitaEngineClass *klass)
{
}

G_MODULE_EXPORT void
theme_init (GTypeModule *module)
{
  adwaita_engine_register_types (module);
}

G_MODULE_EXPORT void
theme_exit (void)
{
}

G_MODULE_EXPORT GtkThemingEngine *
create_engine (void)
{
  return GTK_THEMING_ENGINE (g_object_new (ADWAITA_TYPE_ENGINE,
                                           "name", "adwaita",
                                           NULL));
}
