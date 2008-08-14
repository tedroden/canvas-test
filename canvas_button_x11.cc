
#include "canvas_item.hh"
#include "canvas_button_x11.hh"
#include "canvas.hh"

#include <iostream>

canvas_button_x11::canvas_button_x11(canvas *c) :
  canvas_item (c) {

  memset(&color_state_normal, 0, MAX_COLOR_NAME_LENGTH);
  memset(&color_state_hover, 0, MAX_COLOR_NAME_LENGTH);
  memset(&color_state_active, 0, MAX_COLOR_NAME_LENGTH);
  memset(&color_state_normal_bg, 0, MAX_COLOR_NAME_LENGTH);
  memset(&color_state_hover_bg, 0, MAX_COLOR_NAME_LENGTH);
  memset(&color_state_active_bg, 0, MAX_COLOR_NAME_LENGTH);
  
  set_color_state_normal("Blue");
  set_color_state_hover("Yellow");
  set_color_state_active("Red");

  set_color_state_normal_bg(can->get_color_background());
  set_color_state_hover_bg(can->get_color_background());
  set_color_state_active_bg(can->get_color_background());



}

void canvas_button_x11::prepare() {
  std::cout << "canvas_button_x11::prepare()" << std::endl;
}

bool canvas_button_x11::set_color_state_normal(char *c) {
  memset(&color_state_normal, 0, MAX_COLOR_NAME_LENGTH);
  memcpy(&color_state_normal, c, strlen(c));  
  return(true);
}

bool canvas_button_x11::set_color_state_normal_bg(char *c) {
  memset(&color_state_normal_bg, 0, MAX_COLOR_NAME_LENGTH);
  memcpy(&color_state_normal_bg, c, strlen(c));
  return(true);
}

bool canvas_button_x11::set_color_state_active(char *c) {
  memset(&color_state_active, 0, MAX_COLOR_NAME_LENGTH);
  memcpy(&color_state_active, c, strlen(c));  
  return(true);
}

bool canvas_button_x11::set_color_state_active_bg(char *c) {
  memset(&color_state_active_bg, 0, MAX_COLOR_NAME_LENGTH);
  memcpy(&color_state_active_bg, c, strlen(c));  
  return(true);
}

bool canvas_button_x11::set_color_state_hover(char *c) {
  memset(&color_state_hover, 0, MAX_COLOR_NAME_LENGTH);
  memcpy(&color_state_hover, c, strlen(c));  
  return(true);
}

bool canvas_button_x11::set_color_state_hover_bg(char *c) {
  memset(&color_state_hover_bg, 0, MAX_COLOR_NAME_LENGTH);
  memcpy(&color_state_hover_bg, c, strlen(c));  
  return(true);
}

void canvas_button_x11::render_normal() {
  XGCValues gcv;
  XColor color_normal, gcolor;
  
  gcv.function = GXcopy;
  gcv.line_width = 1;
  
  XAllocNamedColor(can->get_display(), DefaultColormap(can->get_display(), DefaultScreen(can->get_display())),
		   color_state_normal, &color_normal, &gcolor);
  
  gcv.foreground = color_normal.pixel;

  GC std_gc = XCreateGC(can->get_display(), can->get_window(), GCLineWidth|GCFunction|GCBackground|GCForeground, &gcv);    
  XClearArea(can->get_display(), can->get_window(), x, y, w, h, false);
  
  XDrawRectangle(can->get_display(), can->get_window(), std_gc, x, y, w, h);

  if(color_state_normal_bg) {
    XGCValues bg_gcv;   
    XColor color_normal_bg;
    
    bg_gcv.function = GXcopy;
    bg_gcv.line_width = 1;
    XAllocNamedColor(can->get_display(), DefaultColormap(can->get_display(), DefaultScreen(can->get_display())),
		     color_state_normal_bg, &color_normal_bg, &gcolor);    
    
    bg_gcv.foreground = color_normal_bg.pixel;
    GC bg_gc = XCreateGC(can->get_display(), can->get_window(), GCLineWidth|GCFunction|GCBackground|GCForeground, &bg_gcv);

    XFillRectangle(can->get_display(), can->get_window(), bg_gc, x + 1, y + 1, w - 1, h - 1);

  }

}

