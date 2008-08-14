#ifndef __CANVAS_BUTTON_IMAGE_HH
#define __CANVAS_BUTTON_IMAGE_HH

#include "canvas_item.hh"
#include "linkedlist.hh"
#include "canvas_image.hh"
#include "canvas_button_x11.hh"




class canvas_button_image : public canvas_item {

public:
  
  /* functions */
  canvas_button_image(canvas *c); 
  ~canvas_button_image();
   
  void prepare();
  void render(bool);

  void event_pointer_enter();
  void event_pointer_leave();

  void event_button_press(int);
  void event_button_release(int);
  
  bool set_image_normal(char *c);
  bool set_image_hover(char *c);
  bool set_image_active(char *c);

private:

  int ignore_exposure;
  
  void draw_image(char *s);
  void render_normal();
  void render_active();
  void render_hover();

  char image_normal[MAX_IMAGE_PATH_LENGTH];
  char image_hover[MAX_IMAGE_PATH_LENGTH];
  char image_active[MAX_IMAGE_PATH_LENGTH];

//  LinkedList<canvas_item_action> *action_list;
//  LinkedListIterator<canvas_button_action> it;
  
  void *data;
  
};


#endif // __CANVAS_BUTTON_IMAGE_HH
