
#include "linkedlist.hh"
#include "events.hh"
#include "canvas.hh"
#include "canvas_rect.hh"

#include <stdlib.h>
#include <string.h>

using namespace std;




canvas::canvas() {
  
  
  item_list = new LinkedList<canvas_item>; 
  it.setList(item_list);
  
  x = y = 0;
  w = h = 10;

  x11_display_ready = false;
  imlib2_ready = false;
  
  event_list = new events();
  
  canvas_running = true;
  
  set_color_background("White");
  
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

  
}

canvas::~canvas() {
  
  if(color_background) free(color_background);
  XDestroyWindow(dis, win);
  
  x11_display_close();
  canvas_running = false;

    
  if(item_list->count())  {

    it.reset(); 
    for(; it.current(); it++) {
      delete it.current();
    }
  }
  
  fprintf(stderr, "canvas shutting down!\n");
  fflush(stderr);  
}

bool canvas::set_color_background(char *c) {
  if(color_background) free(color_background);
  color_background = (char *) malloc(strlen(c));
  sprintf(color_background, "%s", c);
  return(true);
}

void canvas::set_size(int _x, int _y, int _w, int _h) {
  x = _x;
  y = _y;
  h = _h;
  w = _w;
  
//  XMoveResizeWindow(dis, win, x, y, w, h);
#ifdef CANVAS_DEBUG
  fprintf(stderr, "set_size(%d, %d, %d, %d)\n", _x, _y, _w, _h);
#endif   
}
  

void canvas::taint_area(int _x, int _y, int _w, int _h) {
  // FIXME: Add this to an event queue. no need to do it right away.

  //  fprintf(stderr, "taint_area(x: %d, y: %d, w: %d, h: %d)\n", _x, _y, _w, _h);
  
  int x1 = _x;
  int x2 = _x + _w;
  int y1 = _y;
  int y2 = _y + _h;

    XGrabServer(get_display());
  if(item_list->count())  {
    
    it.reset(); 
    for(; it.current(); it++) {

      int i_x1 = it.current()->get_x();
      int i_x2 = (it.current()->get_x() + it.current()->get_w());

      int i_y1 = it.current()->get_y();
      int i_y2 = (it.current()->get_y() + it.current()->get_h());

      bool taint_y = false;
      bool taint_x = false;
      //	((y1 <= i_y2 && y2 >= i_y1));

      //      fprintf(stderr, "taint_area(x1: %d, y1: %d, x2: %d, y2: %d)\n", x1, y1, x2, y2);


      if(((y1 >= i_y1 && y1 <= i_y2) ||
	  (y1 <= i_y1 && y2 >= i_y1)))
	  taint_y = true;

      //      if(x1 >= i_x1 && x1 <= i_x2)	
      //taint_x = true;
      
      if(((x1 >= i_x1 && x1 <= i_x2) ||
	  (x1 <= i_x1 && x2 >= i_x1)))
	taint_x = true;

      //      if(taint_x == true)
      //	fprintf(stderr, "TAINT_X\n");
      
      //      if(taint_y == true)
      //	fprintf(stderr, "TAINT_Y\n");
      
      if(taint_x || taint_y) {
	//	fprintf(stderr, "TAINTED!\n");
	it.current()->render();
      }
    }
  }
  
  XUngrabServer(get_display());
  /*
  canvas_rect *i =  new canvas_rect(this);
  i->set_x(_x);
  i->set_y(_y);
  i->set_w(_w);
  i->set_h(_h);
  i->render(false);
  insert(i);
  */
}

void canvas::event_loop() {
  
  for(;;) {
    x11_event_check();
    canvas_event_check();

    // check here, because if we wait until 
    // after the run_tiemd, it's going to cause a delay
    if(!canvas_running) {
      break;
    }
    event_list->run_timed(ConnectionNumber(dis));    
  }
  
  delete this;
  
}

