#include <gtk/gtk.h>


void
on_window_destroy                      (GtkObject       *object,
                                        gpointer         user_data);

void
on_nuevo_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_salir_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_acerca_de_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_window_key_press_event              (GtkWidget       *widget,
                                        GdkEventKey     *event,
                                        gpointer         user_data);

gboolean
on_drawingarea_expose_event            (GtkWidget       *widget,
                                        GdkEventExpose  *event,
                                        gpointer         user_data);

gboolean
on_drawingarea_configure_event         (GtkWidget       *widget,
                                        GdkEventConfigure *event,
                                        gpointer         user_data);
