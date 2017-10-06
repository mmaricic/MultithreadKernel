#include "event.h"
#include "kernelev.h"
#include "ivtentry.h"
#include "kernel.h"
#include "schedule.h"
#include "PCB.h"
#include "thread.h"



KernelEv::KernelEv(unsigned char IVTno){
	this->IVTno = IVTno;
	maker = (PCB*)Kernel::running;
	blocked = 0;
	value = 0;
	Kernel::IVT[IVTno]->setEntry(this);
}

void KernelEv::wait(){
	if(Kernel::running == maker){
		if(value == 0){
			Kernel::running->state = BLOCKED;
			blocked = (PCB*)Kernel::running;
			dispatch();
		}
		else 
			value = 0;
	}
}

void KernelEv::signal(){
	if(blocked == 0)
		value = 1;
	else{
		blocked->state = READY;
		Scheduler::put(blocked);
		blocked = 0;
		
		if (Kernel::lockFlag == 1)
			Kernel::changeNeeded = 1;
		else
			dispatch();
	}
}

KernelEv::~KernelEv(){
	maker = blocked = 0;
	if (Kernel::IVT[IVTno] != 0){
		delete Kernel::IVT[IVTno];
		Kernel::IVT[IVTno] = 0;
	}
}
