#include "Queue.h"
#include "PCBlist.h"
#include "PCB.h"
#include "kernel.h"
#include "slpglist.h"

Queue::Queue(){
	first = 0;
	last = 0;
}


PCB* Queue::get(){
	PCB* temp = 0;
	if (first != 0){
		temp = first->thread;
		Elem* old = first;
		first = first->next;
		if (first == 0) 
			last = 0;
		
		if (old->sleeping > 0) 
			Kernel::sleeping_list->remove(old->thread);
		
		delete old;
	}
	return temp;
}

void Queue::remove(PCB* pcb){
	Elem* temp = first, *old = 0;
	while (temp && temp->thread != pcb){
		old = temp;
		temp = temp->next;
	}

	if (temp == 0)
		return;
	
	if (old == 0)
		first = first->next;
	else 
		old->next = temp->next;
	
	if (temp->next == 0) 
		last = old;

	if (first == 0) 
		last = 0;
	
	delete temp;
}


void Queue::put(PCB* pcb, int sleepingTime){
	Elem *temp = new Elem(pcb, sleepingTime);
	if (first == 0) 
		first = temp;
	else 
		last->next = temp;
	
	last = temp;
}

Queue::~Queue(){
	while(first != 0){
		Elem* temp = first;
		first = first->next;
		delete temp;
	}
	last = 0;
}