void canvas::canvas_event_check() {

  if(pointer_canvas_item) {
    if(!((pointer_canvas_item->get_x() < mouse->current_x) &&
	 (pointer_canvas_item->get_y() < mouse->current_y) &&
	 ((pointer_canvas_item->get_x() + pointer_canvas_item->get_w()) >= mouse->current_x) &&	 
	 ((pointer_canvas_item->get_y() + pointer_canvas_item->get_h()) >= mouse->current_y))) {
      
#ifdef CANVAS_DEBUG      
      fprintf(stderr, "x: %d, y: %d, x2: %d, y2: %d, mx: %d, my: %d\n",
	      pointer_canvas_item->get_x(),
	      pointer_canvas_item->get_y(),
	      (pointer_canvas_item->get_w() + pointer_canvas_item->get_x()),
	      (pointer_canvas_item->get_h() + pointer_canvas_item->get_y()),
	      mouse->current_x,
	      mouse->current_y);
#endif // CANVAS_DEBUG
      
      pointer_canvas_item->pointer_leave();
      pointer_canvas_item = NULL;
    }
  }

  
    
  if(item_list->count())  {
    
    it.reset(); 
    for(; it.current(); it++) {

      if((it.current()->get_x() <= mouse->current_x) &&
	 (it.current()->get_y() <= mouse->current_y) &&
	 ((it.current()->get_x() + it.current()->get_w()) >= mouse->current_x) &&	 
	 ((it.current()->get_y() + it.current()->get_h()) >= mouse->current_y)) {

	it.current()->pointer_coords(mouse->current_x - it.current()->get_x(),
				     mouse->current_y - it.current()->get_y());
	
      }
    }
    
  }
    
  
  /*
  // Check for drag.
  if(mouse->button1_down == true ||
     mouse->button2_down == true ||
     mouse->button3_down == true ||
     mouse->button4_down == true ||
     mouse->button5_down == true) {
    
    XGCValues gcv;
    
    gcv.function = GXcopy;
    gcv.line_width = 1;
    
    if(mouse->button1_down == true)
      gcv.foreground = color_light_grey.pixel;
    else if(mouse->button3_down == true)
      gcv.foreground = color_blue.pixel;
    
    
    
    gcv.subwindow_mode = IncludeInferiors;
    
    GC std_gc = XCreateGC(dis, win, GCLineWidth|GCSubwindowMode|GCFunction|GCForeground, &gcv);

    //XDrawPoint(dis, win, std_gc, mouse->current_x, mouse->current_y);
    if(mouse->last_x && mouse->last_y)
      XDrawLine(dis, win, std_gc, mouse->last_x, mouse->last_y, mouse->current_x, mouse->current_y);
    else
      XDrawPoint(dis, win, std_gc, mouse->current_x, mouse->current_y);
    
  }

  */
  
  
}

