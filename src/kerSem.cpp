#include "semaphor.h"
#include "Queue.h"
#include "kersem.h"
#include "PCB.h"
#include "kernel.h"
#include "schedule.h"
#include "slpgList.h"
#include "thread.h"

KernelSem::KernelSem(int initval){
	val = initval;
	waiting_list = new Queue();
}

int KernelSem::wait(int maxTimeToWait){
	lock
		
		if (--val < 0){
			unlock
			block(maxTimeToWait);
		}
		else
			unlock
		
	return Kernel::running->returnType;
}
	
void KernelSem::signal(){
		if (val++ < 0)
			deblock();
		
}

void KernelSem::block(int waitingTime){
	lock
	
	waiting_list->put((PCB*)Kernel::running, waitingTime);
	if (waitingTime > 0) 
		Kernel::sleeping_list->add((PCB*)Kernel::running, this, waitingTime);
	Kernel::running->state = BLOCKED;
	
	unlock
	
	dispatch();
}

void KernelSem::deblock(){
	PCB* thread = waiting_list->get();
	thread->returnType = 1;
	thread->state = READY;
	Scheduler::put(thread);
}

KernelSem::~KernelSem(){
	delete waiting_list;
}