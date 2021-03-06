#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "Serpiente.h"

COMIDA com = { {0,0}, NADA };

GdkDrawable *pixmap = NULL;
GdkPixmap *img = NULL;

static int tams = 5;
static int cuenta = 0;
static int muerto = FALSE;
static int contador = 0;
extern PEDACITOS* serpiente;

//Prototipos de función
PEDACITOS * NuevaSerpiente(int);
PEDACITOS * AjustarSerpiente(PEDACITOS*, int*, int, GtkWidget *);
int MoverSerpiente(PEDACITOS*, int, GtkWidget *, int);
int Colisionar(PEDACITOS*, int);
int Comer(PEDACITOS*, int);
void TrazarTablero(GtkWidget *);
void DibujarSerpiente(GtkWidget *);
void GenerarComida(GtkWidget *);

void on_window_destroy(GtkObject *object, gpointer user_data){
    free(serpiente);
    gtk_exit(0);
}

void on_nuevo_activate(GtkMenuItem *menuitem, gpointer user_data){
    if(serpiente != NULL){
      free(serpiente);
      tams = 5;
      cuenta = 0;
      muerto = FALSE;
      contador = 0;
      serpiente = NuevaSerpiente(tams);
    }
}

void on_salir_activate(GtkMenuItem *menuitem, gpointer user_data){
    on_window_destroy((GtkObject *) menuitem, user_data);
}

void on_acerca_de_activate(GtkMenuItem *menuitem, gpointer user_data){
  GtkWidget *message_dialog;
  message_dialog = gtk_message_dialog_new (user_data, GTK_DIALOG_MODAL, 
                                            GTK_MESSAGE_INFO, 
                                            GTK_BUTTONS_OK, 
                                            "Snake v1.0\nRubén Sánchez González.\nIngeniería en computación., 804\nUNISTMO Tehuantepec.\nSistemas de cómputo paralelos y distribuidos.\nM.C. J. Jesús Arellano Pimentel."); 
  gtk_window_set_title(GTK_WINDOW(message_dialog), "Acerca de");
  gtk_dialog_run(GTK_DIALOG(message_dialog));
  gtk_widget_destroy(message_dialog);
}

