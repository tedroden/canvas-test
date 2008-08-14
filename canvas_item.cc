
#include "canvas_item.hh"
#include "canvas_item.hh"
#include <iostream>

canvas_item::canvas_item(canvas *c) {
  x = y = w = h = 0;
  id = 0;
  tainted = false;
  can = c;
  rendered_count = 0;
  _has_pointer = false;

  mouse = (mouse_state *) malloc(sizeof(mouse_state));
  mouse->moving = 
    mouse->button1_down = 
    mouse->button2_down = 
    mouse->button3_down = 
    mouse->button4_down = 
    mouse->button5_down =
    false;
  
  mouse->click_x = 
    mouse->click_y =
    mouse->current_x = 
    mouse->current_y = 
    mouse->last_x = 
    mouse->last_y =
    0;
  
  action_list = new LinkedList<canvas_item_action>; 
  it.setList(action_list);

}

canvas_item::~canvas_item() {
  free(mouse);

  if( action_list->count() )  {    
    it.reset(); 
    for(; it.current(); it++) {
      free(it.current()->data);
      free(it.current());
    }
  }
}


void canvas_item::pointer_coords(int _x, int _y) {
  pointer_x = _x;
  pointer_y = _y;

  if(!_has_pointer) 
    pointer_enter();
}


void canvas_item::event_button_press(int button) {

  mouse->button1_down =
    mouse->button2_down =
    mouse->button3_down =
    mouse->button4_down =
    mouse->button5_down =
    false;
  
  switch(button) {
  case 1:
    mouse->button1_down = true;
    break;
  case 2:
    mouse->button2_down = true;
    break;
  case 3:
    mouse->button3_down = true;
    break;
  case 4:
    mouse->button4_down = true;
    break;
  case 5:
    mouse->button5_down = true;
    break;        
  }
  
  
  if( action_list->count() )  {    
    it.reset();
    
    for(; it.current(); it++) {
      if(it.current()->button == button) {
	it.current()->pointer_button_state = POINTER_BUTTON_STATE_DOWN;
      }
    }
  }
  
	
  render(true);
//  fprintf(stderr, "canvas_button_x11::event_button_press(%d)\n", button);
}

void canvas_item::event_button_release(int button) {

  mouse->button1_down =
    mouse->button2_down =
    mouse->button3_down =
    mouse->button4_down =
    mouse->button5_down =
    false;
  
  // pointer click action


  if( action_list->count() )  {    
    it.reset(); 
    for(; it.current(); it++) {

      // if the button is currently pressed, we've successfully clicked.
      if(it.current()->pointer_button_state  == POINTER_BUTTON_STATE_DOWN) {
	
//	it.current()->pointer_button_state  == POINTER_BUTTON_STATE_UP;
	
	// FIXME: We have an object,  (not working)
	if(it.current()->object) {
	  void (*func) (void *, void *);
	  func = (void (*)(void *, void *)) it.current()->func;
	  func(it.current()->object, it.current()->data);
	}
	
	// We don't have an "object" just call the function and pass the data.
	else {
	  void (*func) (canvas_item *, void *);
	  func = (void (*)(canvas_item *, void *)) it.current()->func;
	  func(this, it.current()->data);
	}
	
      }
    }

    
    render(true);
  
  }
}

void canvas_item::set_action_button_click(int button, void *object, void (*f), 
					  int data_type, void *data, int data_size) {

  canvas_item_action *action = (canvas_item_action *) malloc(sizeof(canvas_item_action));
  action->button = button;
  action->object = object;
  action->func = f;
  action->data = malloc(data_size + 1);
  action->data_size = data_size;
  action->data_type = data_type;
  memcpy( action->data, data, data_size);
  action_list->insert(action);
}


void canvas_item::pointer_enter() {

  can->pointer_acknowledge(this);
  _has_pointer = true;
  
  event_pointer_enter();
}

void canvas_item::pointer_leave() {

  _has_pointer = false;
  event_pointer_leave();
  
  if( action_list->count() )  {    
    it.reset();
    
    for(; it.current(); it++) {
      it.current()->pointer_button_state = POINTER_BUTTON_STATE_UP;
    }
    
  }
  
  //  fprintf(stderr, "canvas_item::event_pointer_leave(%s)\n", (has_pointer() ? "HAVE POINTER!" : "NO POINTER"));
  //	render(true);	
  
  
#ifdef CANVAS_DEBUG  
  std::cout << "canvas_item::pointer_leave()" << std::endl;
#endif // CANVAS_DEBUG

}


void canvas_item::event_pointer_enter() {

	render(true);
}



void canvas_item::event_pointer_leave() {
    
  if( action_list->count() )  {
    it.reset();

    for(; it.current(); it++) {
      it.current()->pointer_button_state = POINTER_BUTTON_STATE_UP;
    }
    
  }
    
//  fprintf(stderr, "canvas_button_x11::event_pointer_leave(%s)\n", (has_pointer() ? "HAVE POINTER!" : "NO POINTER"));
 	render(true);
}


void canvas_item::prepare() {
  std::cout << "canvas_item::prepare()" << std::endl;
}

void canvas_item::render_success() {
  rendered_count++;
}

void canvas_item::taint() {
  tainted = true;
}

void canvas_item::untaint() {
  tainted = false;
}

bool canvas_item::is_tainted() {
  return(tainted);
}

int canvas_item::set_x(int _x) {
  if(x != _x) taint();
  
  x = _x;
  return(x);
}

int canvas_item::set_y(int _y) {
  if(y != _y) taint();
  
  y = _y;
  return(y);
}

int canvas_item::set_h(int _h) {
  if(h != _h) taint();
  
  h = _h;
  return(h);
}

int canvas_item::set_w(int _w) {
  if(w != _w) taint();
  
  w = _w;
  return(w);
}

int canvas_item::get_id() {
  int i = id;
  return(i);
}

int canvas_item::set_id(int i) {
  if(id != 0) {
    fprintf(stderr, "ERROR: canvas_item::set_id(): setting a new id (changing from %d to %d)!", id, i);
  }
  id = i;
  
  return(i);
}



