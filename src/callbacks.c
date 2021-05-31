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

struct pos {
	int x;
	int y;
};
typedef struct pos POS;

struct PedacitoS {
	POS pos;
	int tipo;
	int dir;
};
typedef struct PedacitoS PEDACITOS;

struct comida {
	POS pos;
	int tipo;
};
typedef struct comida COMIDA;

COMIDA com = { {0,0}, NADA };

GdkDrawable *pixmap = NULL;
GdkPixmap *img = NULL;

static PEDACITOS* serpiente = NULL;
static int tams = 5;
static int cuenta = 0;

//Prototipos de función
void trazar(GtkWidget *);
PEDACITOS * NuevaSerpiente(int);

void on_window_destroy(GtkObject *object, gpointer user_data){
    gtk_exit(0);
}


void on_nuevo_activate(GtkMenuItem *menuitem, gpointer user_data){
    serpiente = NuevaSerpiente(5);
    printf("Hola mundo\n");
}

void on_salir_activate(GtkMenuItem *menuitem, gpointer user_data){
    on_window_destroy((GtkObject *) menuitem, user_data);
}


void on_acerca_de_activate(GtkMenuItem *menuitem, gpointer user_data){

}

gboolean on_window_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
  switch(event->keyval)
	{
    case GDK_Right:
			printf("Derecha\n");
		break;
		case GDK_Left:
			printf("Izquierda\n");
		break;
		case GDK_Up:
			printf("Arriba\n");
		break;
		case GDK_Down:
			printf("Abajo\n");
		break;
		default: break;
	}
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
  serpiente = NuevaSerpiente(5);
	DibujarSerpiente(widget);

	return TRUE;
}