gboolean on_window_key_press_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data) {
  GtkWidget *message_dialog;
  message_dialog = gtk_message_dialog_new (user_data, GTK_DIALOG_MODAL, 
                                            GTK_MESSAGE_ERROR, 
                                            GTK_BUTTONS_OK, 
                                            "Ya se murió, F"); 
  gtk_window_set_title(GTK_WINDOW(message_dialog), "Fin del juego");
  switch(event->keyval)
	{
    case GDK_Right:
			if(!muerto){
        if(!MoverSerpiente(serpiente, DER, widget, tams)){
          gtk_dialog_run(GTK_DIALOG(message_dialog));
          gtk_widget_destroy(message_dialog);                              
        }
        if (Comer(serpiente, tams)) {
            serpiente = AjustarSerpiente(serpiente, &tams, com.tipo, widget);
            com.tipo = NADA;
        }
      }
      DibujarSerpiente(widget);
		break;
		case GDK_Left:
      if(!muerto){
        if(!MoverSerpiente(serpiente, IZQ, widget, tams)){
          gtk_dialog_run(GTK_DIALOG(message_dialog));
          gtk_widget_destroy(message_dialog); 
        }
        if (Comer(serpiente, tams)) {
            serpiente = AjustarSerpiente(serpiente, &tams, com.tipo, widget);
            com.tipo = NADA;
        }
      }
      DibujarSerpiente(widget);
		break;
		case GDK_Up:
      if(!muerto){
        if(!MoverSerpiente(serpiente, ARR, widget, tams)){
          gtk_dialog_run(GTK_DIALOG(message_dialog));
          gtk_widget_destroy(message_dialog); 
        }
        if (Comer(serpiente, tams)) {
            serpiente = AjustarSerpiente(serpiente, &tams, com.tipo, widget);
            com.tipo = NADA;
        }
      }
      DibujarSerpiente(widget);
		break;
		case GDK_Down:
      if(!muerto){
        if(!MoverSerpiente(serpiente, ABA, widget, tams)){
          gtk_dialog_run(GTK_DIALOG(message_dialog));
          gtk_widget_destroy(message_dialog); 
        }
        if (Comer(serpiente, tams)) {
            serpiente = AjustarSerpiente(serpiente, &tams, com.tipo, widget);
            com.tipo = NADA;
        }
      }
      DibujarSerpiente(widget);
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

    //Dibujar comida
    if(com.tipo == CRECE){
      gdk_draw_rectangle(
        pixmap,
        widget->style->black_gc,
        TRUE,
        com.pos.x * TAMSERP,
        com.pos.y * TAMSERP,
        TAMSERP,
        TAMSERP
      );
    }else if(com.tipo == ACHICA){
      gdk_draw_arc(
        pixmap,
        widget->style->black_gc,
        TRUE,
        com.pos.x * TAMSERP,
        com.pos.y * TAMSERP,
        TAMSERP, TAMSERP,
        0, (64*360)
      );
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
		printf("Sin memoria");
    exit(0);
	}
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
	serpiente[i].dir = DER;

	return serpiente;
}

int MoverSerpiente(PEDACITOS* serpiente, int dir, GtkWidget * widget, int tams){
  GtkWidget *tablero = lookup_widget(widget,"drawingarea");
  int i = 0;
	while (serpiente[i].tipo != CABEZA) {
		serpiente[i].dir = serpiente[i + 1].dir;
		serpiente[i].pos = serpiente[i + 1].pos;
		i++;
	}

  switch (serpiente[i].dir)
	{
	case DER:
		if (dir != IZQ)
			serpiente[i].dir = dir;
		break;
	case IZQ:
		if (dir != DER)
			serpiente[i].dir = dir;
		break;
	case ARR:
		if (dir != ABA)
			serpiente[i].dir = dir;
		break;
	case ABA:
		if (dir != ARR)
			serpiente[i].dir = dir;
		break;
	}

  switch (serpiente[i].dir)
	{
	case DER:
		serpiente[i].pos.x = serpiente[i].pos.x + 1;
		if (serpiente[i].pos.x >= tablero->allocation.width / TAMSERP)
			serpiente[i].pos.x = 0;
		break;
	case IZQ:
		serpiente[i].pos.x = serpiente[i].pos.x - 1;
		if (serpiente[i].pos.x < 0)
			serpiente[i].pos.x = tablero->allocation.width / TAMSERP;
		break;
	case ARR:
		serpiente[i].pos.y = serpiente[i].pos.y - 1;
		if (serpiente[i].pos.y < 0)
			serpiente[i].pos.y = tablero->allocation.height / TAMSERP;
		break;
	case ABA:
		serpiente[i].pos.y = serpiente[i].pos.y + 1;
		if (serpiente[i].pos.y > tablero->allocation.height / TAMSERP)
			serpiente[i].pos.y = 0;
		break;
	}

  return !Colisionar(serpiente, tams);
}
int Colisionar(PEDACITOS* serpiente, int tams){
  int i = 0;
	while (serpiente[i].tipo != CABEZA) {
		if (serpiente[i].pos.x == serpiente[tams - 1].pos.x && serpiente[i].pos.y == serpiente[tams - 1].pos.y) {
			muerto = TRUE;
      return 1;
		}
		i++;
	}
	return 0;
}

PEDACITOS * AjustarSerpiente(PEDACITOS* serpiente, int* tams, int comida, GtkWidget* widget){
  int i;
	PEDACITOS cabeza = serpiente[*tams - 1];
  GtkWidget *tablero = lookup_widget(widget,"drawingarea");
	switch (comida)
	{
    case CRECE: {
      (*tams)++;
      
      serpiente = (PEDACITOS *) realloc(serpiente, sizeof(PEDACITOS) * (*tams));
      serpiente[*tams - 2].tipo = CUERPO;
      serpiente[*tams - 1] = cabeza;
      i = *tams - 1;
      switch (serpiente[i].dir)
      {
      case DER:
        serpiente[i].pos.x = serpiente[i].pos.x + 1;
        if (serpiente[i].pos.x >= tablero->allocation.width / TAMSERP)
          serpiente[i].pos.x = 0;
        break;
      case IZQ:
        serpiente[i].pos.x = serpiente[i].pos.x - 1;
        if (serpiente[i].pos.x < 0)
          serpiente[i].pos.x = tablero->allocation.width / TAMSERP;
        break;
      case ARR:
        serpiente[i].pos.y = serpiente[i].pos.y - 1;
        if (serpiente[i].pos.y < 0)
          serpiente[i].pos.y = tablero->allocation.height / TAMSERP;
        break;
      case ABA:
        serpiente[i].pos.y = serpiente[i].pos.y + 1;
        if (serpiente[i].pos.y > tablero->allocation.height / TAMSERP)
          serpiente[i].pos.y = 0;
        break;
      }
      break;
    }
    case ACHICA: {
      if (*tams > 2) {
        
        i = 0;
        while (serpiente[i].tipo != CABEZA) {
          serpiente[i] = serpiente[i + 1];
          i++;
        }
        (*tams)--;
        serpiente = (PEDACITOS*)realloc(serpiente, sizeof(PEDACITOS) * (*tams));
        serpiente[*tams - 1] = cabeza;
      }
      break;
    }
	}
  return serpiente;
}
int Comer(PEDACITOS* serpiente, int tams){
  if (serpiente[tams - 1].pos.x == com.pos.x && serpiente[tams - 1].pos.y == com.pos.y) {
		return 1;
	}
	return 0;
}

void TrazarTablero(GtkWidget * widget){
  if(!muerto){
    contador++;
    if(contador >= 15){
      GenerarComida(widget);
      contador = 0;
    }
    if(!MoverSerpiente(serpiente, serpiente[tams - 1].dir , widget, tams)){
      
    }
    if(Comer(serpiente, tams)){
      serpiente = AjustarSerpiente(serpiente, &tams, com.tipo, widget);
      com.tipo = NADA;
    }
  }
  DibujarSerpiente(widget);
}

void GenerarComida(GtkWidget * widget){
  GtkWidget *tablero = lookup_widget(widget, "drawingarea");
  if(rand() % 100 < 80){
    com.tipo = CRECE;
  }else{
    com.tipo = ACHICA;
  }
  com.pos.x = rand() % tablero->allocation.width / TAMSERP;
  com.pos.y = rand() % tablero->allocation.height / TAMSERP;
}