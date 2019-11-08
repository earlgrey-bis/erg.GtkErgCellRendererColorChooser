/* gtkcellrenderertext.c
 * Copyright (C) 2000  Red Hat, Inc.,  Jonathan Blandford <jrb@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <config.h>

#include "GtkErgCellRendererColorChooser.h"

#include <stdlib.h>

#include <gtk/gtkeditable.h>
#include <gtk/gtkentry.h>
#include <gtk/gtksizerequest.h>
#include <gtk/gtkmarshalers.h>
#include <gtk/gtkintl.h>
#include <gtk/gtkprivate.h>
#include <gtk/gtktreeprivate.h>

//#include <gtk/a11y/gtktextcellaccessible.h>
#include <gtk/gtk-a11y.h>


/**
 * SECTION:gtkcellrenderertext
 * @Short_description: Renders text in a cell
 * @Title: GtkErgCellRendererColorChooser
 *
 * A #GtkErgCellRendererColorChooser renders a given text in its cell, using the font, color and
 * style information provided by its properties. The text will be ellipsized if it is
 * too long and the #GtkErgCellRendererColorChooser:ellipsize property allows it.
 *
 * If the #GtkCellRenderer:mode is %GTK_CELL_RENDERER_MODE_EDITABLE,
 * the #GtkErgCellRendererColorChooser allows to edit its text using an entry.
 */


//  ************************************************************************************************
//  Declarations
//  ************************************************************************************************
static void gtk_erg_cell_renderer_color_chooser_finalize   (GObject                  *object);

static void gtk_erg_cell_renderer_color_chooser_get_property  (GObject                  *object,
						  guint                     param_id,
						  GValue                   *value,
						  GParamSpec               *pspec);

static void gtk_erg_cell_renderer_color_chooser_set_property(
    GObject                  *object,
    guint                     param_id,
    const GValue             *value,
    GParamSpec               *pspec);

static void gtk_erg_cell_renderer_color_chooser_render(
     GtkCellRenderer          *cell,
    cairo_t                  *cr,
    GtkWidget                *widget,
    const GdkRectangle       *background_area,
    const GdkRectangle       *cell_area,
    GtkCellRendererState      flags);

static GtkCellEditable  *   gtk_erg_cell_renderer_color_chooser_start_editing(
                                GtkCellRenderer      *cell,
                                GdkEvent             *event,
                                GtkWidget            *widget,
                                const gchar          *path,
                                const GdkRectangle   *background_area,
                                const GdkRectangle   *cell_area,
                                GtkCellRendererState  flags);

static  gboolean            gtk_erg_cell_renderer_color_chooser_activate(
                                GtkCellRenderer         *   crdr            ,
                                GdkEvent                *   event           ,
                                GtkWidget               *   widget          ,
                                const gchar             *   path            ,
                                const GdkRectangle      *   background_area ,
                                const GdkRectangle      *   cell_area       ,
                                GtkCellRendererState        flags           );

static void       gtk_erg_cell_renderer_color_chooser_get_preferred_width            (GtkCellRenderer       *cell,
                                                                         GtkWidget             *widget,
                                                                         gint                  *minimal_size,
                                                                         gint                  *natural_size);
static void       gtk_erg_cell_renderer_color_chooser_get_preferred_height           (GtkCellRenderer       *cell,
                                                                         GtkWidget             *widget,
                                                                         gint                  *minimal_size,
                                                                         gint                  *natural_size);
static void       gtk_erg_cell_renderer_color_chooser_get_preferred_height_for_width (GtkCellRenderer       *cell,
                                                                         GtkWidget             *widget,
                                                                         gint                   width,
                                                                         gint                  *minimum_height,
                                                                         gint                  *natural_height);
static void       gtk_erg_cell_renderer_color_chooser_get_aligned_area               (GtkCellRenderer       *cell,
									 GtkWidget             *widget,
									 GtkCellRendererState   flags,
									 const GdkRectangle    *cell_area,
									 GdkRectangle          *aligned_area);


static          void    set_color(
                            GtkErgCellRendererColorChooser  *   celltext    ,
                            GdkRGBA                         *   rgba        );

static inline   gboolean  show_placeholder_text(
                            GtkErgCellRendererColorChooser  *   celltext    );
