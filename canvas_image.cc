
#include "canvas_item.hh"
#include "canvas_image.hh"
#include "canvas.hh"
#include <string.h>

canvas_image::canvas_image(canvas *c) :
canvas_item (c) {
  

}

canvas_image::~canvas_image() {
   
}

void canvas_image::prepare() {
  fprintf(stderr, "canvas_image::prepare()\n");
}

void canvas_image::load_image(char *s) {

  memset(&image_path, 0, MAX_IMAGE_PATH_LENGTH);
  memcpy(&image_path, s, strlen(s));  

  Imlib_Color_Range range = imlib_create_color_range();
  imlib_context_set_color_range(range);
  
  image = imlib_load_image(image_path);
  imlib_context_set_image(image);
  
}



void canvas_image::render(bool force ) {

  fprintf(stderr, "canvas_image::render(%d, %d, %d, %d) %s\n", x, y, w, h, image_path);


  imlib_context_set_image(image);
  
  // Doesn't clear the area.  
  //  XClearArea(can->get_display(), can->get_window(), x, y, w, h, false);
  
  //  imlib_render_image_on_drawable_at_size(x, y, w, h);
  imlib_render_image_on_drawable_at_size(x, y, w, h);
  /*
    
  Imlib_Image img;
  imlib_context_set_display(dis);
  imlib_context_set_visual(vis);
  imlib_context_set_colormap(cm);
  imlib_context_set_drawable(win);
  
  img = imlib_load_image("/home/troden/code/slapwm/apps/fm/images/sun.jpg");
  imlib_context_set_image(img);
  imlib_render_image_on_drawable(0, 0);
  
  XFlush(dis);
  */
  
  
  render_success();

}





