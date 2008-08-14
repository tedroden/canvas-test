
#include "canvas.hh"
#include "canvas_button_x11.hh"
#include "canvas_button_image.hh"
#include "canvas_rect.hh"
#include "canvas_image.hh"
#include <stdio.h>
#include <string>

canvas *can = new canvas();

void button1_press(canvas_item *but, void *_str) {
//  char *str = (char *) but;
  char *str = (char *) _str;

  if(!str) {
    fprintf(stderr, "button1_press(), no color!! using 'Red'\n");
    str = "Red";
  }
  
  fprintf(stderr, "button1_press(%s)\n", str);
  
  canvas_rect *button = new canvas_rect(can);
  button->set_x( 100 );
  button->set_y( 100 );
  button->set_h( 300 );
  button->set_w( 300 );
  button->set_color(str);
  button->render(true);
  can->insert(button);
  
  
}


void draw_image(canvas_item *bt, void *_str) {

  canvas_image *image = new canvas_image(can);
  image->load_image((char *) _str);
  image->set_x(100);
  image->set_y(100);
  image->set_w ( 300 );
  image->set_h ( 300 );
  image->render(true);
  can->insert(image);

  
}

void button_exit(void *garbage) {

  delete can;
  fprintf(stderr, "Exiting Cleanly\n");
  exit(0);
  
}

void button3_press(void *but) {
  char *str = (char *) but;


  if(!str) {
    fprintf(stderr, "button3_press(), no color!! using 'Red'\n");
    str = "Red";
  }

  
  fprintf(stderr, "button3_press(%s)\n", str);
  
  
  
}


int main(int argc, void **argv) {

  int i = 0;
  int last_width, last_x;
  int border_size = 10;

  int width = 25;
  int height = 25;

  last_width = last_x = 0;
  
  can->set_size(0, 0, 640, 480);
  can->set_color_background("DarkSlateGrey");  
  can->x11_display_init();
  can->imlib_init();
  



  char *colors[] =
    {
      "DarkGreen",
      "SlateBlue",
      "GoldenRod",
      "MediumSpringGreen",
      "seashell1",
      "DeepSkyBlue1",
      "salmon1",
      "LavenderBlush",
      "honeydew2",
      "DeepPink"
    };
  
  for(i = 0; i < 10; ++i) {
    /* button1 */
    
    canvas_button_x11 *button1 = new canvas_button_x11(can);
    
    button1->set_x(last_width + last_x + border_size);
    button1->set_y(10);
    button1->set_w(width);
    button1->set_h(height);
    button1->set_color_state_normal(colors[i]);
    button1->set_color_state_hover_bg(colors[i]);
    button1->set_action_button_click(1, (void *) NULL, (void *)&button1_press, CANVAS_STRING, (void *) colors[i], strlen(colors[i]));
    button1->set_action_button_click(3, (void *) NULL, (void *)&button3_press, CANVAS_STRING, (void *) colors[i], strlen(colors[i]));

    last_width = button1->get_w();
    last_x = button1->get_x();
    
    can->insert(button1);
  }
  
  canvas_button_x11 *but = new canvas_button_x11(can);
  but->set_x(last_width + last_x + border_size);
  but->set_y(10);
  but->set_w(width);
  but->set_h(height);
  but->set_color_state_normal("Red");
  but->set_color_state_hover_bg("Red");
//  but->set_action_button_click(1, (void *) NULL, (void *)&draw_image, (void *) NULL);

  but->set_action_button_click(1, (void *) NULL, (void *)&button1_press, CANVAS_STRING, (void *) "Red", strlen("Red"));
  last_width = but->get_w();
  last_x = but->get_x();
  can->insert(but);


  
    
  
  /*  
  canvas_button_x11 *button2 = new canvas_button_x11(can);
  button2->set_color_state_normal("SlateBlue");
  button2->set_color_state_hover_bg("SlateBlue");
  button2->set_action_button_click(1, (void *) NULL, (void *)&button1_press, (void *) "SlateBlue");
  button2->set_x(50 + 25 + 10);
  button2->set_y(10);
  button2->set_w(25);
  button2->set_h(25);
  can->insert(button2);


  canvas_button_x11 *button3 = new canvas_button_x11(can);
  button3->set_color_state_normal("LightGoldenRod");
  button3->set_color_state_hover_bg("LightGoldenRod");
  button3->set_action_button_click(1, (void *) NULL, (void *)&button1_press, (void *) "LightGoldenRod");
  button3->set_x(button2->get_x() + button2->get_h() + 10);
  button3->set_y(10);
  button3->set_w(25);
  button3->set_h(25);
  can->insert(button3);

  */

  
  canvas_button_image *image_button = new canvas_button_image(can);
  image_button->set_image_normal("./button-normal.png");
  image_button->set_image_hover("./button-hover.png");
  image_button->set_image_active("./button-active.png");
  char *img = "./superman.gif";
  //image_button->set_action_button_data(CANVAS_STRING, "./sun.jpg", strlen("./sun.jpg") + 1);
  image_button->set_action_button_click(1, (void *) NULL, 
					(void *)&draw_image, CANVAS_STRING, (void *) img, strlen(img));
								
 // image_button->set_action_button_click(1, (void *) NULL, (void *)&button1_press, (void *) "Orange");
  image_button->set_x(400);
  image_button->set_y(10);
  image_button->set_w(25);
  image_button->set_h(25);
  can->insert(image_button);
  
  
  canvas_button_image *exit_button = new canvas_button_image(can);
  exit_button->set_image_normal("./button-close.png");
  exit_button->set_image_hover("./button-close.png");
  exit_button->set_image_active("./button-close.png");

  exit_button->set_action_button_click(1, (void *) NULL, (void *)&button_exit, CANVAS_NULL, (void *) NULL, 0);
  exit_button->set_x(550);
  exit_button->set_y(10);
  exit_button->set_w(25);
  exit_button->set_h(25);
  can->insert(exit_button);
  
  can->render();
//  can->dump();
  
}