//  ************************************************************************************************
//  Structs etc...
//  ************************************************************************************************
enum
{
      EDITED        ,
      LAST_SIGNAL
};

enum
{
    PROP_0              ,

    PROP_EDITABLE       ,
    PROP_ACTIVATABLE    ,

    PROP_TEXT           ,

    PROP_COLOR_RGBA     ,

    //  ............................................................................................

    PROP_EDITABLE_SET       ,
    PROP_ACTIVATABLE_SET    ,

    LAST_PROP
};

static guint            text_cell_renderer_signals  [LAST_SIGNAL];
static GParamSpec   *   text_cell_renderer_props    [LAST_PROP];

#define GTK_ERG_CELL_RENDERER_COLOR_CHOOSER_PATH "gtk-erg-cell-renderer-color-chooser"

struct _GtkErgCellRendererColorChooserPrivate
{

    GtkWidget           *   d_color_button;

    GdkRGBA                 a_color;
    GdkRGBA                 a_fg;
    GdkRGBA                 a_bg;

    gchar               *   d_text;
    gchar               *   placeholder_text;

    gulong                  focus_out_id;

    guint                   a_editable          :   1;
    guint                   a_activatable       :   1;

    guint                   a_editable_set      :   1;
    guint                   a_activatable_set   :   1;
};

G_DEFINE_TYPE_WITH_PRIVATE (GtkErgCellRendererColorChooser, gtk_erg_cell_renderer_color_chooser, GTK_TYPE_CELL_RENDERER)
//  ************************************************************************************************
//  Gtk boilerplate
//  ************************************************************************************************
#include    "crcc-bpl.ci"
//  ************************************************************************************************
//  GET / SET
//  ************************************************************************************************
#include    "crcc-get.ci"
#include    "crcc-set.ci"
//  ************************************************************************************************
//  FONTS
//  ************************************************************************************************
//  #include    "crcc-fonts.ci"
//  ************************************************************************************************
//  ...
//  ************************************************************************************************
static void
set_color(
    GtkErgCellRendererColorChooser  *   _i_crcc ,
    GdkRGBA                         *   _i_rgba )
{
    GtkErgCellRendererColorChooserPrivate   *   priv    =   _i_crcc->priv;

    if (_i_rgba)
    {
        //if ( ! priv->background_set )
        //{
            //priv->background_set = TRUE;
            //g_object_notify_by_pspec (G_OBJECT (celltext), text_cell_renderer_props[PROP_BACKGROUND_SET]);
        //}

        priv->a_color = *_i_rgba;
    }
    else
    {
        g_warning ("%s\n", "(crcc)set_color:NULL color");
    }
}
/*
static void
set_fg_color (GtkErgCellRendererColorChooser *celltext,
              GdkRGBA             *rgba)
{
  GtkErgCellRendererColorChooserPrivate *priv = celltext->priv;

  if (rgba)
    {
      if (!priv->foreground_set)
        {
          priv->foreground_set = TRUE;
          g_object_notify_by_pspec (G_OBJECT (celltext), text_cell_renderer_props[PROP_FOREGROUND_SET]);
        }

      priv->foreground = *rgba;
    }
  else
    {
      if (priv->foreground_set)
        {
          priv->foreground_set = FALSE;
          g_object_notify_by_pspec (G_OBJECT (celltext), text_cell_renderer_props[PROP_FOREGROUND_SET]);
        }
    }
}
*/
/*
static inline gboolean
show_placeholder_text (GtkErgCellRendererColorChooser *celltext)
{
  GtkErgCellRendererColorChooserPrivate *priv = celltext->priv;

  return priv->editable && priv->placeholder_text &&
    (!priv->text || !priv->text[0]);
}
*/
static void
gtk_erg_cell_renderer_color_chooser_render (GtkCellRenderer      *cell,
			       cairo_t              *cr,
			       GtkWidget            *widget,
			       const GdkRectangle   *background_area,
			       const GdkRectangle   *cell_area,
			       GtkCellRendererState  flags)

