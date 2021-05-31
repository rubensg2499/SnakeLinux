#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define TAMSERP 20

#define CUERPO	1
#define CABEZA	2
#define COLA	3

#define IZQ		1
#define DER		2
#define ARR		3
#define ABA		4

#define CRECE	1
#define ACHICA	2
#define NADA	3

GdkDrawable *pixmap = NULL;
GdkPixmap *img = NULL;


void
on_window_destroy(GtkObject *object, gpointer user_data)
{
    gtk_exit(0);
}


void
on_nuevo_activate(GtkMenuItem *menuitem, gpointer user_data)
{

}


void
on_salir_activate(GtkMenuItem *menuitem, gpointer user_data)
{
    on_window_destroy((GtkObject *) menuitem, user_data);
}


void
on_acerca_de_activate(GtkMenuItem *menuitem, gpointer user_data)
{

}


gboolean on_window_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {

  return FALSE;
}


gboolean on_drawingarea_expose_event (GtkWidget *widget, GdkEventExpose  *event, gpointer user_data) {
  gdk_draw_pixmap(widget->window,
					widget->style->fg_gc[GTK_WIDGET_STATE(widget)],
					pixmap,
					event->area.x, event->area.y,
					event->area.x, event->area.y,
					event->area.width, event->area.height);		
	return FALSE;
}


gboolean on_drawingarea_configure_event(GtkWidget *widget, GdkEventConfigure *event, gpointer user_data) {
  if(pixmap != NULL)
		gdk_pixmap_unref(pixmap);
		
	pixmap = gdk_pixmap_new(widget->window,
							widget->allocation.width,
							widget->allocation.height,
							-1);
	trazar(widget);

	return TRUE;
}

void trazar(GtkWidget * widget){
    GdkRectangle update_rect;
    GtkWidget *where_to_draw = lookup_widget(widget,"drawingarea");
    GdkFont *font = NULL;
    
    if(font == NULL)
		  font = gdk_font_load("fixed");
    
    gdk_draw_rectangle(pixmap, widget->style->white_gc,
              TRUE,
              0, 0,
              where_to_draw->allocation.width,
              where_to_draw->allocation.height);
    
    update_rect.x = 0;
    update_rect.y = 0;
    update_rect.width = where_to_draw->allocation.width;
    update_rect.height = where_to_draw->allocation.height;

    gtk_widget_draw(where_to_draw, &update_rect);  
}