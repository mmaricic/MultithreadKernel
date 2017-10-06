#include "kernelev.h"
#include "evList.h"
#include "kernel.h"

EventList::EventList(){
	first = 0;
	last = 0;
}


void EventList::add(KernelEv* event){

	
	Elem* temp = new Elem(event);
	if (first == 0)
		first = temp;
	else 
		last->next = temp;
	last = temp;

}

void EventList::remove(KernelEv* event){
	
	Elem* temp = first, *prev = 0;
	while(temp && temp->event != event){
		prev = temp;
		temp = temp->next;
	}
	
	if(temp == first)
		first = first->next;
	
	else if(temp == last){
		last = prev;
		last->next = 0;
	}
	
	else prev->next = temp->next;
	
	delete temp;

}

EventList::~EventList(){
	while(first != 0){
		Elem* temp = first;
		first = first->next;
		delete temp;
	}
	last = 0;
}