void canvas::x11_event_check() {
  XEvent ev;
  Atom deleteAtom;
  do {
    
    XNextEvent(dis, &ev);
    switch (ev.type) {
      
      
	case ClientMessage:
	  
	  deleteAtom = XInternAtom(dis, "WM_DELETE_WINDOW", 0);
      if((Atom) ev.xclient.data.l[0] == deleteAtom) {
		canvas_running = false;
      }
      else {
		fprintf(stderr, "Not a delete, it's format is  %d!\n", ev.xclient.format);
		fprintf(stderr, "Not a delete, it's format is  %ld!\n", ev.xclient.data.l[0]);
      }
      break;

	case PropertyNotify:
      break;
      
	case UnmapNotify:
      break;
	case DestroyNotify:
      fprintf(stderr, "DestroyNotify\n");
      fflush(stderr);
	  //      delete (this);
      break;
      
	case Expose:
       
	  taint_area(ev.xexpose.x, ev.xexpose.y, ev.xexpose.width, ev.xexpose.height);
       
	  break;
      
	case ButtonPress:
      mouse->click_x = ev.xbutton.x;
      mouse->click_y = ev.xbutton.y;

      
      switch(ev.xbutton.button) {
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


      if(pointer_canvas_item) {
		pointer_canvas_item->event_button_press(ev.xbutton.button);
      }	
      

      fprintf(stderr, "ButtonPress (button: %d) (%d, %d)\n", 
			  ev.xbutton.button, mouse->click_x, mouse->click_y);
      break;
      
	case ButtonRelease:

       
      switch(ev.xbutton.button) {
	  case 1:
		mouse->button1_down = false;
		break;
      case 2:
		mouse->button2_down = false;
		render(true);
		break;
	  case 3:
		mouse->button3_down = false;
		break;
	  case 4:
		mouse->button4_down = false;
		break;
	  case 5:
		mouse->button5_down = false;
		break;
      }

      if(pointer_canvas_item) {
		pointer_canvas_item->event_button_release(ev.xbutton.button);
      }
      
      break;
	case MotionNotify:
	  mouse->last_x = mouse->current_x;
	  mouse->last_y = mouse->current_y;
	  mouse->current_x = ev.xmotion.x;
	  mouse->current_y = ev.xmotion.y;      
	  //       fprintf(stderr, "MotionNotify (x: %d, y: %d)\n", mouse->current_x, mouse->current_y);
      break;      
    }
  }
  while (XPending(dis));
  
}

void canvas::x11_display_close() {
  
  // XCloseDisplay(dis);
  // 
}

void canvas::x11_display_init() {
  
  if(!x11_display_ready) {

    dis = XOpenDisplay(NULL);
    vis   = DefaultVisual(dis, DefaultScreen(dis));
    depth = DefaultDepth(dis, DefaultScreen(dis));
    cm    = DefaultColormap(dis, DefaultScreen(dis));

    
    XAllocNamedColor(dis, DefaultColormap(dis, DefaultScreen(dis)), 
		     color_background, &xcolor_color_background, &gcolor);
#ifdef CANVAS_DEBUG  
    fprintf(stderr, "CANVAS x11_display_init(): x: %d, y: %d, w: %d, h: %d\n", x, y, w, h);
#endif     
    XSetWindowAttributes swa;
    swa.background_pixel = xcolor_color_background.pixel;
    swa.colormap = cm;
    swa.backing_store = Always;

    win = XCreateWindow(dis, DefaultRootWindow(dis), x, y, w, h, 0, 
			DefaultDepth(dis, DefaultScreen(dis)), InputOutput, vis,
			(CWBackPixel | CWColormap | CWBackingStore),
			&swa);
			
//    win = XCreateSimpleWindow(dis, DefaultRootWindow(dis), x, y, w, h, 0, 0, 
//			      xcolor_color_background.pixel);
    // FIXME: more inputs!
    XSelectInput(dis, win, 
		 (ButtonReleaseMask | 
		  ButtonPressMask |
		  PropertyChangeMask |
		  
		  StructureNotifyMask | 
		  SubstructureNotifyMask | 
		  ButtonReleaseMask | 
		  PointerMotionMask |
		  ExposureMask)
		 );
    

    Atom deleteAtom = XInternAtom(dis, "WM_DELETE_WINDOW", 0); 
    XSetWMProtocols(dis, win, &deleteAtom, 1);     
    
    XMapWindow(dis, win);
    
    x11_display_ready = true;
  }   
  
}


int canvas::insert(canvas_item *i) {
  
  int __id = 0;
  
  if(!i->get_id()) {
    
    if(item_list->count())  {
      
      it.reset(); 
      for(; it.current(); it++) {
	__id = (it.current()->get_id() + 1);
      }
    
    }
    else {
      __id = 1;

    }
    
  }

  i->set_id(__id);
  item_list->insert(i);
  return(__id);
}


void canvas::render(bool force) {
  
  
  fprintf(stderr, "canvas::render()\n");
  x11_display_init();
  imlib_init();
    
  
  if(item_list->count())  {
    
    it.reset(); 
    for(; it.current(); it++) 
      it.current()->render(force);
    
  }
  
  event_loop();

  
}


void canvas::imlib_init() {
  if(!imlib2_ready) {
    // FIXME: EH? 
    imlib_set_cache_size(2048 * 1024);
    // FIXME: EH? 
    imlib_set_font_cache_size(512 * 1024);
    imlib_set_color_usage(128);
    imlib_context_set_dither(1);
    
    imlib_context_set_display(dis);
    imlib_context_set_visual(vis);
    imlib_context_set_colormap(cm);
    imlib_context_set_drawable(win);
    imlib2_ready = true;
  }
}

void canvas::dump() {
  
  fprintf(stderr, "Dumping Canvas!\n");
  if(item_list->count())  {
    
    it.reset(); 
    for(; it.current(); it++) {
      canvas_item *item = it.current();
      fprintf(stderr, "ITEM: id: %05d, x: %05d, y: %05d, h: %05d, w: %05d\n", 
	      item->get_id(), item->get_x(), item->get_y(), item->get_h(), item->get_h());
    }
    
  }
  else {
    fprintf(stderr, "Canvas item list is empty!\n");
  }
  
}
