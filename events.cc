#include "linkedlist.hh"
#include "events.hh"
#include <stdlib.h>
#include <sys/time.h>

events::events() {
     
     _timed_events = new LinkedList<TimedEvent>; 
     _timed_events_it.setList(_timed_events);
  
}

events::~events() {

  if( _timed_events->count() )  {    
    _timed_events_it.reset(); 
    for(; _timed_events_it.current(); _timed_events_it++) {
      free(_timed_events_it.current());
    }
  }

}


void events::set_timed(int times, double interval, void *object, void (*f), void *data) {

     TimedEvent *e = (TimedEvent *) malloc(sizeof(TimedEvent));
     e->times = times;
     e->interval = interval;
     e->obj = object;
     e->func = f; 
     e->data = data;

     e->__time = interval + get_time(); 
     _timed_events->insert(e);
     
}

double events::get_time() {
  struct timeval timev;
  gettimeofday(&timev, NULL);
  return timev.tv_sec + (timev.tv_usec / 1000000.0);
}

/* this is running pretty well.
   but it's a bit hard to look at.
   i'll fix one day */
void events::run_timed(int fd) {
     double t = get_time();
     fd_set fdset;
     struct timeval timeout;

     // neat.. this stops this function
     // from taking up all the cpu.
     FD_ZERO(&fdset);
     FD_SET(fd, &fdset);
     timeout.tv_sec = 0;
     timeout.tv_usec =  500000;
     select (fd+1, &fdset, NULL, NULL, &timeout);
     
     
     if( _timed_events->count() )  {    
	  _timed_events_it.reset(); 
	  for(; _timed_events_it.current(); _timed_events_it++) {

	       if(_timed_events_it.current()->__time <= t) {
		    
		    
		    if(_timed_events_it.current()->times != 0) {
			      
			 /* set up next time */
			 _timed_events_it.current()->__time =
			      _timed_events_it.current()->interval + t;
			 
			 if(_timed_events_it.current()->times != -1) 
			      _timed_events_it.current()->times--;

			 void (*func) (void *);
			 func = (void (*) (void *)) _timed_events_it.current()->func;
			 
			 func(_timed_events_it.current()->obj);
			 
		    }
		    
		    else {
		      TimedEvent *te = _timed_events_it.current();
		      _timed_events->remove(te);
		      free(te);
		      break;
		    }


	       }
	  }
		  
     }


}

