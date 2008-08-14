#ifndef __EVENTS_HH
#define __EVENTS_HH


#include "linkedlist.hh"

struct _timed_event {	
     double interval;
     int times;
     int type;
     void *data;

     void (*func);
     void *obj;
     
     double __time;  
};
typedef struct _timed_event TimedEvent;

class events {

  
  
public:
     events();
     ~events();
     
     void set_timed(int times, double interval,   
		    void *object, void (*f), void *data);
     
     void run_timed(int);
  
private:
  double events::get_time();
  
     LinkedList<TimedEvent> *_timed_events;
     LinkedListIterator<TimedEvent> _timed_events_it;

     double last_time;
};
#endif //  __EVENTS_HH
