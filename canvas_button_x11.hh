#ifndef __CANVAS_BUTTON_X11_HH
#define __CANVAS_BUTTON_X11_HH

#include "canvas_item.hh"
#include "linkedlist.hh"



#define POINTER_BUTTON_STATE_DOWN 1
#define POINTER_BUTTON_STATE_UP   2


class canvas_button_x11 : public canvas_item {

public:
  
  /* functions */
  canvas_button_x11(canvas *c); 
 // ~canvas_button_x11();
   
  void prepare();
  void render(bool);

  
  bool set_color_state_normal(char *c);
  bool set_color_state_normal_bg(char *c);
  bool set_color_state_hover(char *c);
  bool set_color_state_hover_bg(char *c);
  bool set_color_state_active(char *c);
  bool set_color_state_active_bg(char *c);


//  void set_action_button_click(int button, void *object, void (*f), void *data);
  

private:

  void render_normal();
  void render_active();
  void render_hover();

  char color_state_normal[MAX_COLOR_NAME_LENGTH];
  char color_state_normal_bg[MAX_COLOR_NAME_LENGTH];
  char color_state_hover[MAX_COLOR_NAME_LENGTH];
  char color_state_hover_bg[MAX_COLOR_NAME_LENGTH];
  char color_state_active[MAX_COLOR_NAME_LENGTH];
  char color_state_active_bg[MAX_COLOR_NAME_LENGTH];

  void *data;

};


#endif // __CANVAS_BUTTON_X11_HH
