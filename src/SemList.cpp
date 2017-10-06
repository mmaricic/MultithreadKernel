#include "SemList.h"
#include "kersem.h"
#include "PCB.h"
#include "kernel.h"

SemaphoreList::SemaphoreList(){
	first = 0;
	last = 0;
}


void SemaphoreList::add(KernelSem* sem){

	
	Elem* temp = new Elem(sem);
	if (first == 0)
		first = last = temp;
	else
		last = last->next = temp;

}

void SemaphoreList::remove(KernelSem* sem){
	
	Elem* temp = first, *prev = 0;
	while(temp && temp->elem != sem){
		prev = temp;
		temp = temp->next;
	}
	
	if(temp == first)
		first = first->next;
	
	else if(temp == last) {
		last = prev;
		last->next = 0;
	}
	
	else prev->next = temp->next;
	
	if (first == 0)
		last = 0;
	
	delete temp;
}


SemaphoreList::~SemaphoreList(){
	while(first != 0){
		Elem* temp = first;
		first = first->next;
		delete temp;
	}
	last = 0;
}
