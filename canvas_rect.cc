
#include "canvas_item.hh"
#include "canvas_rect.hh"
#include "canvas.hh"

#include <X11/Xft/Xft.h>
// #include <XRender.h>
#include <iostream>

canvas_rect::canvas_rect(canvas *c) :
canvas_item (c) {

  memset(&color_outline, 0, MAX_COLOR_NAME_LENGTH);
  memset(&color_fill, 0, MAX_COLOR_NAME_LENGTH);

  set_color("White");
}

canvas_rect::~canvas_rect() {
   
}

void canvas_rect::prepare() {
  std::cout << "canvas_rect::prepare()" << std::endl;
}



bool canvas_rect::set_color_outline(char *c) {
  memset(&color_outline, 0, MAX_COLOR_NAME_LENGTH);
  memcpy(&color_outline, c, strlen(c));  
  return(true);
}

bool canvas_rect::set_color_fill(char *c) {
  memset(&color_fill, 0, MAX_COLOR_NAME_LENGTH);
  memcpy(&color_fill, c, strlen(c));  
  return(true);
}

bool canvas_rect::set_color(char *c) {
  set_color_outline(c);
  set_color_fill(c);
  return(true);
}

void canvas_rect::render(bool force ) {
  
  XGCValues gcv;
  XColor xcolor_color_outline, gcolor;
  
  gcv.function = GXcopy;
  gcv.line_width = 1;
  
  XAllocNamedColor(can->get_display(), DefaultColormap(can->get_display(), DefaultScreen(can->get_display())),
		   color_outline, &xcolor_color_outline, &gcolor);
  
  gcv.foreground = xcolor_color_outline.pixel;
  
  GC std_gc = XCreateGC(can->get_display(), can->get_window(), GCLineWidth|GCFunction|GCBackground|GCForeground, &gcv);    
  XClearArea(can->get_display(), can->get_window(), x, y, w, h, false);

 // fprintf(stderr, "canvas_rect::render(%d, %d, %d, %d) %s, %s\n", x, y, w, h, color_outline, color_fill);
  
  XDrawRectangle(can->get_display(), can->get_window(), std_gc, x, y, w, h);

  if(color_fill) {
    XColor xcolor_color_fill;
    XAllocNamedColor(can->get_display(), DefaultColormap(can->get_display(), DefaultScreen(can->get_display())),
		     color_fill, &xcolor_color_fill, &gcolor);    
    gcv.foreground = xcolor_color_fill.pixel;    
    XFillRectangle(can->get_display(), can->get_window(), std_gc, x + 1, y + 1, w, h);
  }  
  
  //  fprintf(stderr, "canvas_rect::render(%d, %d, %d, %d)\n", x, y, w, h);
  render_success();

}





