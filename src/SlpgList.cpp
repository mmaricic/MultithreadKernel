#include "KerSem.h"
#include "PCB.h"
#include "SlpgList.h"
#include "kernel.h"
#include "schedule.h"
#include "Queue.h"

SleepingList::SleepingList(){
	first = 0; 
	last = 0; 
}


void SleepingList::add(PCB* pcb, KernelSem* semaphore, int time){
	Element *newElem = new Element(pcb, time, semaphore);
	if (first == 0){
		first = newElem;
		last = newElem;
	}

	else{

		Element *temp = first, *prev = 0;
		unsigned int currSeelpingTime = first->sleepingTime, prevSleepingTime = 0;

		while (temp && time > currSeelpingTime){
			prev = temp;
			prevSleepingTime = currSeelpingTime;
			temp = temp->next;
			if(temp)
				currSeelpingTime += temp->sleepingTime;
		}

		if (prev == 0) {
			first->sleepingTime -= time;
			newElem->next = first;
			first = newElem;
		}

		else if (temp == 0){
			newElem->sleepingTime -= prevSleepingTime;
			last->next = newElem;
			last = newElem;
		}

		else {
			newElem->next = prev->next;
			prev->next = newElem;

			newElem->sleepingTime -= prevSleepingTime;
			newElem->next->sleepingTime -= newElem->sleepingTime;
		}
	}
}

void SleepingList::reduceSleeping(){

	
	if (first != 0 && first->sleepingTime > 0)
		first->sleepingTime--;
		
		while (first != 0 && first->sleepingTime == 0){
			
			Element *temp = first;
			first = first->next;
			temp->mySem->waiting_list->remove(temp->elem);
			temp->mySem->val++;
			temp->elem->state = READY;
			temp->elem->returnType = 0;
			Scheduler::put(temp->elem);
			
			delete temp;
		}
		
		if (first == 0) 
			last = 0;
 	

}


void SleepingList::remove(PCB* pcb){
	Element* temp = first, *old = 0;
	while (temp && temp->elem != pcb){
		old = temp;
		temp = temp->next;
	}
	
	if (temp == 0)
		return;
	
	if (temp->next == 0){
		last = old;
		last->next = 0;
	}
	
	else {
		temp->next->sleepingTime += temp->sleepingTime;
		if (old == 0)
			first = first->next;
		else 
			old->next = temp->next;
	}
	
	if (first == 0) last = 0;
	delete temp;
}


SleepingList::~SleepingList(){
	while(first != 0){
		Element* temp = first;
		first = first->next;
		delete temp;
	}
	last = 0;
}
