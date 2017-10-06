#ifndef _eventlist_h_
#define _eventlist_h_

class KernelEv;

class EventList{
private:
	struct Elem{
		
		KernelEv* event;
		Elem* next;
		Elem(KernelEv* kerev){
			event = kerev; 
			next = 0;
		}
	};
	
	Elem* first, *last;

	public:
	EventList();
	void add(KernelEv *event);
	void remove(KernelEv *event);
	~EventList();
};

#endif