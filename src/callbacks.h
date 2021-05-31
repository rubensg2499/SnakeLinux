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
