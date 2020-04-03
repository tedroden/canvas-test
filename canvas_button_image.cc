
#include "canvas_item.hh"
#include "canvas_button_image.hh"
#include "canvas_image.hh"
#include "canvas.hh"

#include <cstring>
#include <iostream>

canvas_button_image::canvas_button_image(canvas *c) :
  canvas_item (c) {

  memset(&image_normal, 0, MAX_IMAGE_PATH_LENGTH);
  memset(&image_active, 0, MAX_IMAGE_PATH_LENGTH);
  memset(&image_hover, 0, MAX_IMAGE_PATH_LENGTH);
  
  ignore_exposure = 4;
  
}

canvas_button_image::~canvas_button_image() {
  
}



void canvas_button_image::prepare() {
  std::cout << "canvas_button_image::prepare()" << std::endl;
}


void canvas_button_image::event_pointer_enter() {
  fprintf(stderr, "canvas_button_image::event_pointer_enter()\n");
  render(true);
}

void canvas_button_image::event_pointer_leave() {

  if( action_list->count() )  {    
    it.reset();
    
    for(; it.current(); it++) {
      it.current()->pointer_button_state = POINTER_BUTTON_STATE_UP;
    }
    
  }
  
  fprintf(stderr, "canvas_button_image::event_pointer_leave()\n");
  render(true);
}


void canvas_button_image::event_button_press(int button) {


  if( action_list->count() )  {    
    it.reset();
    
    for(; it.current(); it++) {
      if(it.current()->button == button) {
	it.current()->pointer_button_state = POINTER_BUTTON_STATE_DOWN;
      }
    }
  }
  
	
  render(true);
  fprintf(stderr, "canvas_button_image::event_button_press(%d)\n", button);
}


      
      
void canvas_button_image::event_button_release(int button) {

  // pointer click action


  if( action_list->count() )  {    
    it.reset(); 
    for(; it.current(); it++) {

      // if the button is currently pressed, we've successfully clicked.
      if(it.current()->pointer_button_state  == POINTER_BUTTON_STATE_DOWN) {
	//it.current()->pointer_button_state  == POINTER_BUTTON_STATE_UP;
	
	// FIXME: We have an object,  (not working)
	if(it.current()->object) {
	  void (*func) (void *, void *);
	  func = (void (*)(void *, void *)) it.current()->func;
	  func(it.current()->object, it.current()->data);
	}
	
	// We don't have an "object" just call the function and pass the data.
	else {
	  void (*func) (void *);
	  func = (void (*)(void *)) it.current()->func;
	  func(it.current()->data);
	}
      }
    }

    
    render(true);
  
  }
}



bool canvas_button_image::set_image_normal(char *c) {
  memset(&image_normal, 0, MAX_IMAGE_PATH_LENGTH);
  memcpy(&image_normal, c, strlen(c));  
  return(true);
}

bool canvas_button_image::set_image_active(char *c) {
  memset(&image_active, 0, MAX_IMAGE_PATH_LENGTH);
  memcpy(&image_active, c, strlen(c));  
  return(true);
}

bool canvas_button_image::set_image_hover(char *c) {
  memset(&image_hover, 0, MAX_IMAGE_PATH_LENGTH);
  memcpy(&image_hover, c, strlen(c));  
  return(true);
}

void canvas_button_image::draw_image(char *path) {
  Imlib_Color_Range range = imlib_create_color_range();
  imlib_context_set_color_range(range);
  Imlib_Image image = imlib_load_image(path);
  imlib_context_set_image(image);
  imlib_render_image_on_drawable_at_size(x, y, w, h);
}

void canvas_button_image::render_normal() {
  draw_image(image_normal);
}

void canvas_button_image::render_hover() {
  draw_image(image_hover);  
}

void canvas_button_image::render_active() {
  draw_image(image_active);
}

void canvas_button_image::render(bool force ) {

  
  if(!ignore_exposure) {
    ignore_exposure = 2;
    XClearArea(can->get_display(), can->get_window(), x, y, w, h, false);
  }
  
//  else {
    ignore_exposure--;
    if(can->get_mouse()->button1_down == true ||
       can->get_mouse()->button2_down == true ||
       can->get_mouse()->button3_down == true ||
       can->get_mouse()->button4_down == true ||
       can->get_mouse()->button5_down == true) {
      render_active();
    }
    else if(has_pointer()) {
      render_hover();
    }
    else {
      render_normal();
    }

    //  fprintf(stderr, "canvas_button_image::render(%d, %d, %d, %d)\n", x, y, w, h);
    render_success();
 //}
  
}





