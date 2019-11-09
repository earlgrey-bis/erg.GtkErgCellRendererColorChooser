/* gtkcellrenderertext.h
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

#ifndef __GTK_ERG_CELL_RENDERER_COLOR_CHOOSER_H__
#define __GTK_ERG_CELL_RENDERER_COLOR_CHOOSER_H__


//#if !defined (__GTK_H_INSIDE__) && !defined (GTK_COMPILATION)
//#error "Only <gtk/gtk.h> can be included directly."
//#endif

//#include <gtk/gtkcellrenderer.h>
#include <gtk/gtk.h>


G_BEGIN_DECLS


#define GTK_TYPE_ERG_CELL_RENDERER_COLOR_CHOOSER		    (gtk_erg_cell_renderer_color_chooser_get_type ())
#define GTK_ERG_CELL_RENDERER_COLOR_CHOOSER(obj)		    (G_TYPE_CHECK_INSTANCE_CAST ((obj)  , GTK_TYPE_ERG_CELL_RENDERER_COLOR_CHOOSER, GtkErgCellRendererColorChooser))
#define GTK_ERG_CELL_RENDERER_COLOR_CHOOSER_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass)   , GTK_TYPE_ERG_CELL_RENDERER_COLOR_CHOOSER, GtkErgCellRendererColorChooserClass))
#define GTK_IS_ERG_CELL_RENDERER_COLOR_CHOOSER(obj)		    (G_TYPE_CHECK_INSTANCE_TYPE ((obj)  , GTK_TYPE_ERG_CELL_RENDERER_COLOR_CHOOSER))
#define GTK_IS_ERG_CELL_RENDERER_COLOR_CHOOSER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass)   , GTK_TYPE_ERG_CELL_RENDERER_COLOR_CHOOSER))
#define GTK_ERG_CELL_RENDERER_COLOR_CHOOSER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj)   , GTK_TYPE_ERG_CELL_RENDERER_COLOR_CHOOSER, GtkErgCellRendererColorChooserClass))

typedef struct _GtkErgCellRendererColorChooser              GtkErgCellRendererColorChooser;
typedef struct _GtkErgCellRendererColorChooserPrivate       GtkErgCellRendererColorChooserPrivate;
typedef struct _GtkErgCellRendererColorChooserClass         GtkErgCellRendererColorChooserClass;

struct _GtkErgCellRendererColorChooser
{
  GtkCellRenderer parent;

  /*< private >*/
  GtkErgCellRendererColorChooserPrivate *priv;
};

struct _GtkErgCellRendererColorChooserClass
{
    GtkCellRendererClass parent_class;

    void (*color_picked)
    (
        GtkErgCellRendererColorChooser  *   _i_cell_renderer_text,
        const gchar                     *   _i_path,
        const gchar                     *   _i_new_text
    );

    /* Padding for future expansion */
    void (*_gtk_reserved1) (void);
    void (*_gtk_reserved2) (void);
    void (*_gtk_reserved3) (void);
    void (*_gtk_reserved4) (void);
};

GDK_AVAILABLE_IN_ALL    GType               gtk_erg_cell_renderer_color_chooser_get_type                    (void) G_GNUC_CONST;

GDK_AVAILABLE_IN_ALL    GtkCellRenderer *   gtk_erg_cell_renderer_color_chooser_new                         (void);

GDK_AVAILABLE_IN_ALL    void                gtk_erg_cell_renderer_color_chooser_set_fixed_height_from_font  (
                                                GtkErgCellRendererColorChooser  *   renderer        ,
                                                gint                                number_of_rows  );


G_END_DECLS

#endif /* __GTK_ERG_CELL_RENDERER_COLOR_CHOOSER_H__ */
