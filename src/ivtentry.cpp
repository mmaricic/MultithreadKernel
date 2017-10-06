#include "ivtentry.h"
#include "kernelev.h"
#include <dos.h>

IVTentry::IVTentry(unsigned char entryNum, pInterrupt newRoutine){
	this->entryNum = entryNum;
	oldRoutine = getvect(entryNum);
	setvect(entryNum, newRoutine);
	Kernel::IVT[entryNum] = this;
}

IVTentry::~IVTentry(){
	setvect(entryNum, oldRoutine);
}


void IVTentry::signal(){
	myEvent->signal();
}


void IVTentry::callOld(){
	oldRoutine();
}
	
void IVTentry::setEntry(KernelEv *event){
	myEvent = event;
}