{
    GtkErgCellRendererColorChooser          *   celltext    =   GTK_ERG_CELL_RENDERER_COLOR_CHOOSER (cell);
    GtkErgCellRendererColorChooserPrivate   *   priv        =   celltext->priv;
    GtkStyleContext                         *   context;

    printf("(crcc)render [%p]\n", cell);

    //layout = get_layout (celltext, widget, cell_area, flags);
    //get_size (cell, widget, cell_area, layout, &x_offset, &y_offset, NULL, NULL);
    context = gtk_widget_get_style_context (widget);

  //if (priv->background_set && (flags & GTK_CELL_RENDERER_SELECTED) == 0)
    {
        GdkRGBA   color;
        color.red   =   0.90;
        color.green =   0.20;
        color.blue  =   0.35;
        color.alpha =   1.0;

      gdk_cairo_rectangle (cr, background_area);
      gdk_cairo_set_source_rgba (cr, &priv->a_color);
      //gdk_cairo_set_source_rgba (cr, &color);
      cairo_fill (cr);
    }

    //gtk_cell_renderer_get_padding (cell, &xpad, &ypad);


    //cairo_save (cr);

    //gdk_cairo_rectangle (cr, cell_area);
    //cairo_clip (cr);

    //gtk_render_layout (context, cr,
    //                   cell_area->x + x_offset + xpad,
    //                   cell_area->y + y_offset + ypad,
    //                   layout);

    //cairo_restore (cr);

    //g_object_unref (layout);
}
//  ************************************************************************************************
//  EDITING
//  ************************************************************************************************
#include    "crcc-edit.ci"
//  ************************************************************************************************
//  Width Height etc...
//  ************************************************************************************************
static void
gtk_erg_cell_renderer_color_chooser_get_preferred_width (GtkCellRenderer *cell,
                                            GtkWidget       *widget,
                                            gint            *minimum_size,
                                            gint            *natural_size)
{
    GtkErgCellRendererColorChooserPrivate *priv;
    GtkErgCellRendererColorChooser        *celltext;

    printf("(crcc)preferred w\n");


    celltext = GTK_ERG_CELL_RENDERER_COLOR_CHOOSER (cell);
    priv = celltext->priv;

    *( minimum_size )   = 40;
    *( natural_size )   = 40;


}

static void
gtk_erg_cell_renderer_color_chooser_get_preferred_height_for_width (GtkCellRenderer *cell,
                                                       GtkWidget       *widget,
                                                       gint             width,
                                                       gint            *minimum_height,
                                                       gint            *natural_height)
{
    GtkErgCellRendererColorChooser  *   celltext;

    printf("(crcc)preferred h for w\n");

  celltext = GTK_ERG_CELL_RENDERER_COLOR_CHOOSER (cell);

    *( minimum_height )   = 15;
    *( natural_height )   = 15;
}

static void
gtk_erg_cell_renderer_color_chooser_get_preferred_height (GtkCellRenderer *cell,
                                             GtkWidget       *widget,
                                             gint            *minimum_size,
                                             gint            *natural_size)
{
  gint min_width;

    printf("(crcc)preferred h\n");

  /* Thankfully cell renderers dont rotate, so they only have to do
   * height-for-width and not the opposite. Here we have only to return
   * the height for the base minimum width of the renderer.
   *
   * Note this code path wont be followed by GtkTreeView which is
   * height-for-width specifically.
   */
  //gtk_cell_renderer_get_preferred_width (cell, widget, &min_width, NULL);
  //gtk_erg_cell_renderer_color_chooser_get_preferred_height_for_width (cell, widget, min_width,
  //                                                       minimum_size, natural_size);
    *( minimum_size )   = 15;
    *( natural_size )   = 15;
}

static void
gtk_erg_cell_renderer_color_chooser_get_aligned_area (GtkCellRenderer       *cell,
					 GtkWidget             *widget,
					 GtkCellRendererState   flags,
					 const GdkRectangle    *cell_area,
					 GdkRectangle          *aligned_area)
{
    /*
  GtkCellRendererText *celltext = GTK_CELL_RENDERER_TEXT (cell);
  PangoLayout *layout;
  gint x_offset = 0;
  gint y_offset = 0;

  layout = get_layout (celltext, widget, cell_area, flags);
  get_size (cell, widget, cell_area, layout, &x_offset, &y_offset,
	    &aligned_area->width, &aligned_area->height);

  aligned_area->x = cell_area->x + x_offset;
  aligned_area->y = cell_area->y + y_offset;

  g_object_unref (layout);
    */
  aligned_area->x = cell_area->x;
  aligned_area->y = cell_area->y;
}
