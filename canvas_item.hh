#ifndef __CANVAS_ITEM_HH
#define __CANVAS_ITEM_HH

#include "linkedlist.hh"
#include "canvas.hh"


#define POINTER_BUTTON_STATE_DOWN 1
#define POINTER_BUTTON_STATE_UP   2

struct _canvas_item_action {

  int button;
  int pointer_button_state;
  
  int data_size;
  int data_type;  
  void *data;
  void (*func);
  void *object;
  
};
typedef _canvas_item_action canvas_item_action;


class canvas_item {

public:
  /* functions */
  canvas_item(canvas *c);
  virtual	~canvas_item();
   
  void taint();
  void untaint();
  bool is_tainted();

  int get_id();
  int set_id(int i);

  int set_x(int);
  int set_y(int);
  int get_x() { return (x); };
  int get_y() { return (y); };
   
  int set_w(int);
  int set_h(int);
  int get_w() { return (w); };
  int get_h() { return (h); };
   
  void prepare();   
   virtual void render() { return(render(true)); }
   virtual void render(bool) {};

  
  // set by the canvas object. 
  void pointer_coords(int x, int y);
  void pointer_enter();
  void pointer_leave();

  void event_button_press(int);
  void event_button_release(int);
  
  bool has_pointer () { return(_has_pointer); };
  
	
void canvas_item::set_action_button_click(int button, void *object, void (*f), int data_type, void *data, int data_size);

protected:

  // EVENTS
	void event_pointer_enter();
	void event_pointer_leave();

  canvas *can;
  /* coords */
  int x, y, w, h;
  bool _has_pointer;
  int pointer_x, pointer_y;
  
  int rendered_count;
  void render_success();
  int get_rendered_count() { return(rendered_count); };

  mouse_state *mouse;
  LinkedList<canvas_item_action> *action_list;
  LinkedListIterator<canvas_item_action> it;

private:
   
   
  int id;
  bool tainted;
   
   
  
};


#endif // __CANVAS_ITEM_HH
