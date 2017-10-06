#include "semaphor.h"
#include "kersem.h"
#include "kernel.h"
#include "semlist.h"


Semaphore::Semaphore(int init){
	myImpl = new KernelSem(init);
	lock
	Kernel::semaphore_list->add(myImpl);
	unlock
}

int Semaphore::wait(Time maxTimeToWait) { 
	return myImpl->wait(maxTimeToWait); 
}

void Semaphore::signal() {
	asm cli;	
	myImpl->signal(); 
	asm sti;
}

int Semaphore::val() const { 
	return myImpl->val; 
}

Semaphore::~Semaphore(){
	lock

	Kernel::semaphore_list->remove(myImpl);
	delete myImpl;

	unlock
}
