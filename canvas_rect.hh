#ifndef __CANVAS_RECT_HH
#define __CANVAS_RECT_HH

#include "canvas_item.hh"


class canvas_rect : public canvas_item {

  public:
   /* functions */
   canvas_rect(canvas *c); 
//     : canvas_item(mc) {}
   
   ~canvas_rect();
   
   void prepare();
   void render(bool);
  void render_xft(bool);

  bool set_color_outline(char *c);
  bool set_color_fill(char *c);
  bool set_color(char *c);

  void event_pointer_enter() {};
  void event_pointer_leave() {};
  
  private:
  char color_outline[255];
  char color_fill[255];
  char *file_name;
  char *format;
   
  
  
};


#endif // __CANVAS_RECT_HH
