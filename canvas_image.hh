#ifndef __CANVAS_IMAGE_HH
#define __CANVAS_IMAGE_HH

#include "canvas_item.hh"

#define MAX_IMAGE_PATH_LENGTH 255

class canvas_image : public canvas_item {

public:
  /* functions */
  canvas_image(canvas *c); 
  ~canvas_image();
  
  void prepare();
  void render(bool);
  
  void load_image(char *file_name);
  
  void event_pointer_enter() {};
  void event_pointer_leave() {};
  
private:
  char image_path[MAX_IMAGE_PATH_LENGTH];
  char *format;
  Imlib_Image image;
  
};


#endif // __CANVAS_IMAGE_HH
