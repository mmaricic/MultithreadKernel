#ifndef _ivtentry_h_
#define _ivtentry_h_
#include "kernel.h"

#define PREPAREENTRY(numEntry, oldFlag)\
void interrupt inter##numEntry(...); \
IVTentry *newEntry##numEntry = new IVTentry(numEntry, inter##numEntry); \
void interrupt inter##numEntry(...) {\
newEntry##numEntry->signal();\
if (oldFlag == 1)\
newEntry##numEntry->callOld();\
}



class KernelEv;

class IVTentry{
public:
	IVTentry(unsigned char entryNum, pInterrupt newRoutine);
	void signal();
	void callOld();
	void setEntry(KernelEv *event);
	~IVTentry();
	
private:
	KernelEv *myEvent;
	pInterrupt oldRoutine;
	unsigned char entryNum;
};

#endif