void canvas_button_x11::render_hover() {
  XGCValues gcv;
  XColor color_hover, gcolor;;
  
  gcv.function = GXcopy;
  gcv.line_width = 1;
  
  XAllocNamedColor(can->get_display(), DefaultColormap(can->get_display(), DefaultScreen(can->get_display())),
		   color_state_hover, &color_hover, &gcolor);
  
  gcv.foreground = color_hover.pixel;
  GC std_gc = XCreateGC(can->get_display(), can->get_window(), GCLineWidth|GCFunction|GCBackground|GCForeground, &gcv);
  XClearArea(can->get_display(), can->get_window(), x, y, w, h, false);
  
  XDrawRectangle(can->get_display(), can->get_window(), std_gc, x, y, w, h);
    
  
  if(color_state_hover_bg) {
    XGCValues bg_gcv;   
    XColor color_hover_bg;
    
    bg_gcv.function = GXcopy;
    bg_gcv.line_width = 1;
    XAllocNamedColor(can->get_display(), DefaultColormap(can->get_display(), DefaultScreen(can->get_display())),
		     color_state_hover_bg, &color_hover_bg, &gcolor);    
    
    bg_gcv.foreground = color_hover_bg.pixel;
    GC bg_gc = XCreateGC(can->get_display(), can->get_window(), GCLineWidth|GCFunction|GCBackground|GCForeground, &bg_gcv);

    XFillRectangle(can->get_display(), can->get_window(), bg_gc, x + 1, y + 1, w - 1, h - 1);

  }
  
}


void canvas_button_x11::render_active() {
  XGCValues gcv;
  XColor color_active, gcolor;;
  
  gcv.function = GXcopy;
  gcv.line_width = 1;
  
  XAllocNamedColor(can->get_display(), DefaultColormap(can->get_display(), DefaultScreen(can->get_display())),
		   color_state_active, &color_active, &gcolor);
  
  gcv.foreground = color_active.pixel;
  GC std_gc = XCreateGC(can->get_display(), can->get_window(), GCLineWidth|GCFunction|GCBackground|GCForeground, &gcv);
  XClearArea(can->get_display(), can->get_window(), x, y, w, h, false);  
  XDrawRectangle(can->get_display(), can->get_window(), std_gc, x, y, w, h);
  
  if(color_state_active_bg) {
    XGCValues bg_gcv;   
    XColor color_active_bg;
    
    bg_gcv.function = GXcopy;
    bg_gcv.line_width = 1;
    XAllocNamedColor(can->get_display(), DefaultColormap(can->get_display(), DefaultScreen(can->get_display())),
		     color_state_active_bg, &color_active_bg, &gcolor);    
    
    bg_gcv.foreground = color_active_bg.pixel;
    GC bg_gc = XCreateGC(can->get_display(), can->get_window(), GCLineWidth|GCFunction|GCBackground|GCForeground, &bg_gcv);

    XFillRectangle(can->get_display(), can->get_window(), bg_gc, x + 1, y + 1, w - 1, h - 1);
  }
}

void canvas_button_x11::render(bool force) {
  
  if(mouse->button1_down == true ||
     mouse->button2_down == true ||
     mouse->button3_down == true ||
     mouse->button4_down == true ||
     mouse->button5_down == true) {
    render_active();
  }
  else if(has_pointer()) {
    render_hover();
  }
  else {
    render_normal();
  }

  //  fprintf(stderr, "canvas_button_x11::render(%d, %d, %d, %d)\n", x, y, w, h);
  render_success();
}





