#include "kernel.h"
#include "PCBlist.h"
#include "schedule.h"
#include "PCB.h"
#include "thread.h"
#include "semaphor.h"

Thread::Thread (StackSize stackSize, Time timeSlice){
	myPCB = new PCB(stackSize, timeSlice, this);
	PCBlist* temp = new PCBlist(myPCB);
	lock
	if(Kernel::PCB_head ==0)
		Kernel::PCB_head = temp;
	else Kernel::PCB_tail->next = temp;
	Kernel::PCB_tail = temp;
	unlock
	
}

void Thread::waitToComplete(){
	if(myPCB->state != FINISHED){
		lock
		Kernel::running->state = BLOCKED;
		PCBlist *temp = new PCBlist((PCB*)Kernel::running);
		temp->next = myPCB->waitToCompl;
		myPCB->waitToCompl = temp;
		unlock
		
	}
	
	dispatch();
}

ID Thread::getId() {return myPCB->threadID;}

ID Thread::getRunningId() {return Kernel::running->threadID;}

Thread* Thread::getThreadById(ID id){
	PCBlist *temp = (PCBlist*)Kernel::PCB_head;
	while(temp != 0 && temp->elem->threadID != id)
		temp = temp->next;
	
	if(temp != 0) 
		return temp->elem->myThread;
	
	return 0;
}

Thread::~Thread(){
	waitToComplete();
	
	PCBlist *temp = (PCBlist*)Kernel::PCB_head;
	PCBlist *prev = 0;
	
	lock
	while(temp != 0 && temp->elem != myPCB){
		prev = temp;
		temp = temp->next;
	}
	
	if(prev == 0) 
		Kernel::PCB_head = temp->next;
	
	else
		prev->next = temp->next;
	
	if (Kernel::PCB_head==0)
		Kernel::PCB_tail = 0;
	unlock
	
	delete temp;
	delete myPCB;
}

void Thread::start(){
	if(myPCB->state == NEW){
		myPCB->initStack();
		myPCB->state = READY;
		asm cli;
		Scheduler::put(myPCB);
		asm sti;
	}
}

void dispatch(){

	asm cli;
	Kernel::explicitContextChange = 1;
	Kernel::timer();
	asm sti;
}



ID 	Thread::fork(){
	PCB::fork();
	return Kernel::running->forkValue;
}


void Thread::exit(){
	lock
	
	PCB::finishThread();
	
	unlock
	
	dispatch();
}


void Thread::waitForForkChildren(){
	while(Kernel::running->numOfChildren > 0){
		Kernel::running->waitChildren->wait(0);
		Kernel::running->numOfChildren--;
	}
}

Thread* Thread::clone() const{

	return new Thread(myPCB->sizeOfStack*sizeof(unsigned), myPCB->timeSlice);
}