void DibujarSerpiente(GtkWidget * widget){
    GdkRectangle update_rect;
    GtkWidget *where_to_draw = lookup_widget(widget,"drawingarea");
    GdkFont *font = NULL;
    int i = 1;
    if(font == NULL)
		  font = gdk_font_load("fixed");
    
    gdk_draw_rectangle(pixmap, widget->style->white_gc,
              TRUE,
              0, 0,
              where_to_draw->allocation.width,
              where_to_draw->allocation.height);    

    //Inicio área de dibujo   
    //DIbujar cola 
    switch (serpiente[0].dir)
    {
      case DER:
        gdk_draw_line(pixmap, widget->style->black_gc, 
        serpiente[0].pos.x * TAMSERP + TAMSERP, 
        serpiente[0].pos.y * TAMSERP, 
        serpiente[0].pos.x * TAMSERP, 
        serpiente[0].pos.y * TAMSERP + TAMSERP / 2);
        
        gdk_draw_line(pixmap, widget->style->black_gc, 
        serpiente[0].pos.x * TAMSERP, 
        serpiente[0].pos.y * TAMSERP + TAMSERP / 2, 
        serpiente[0].pos.x * TAMSERP + TAMSERP, 
        serpiente[0].pos.y * TAMSERP + TAMSERP);

        gdk_draw_line(pixmap, widget->style->black_gc, 
        serpiente[0].pos.x * TAMSERP + TAMSERP, 
        serpiente[0].pos.y * TAMSERP + TAMSERP,
        serpiente[0].pos.x * TAMSERP + TAMSERP,
        serpiente[0].pos.y * TAMSERP);
      break;
      case IZQ:
        gdk_draw_line(pixmap, widget->style->black_gc, 
        serpiente[0].pos.x * TAMSERP,
        serpiente[0].pos.y * TAMSERP, 
        serpiente[0].pos.x * TAMSERP + TAMSERP,
        serpiente[0].pos.y * TAMSERP + TAMSERP / 2);
        
        gdk_draw_line(pixmap, widget->style->black_gc, 
        serpiente[0].pos.x * TAMSERP + TAMSERP,
        serpiente[0].pos.y * TAMSERP + TAMSERP / 2, 
        serpiente[0].pos.x * TAMSERP,
        serpiente[0].pos.y * TAMSERP + TAMSERP);

        gdk_draw_line(pixmap, widget->style->black_gc, 
        serpiente[0].pos.x * TAMSERP,
        serpiente[0].pos.y * TAMSERP + TAMSERP,
        serpiente[0].pos.x * TAMSERP, 
        serpiente[0].pos.y * TAMSERP);
      break;
      case ARR:
        gdk_draw_line(pixmap, widget->style->black_gc, 
        serpiente[0].pos.x * TAMSERP,
        serpiente[0].pos.y * TAMSERP, 
        serpiente[0].pos.x * TAMSERP + TAMSERP / 2,
        serpiente[0].pos.y * TAMSERP + TAMSERP);
        
        gdk_draw_line(pixmap, widget->style->black_gc, 
        serpiente[0].pos.x * TAMSERP + TAMSERP / 2,
        serpiente[0].pos.y * TAMSERP + TAMSERP, 
        serpiente[0].pos.x * TAMSERP + TAMSERP,
        serpiente[0].pos.y * TAMSERP);

        gdk_draw_line(pixmap, widget->style->black_gc, 
        serpiente[0].pos.x * TAMSERP + TAMSERP,
        serpiente[0].pos.y * TAMSERP,
        serpiente[0].pos.x * TAMSERP,
        serpiente[0].pos.y * TAMSERP);
      break;
      case ABA:
        gdk_draw_line(pixmap, widget->style->black_gc, 
        serpiente[0].pos.x * TAMSERP,
        serpiente[0].pos.y * TAMSERP + TAMSERP, 
        serpiente[0].pos.x * TAMSERP + TAMSERP / 2,
        serpiente[0].pos.y * TAMSERP);
        
        gdk_draw_line(pixmap, widget->style->black_gc, 
        serpiente[0].pos.x * TAMSERP + TAMSERP / 2,
        serpiente[0].pos.y * TAMSERP, 
        serpiente[0].pos.x * TAMSERP + TAMSERP,
        serpiente[0].pos.y * TAMSERP + TAMSERP);

        gdk_draw_line(pixmap, widget->style->black_gc, 
        serpiente[0].pos.x * TAMSERP + TAMSERP,
        serpiente[0].pos.y * TAMSERP + TAMSERP,
        serpiente[0].pos.x * TAMSERP,
        serpiente[0].pos.y * TAMSERP + TAMSERP);
      break;
    }
    
    //Dibujar cuerpo
    while (serpiente[i].tipo != CABEZA) {
      gdk_draw_rectangle(pixmap, 
              widget->style->black_gc,
              FALSE,
              serpiente[i].pos.x * TAMSERP,
              serpiente[i].pos.y * TAMSERP,
              TAMSERP,
              TAMSERP);
      i++;
	  }
    
    //Dibujar cabeza
    gdk_draw_rectangle(pixmap, 
              widget->style->black_gc,
              FALSE,
              serpiente[i].pos.x * TAMSERP,
              serpiente[i].pos.y * TAMSERP,
              TAMSERP,
              TAMSERP);

    	switch (serpiente[i].dir)
      {
      case DER:
        gdk_draw_arc(pixmap, widget->style->black_gc, 
          FALSE,
          serpiente[i].pos.x * TAMSERP, 
          serpiente[i].pos.y * TAMSERP,
          TAMSERP / 2,
          TAMSERP / 2,
          0, (64*360));
        gdk_draw_arc(pixmap, widget->style->black_gc, 
          FALSE,
          serpiente[i].pos.x * TAMSERP, 
          serpiente[i].pos.y * TAMSERP + TAMSERP / 2,
          TAMSERP / 2,
          TAMSERP / 2,
          0, (64*360));
        break;
      case IZQ:
        gdk_draw_arc(pixmap, widget->style->black_gc, 
          FALSE,
          serpiente[i].pos.x * TAMSERP + TAMSERP / 2, 
          serpiente[i].pos.y * TAMSERP,
          TAMSERP / 2,
          TAMSERP / 2,
          0, (64*360));
        gdk_draw_arc(pixmap, widget->style->black_gc, 
          FALSE,
          serpiente[i].pos.x * TAMSERP + TAMSERP / 2, 
          serpiente[i].pos.y * TAMSERP + TAMSERP / 2,
          TAMSERP / 2,
          TAMSERP / 2,
          0, (64*360));
        break;
      case ARR:
        gdk_draw_arc(pixmap, widget->style->black_gc, 
          FALSE,
          serpiente[i].pos.x * TAMSERP, 
          serpiente[i].pos.y * TAMSERP + TAMSERP / 2,
          TAMSERP / 2,
          TAMSERP / 2,
          0, (64*360));
        gdk_draw_arc(pixmap, widget->style->black_gc, 
          FALSE,
          serpiente[i].pos.x * TAMSERP + TAMSERP / 2, 
          serpiente[i].pos.y * TAMSERP + TAMSERP / 2,
          TAMSERP / 2,
          TAMSERP / 2,
          0, (64*360));
        break;
      case ABA:
        gdk_draw_arc(pixmap, widget->style->black_gc, 
          FALSE,
          serpiente[i].pos.x * TAMSERP, 
          serpiente[i].pos.y * TAMSERP,
          TAMSERP / 2,
          TAMSERP / 2,
          0, (64*360));
        gdk_draw_arc(pixmap, widget->style->black_gc, 
          FALSE,
          serpiente[i].pos.x * TAMSERP + TAMSERP / 2, 
          serpiente[i].pos.y * TAMSERP,
          TAMSERP / 2,
          TAMSERP / 2,
          0, (64*360));
        break;
      }
    //Fin área de dibujo
    update_rect.x = 0;
    update_rect.y = 0;
    update_rect.width = where_to_draw->allocation.width;
    update_rect.height = where_to_draw->allocation.height;
    gtk_widget_draw(where_to_draw, &update_rect);  
}

PEDACITOS * NuevaSerpiente(int tams){
  PEDACITOS* serpiente = NULL;
	int i;
	if (tams < 2)
		tams = 2;
	serpiente = (PEDACITOS*)malloc(sizeof(PEDACITOS) * tams);
	if (serpiente == NULL) {
		//essageBox(NULL, L"Sin memoria", L"Error", MB_OK | MB_ICONERROR);
		printf("Sin memoria");
    exit(0);
	}
  printf("Serpiente creada con exito");
	serpiente[0].tipo = COLA;
	serpiente[0].pos.x = 1;
	serpiente[0].pos.y = 1;
	serpiente[0].dir = DER;

	for (i = 1; i < tams - 1; i++) {
		serpiente[i].tipo = CUERPO;
		serpiente[i].pos.x = i + 1;
		serpiente[i].pos.y = 1;
		serpiente[i].dir = DER;
	}

	serpiente[i].tipo = CABEZA;
	serpiente[i].pos.x = tams;
	serpiente[i].pos.y = 1;
	serpiente[i].dir = ABA;

	return serpiente;
}