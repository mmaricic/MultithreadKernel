#include "event.h"
#include "kernel.h"
#include "kernelev.h"
#include "evList.h"

Event::Event (IVTNo ivtNo){
	myImpl = new KernelEv(ivtNo);
	lock
	Kernel::events_list->add(myImpl);
	unlock
}
	
	void Event::wait (){
		myImpl->wait();
	}

	void Event::signal(){
		myImpl->signal();
	}

Event::~Event (){
	lock
	Kernel::events_list->remove(myImpl);
	delete myImpl;
	unlock
}