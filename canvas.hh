
#ifndef __CANVAS_HH
#define __CANVAS_HH

#define MAX_COLOR_NAME_LENGTH 100

#define CANVAS_NULL     0
#define CANVAS_STRING 	1
#define CANVAS_INT	 	2


#include <stdio.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <Imlib2.h>
#include <stdlib.h>

// #include <X11/Xproto.h>
// #include <X11/cursorfont.h>
// #include <X11/Xft/Xft.h>

#include <sys/types.h>




class canvas_item;
class canvas;



struct _mouse_state {
  bool moving;
  
  bool button1_down;
  bool button2_down;
  bool button3_down;
  bool button4_down;
  bool button5_down;
  
  int click_x, click_y;
  int current_x, current_y;
  int last_x, last_y;
  
};
typedef _mouse_state mouse_state;

#include "canvas_item.hh"
#include "events.hh"
#include "linkedlist.hh"
#include <Imlib2.h>



class canvas {

  
  
public:
  
  canvas();
  ~canvas();
   
  void render(bool);
  void render(void) { render(false); };
  
  void dump();
  
  int insert(canvas_item *i);
  int remove(int id);
  void set_size(int _x, int _y, int _w, int _h);
  
  void x11_display_init();  
  void imlib_init();
  
  
  Display *get_display() { return(dis); };
  Window get_window() {  return(win); };
  Colormap get_colormap() { return(cm); };
  Screen get_screen() { return(screen); };
  
  events *get_event_list() { return(event_list); };
  mouse_state *get_mouse() { return(mouse); }; 
  void pointer_acknowledge(canvas_item *i) { pointer_canvas_item = i; };
  bool set_color_background(char *c);
  char *get_color_background() { return(color_background); };
  
private:

  LinkedList<canvas_item> *item_list;
  LinkedListIterator<canvas_item> it;



  
  mouse_state *mouse;
  events *event_list;
  
  // X11 stuff
  Display *dis;
  Window win;
  Visual *vis;
  Colormap cm;
  Screen screen;
  int depth;
  
  canvas_item *pointer_canvas_item;
  
  char *color_background;
  XColor xcolor_color_background, gcolor;

  void x11_display_close();  	
  bool x11_display_ready;
  bool imlib2_ready;

  void taint_area(int _x, int _y, int _w, int _h);
  
  int x, y, w, h;
  bool canvas_running;
  
  void event_loop();
  void x11_event_check();
  void canvas_event_check();
  
  
  
};

#endif // __CANVAS_HH
