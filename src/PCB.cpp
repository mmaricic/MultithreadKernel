#include "thread.h"
#include "PCBlist.h"
#include "kernel.h"
#include "schedule.h"
#include "PCB.h"
#include <dos.h>
#include "semaphore.h"

int PCB::stID = 0;

PCB::PCB(StackSize stackSize, Time tS, Thread* thr){
	lock
	threadID = ++stID;
	myThread = thr;
	unlock
	waitToCompl = 0;
	returnType = 1;
	
	sizeOfStack = stackSize / sizeof(unsigned);
	stack = 0;
	ss = 0;
	sp = 0;
	bp = sp; 
	
	state = NEW;
	timeSlice = tS;
	
	waitChildren = new Semaphore(0);
	myMaker = 0;
	numOfChildren = 0;
	forkValue = 0;
	isForked = 0;
}

void PCB::initStack(){
	stack = new unsigned[sizeOfStack];
	stack[sizeOfStack - 1] = 0x200;
	stack[sizeOfStack - 2] = FP_SEG(PCB::wrapper);
	stack[sizeOfStack - 3] = FP_OFF(PCB::wrapper);
	ss = FP_SEG(stack + sizeOfStack - 12);
	sp = FP_OFF(stack + sizeOfStack - 12);
	bp = sp;
}


PCB::~PCB(){
	while(waitToCompl != 0){
		PCBlist* temp = waitToCompl;
		waitToCompl = waitToCompl->next;
		delete temp;
	}

	if (stack != 0)
		delete [] stack;
}

void PCB::wrapper(){
	Kernel::running->myThread->run();
	
	Thread::exit();
	
	
}

void PCB::finishThread(){
	while(Kernel::running->waitToCompl != 0) {
		Kernel::running->waitToCompl->elem->state = READY;
		asm cli;
		Scheduler::put(Kernel::running->waitToCompl->elem);
		asm sti;
		PCBlist* temp = Kernel::running->waitToCompl;
		Kernel::running->waitToCompl = Kernel::running->waitToCompl->next;
		delete temp;
	}
	
	if(Kernel::running->myMaker != 0) 
		Kernel::running->myMaker->waitChildren->signal();
	
	delete Kernel::running->waitChildren;
	Kernel::running->state = FINISHED;
}


void interrupt PCB::fork(){
	
	Thread* thr = Kernel::running->myThread->clone();
	if(thr == 0 ) {
		Kernel::running->forkValue = -1;
		return;
	}

	PCB* forked = thr->myPCB;

	forked->isForked = 1;
	
	forked->stack = new unsigned[forked->sizeOfStack];
	static int i;
	for(i = 0; i < forked->sizeOfStack; i++)
		forked->stack[i] = Kernel::running->stack[i];
	
	
	forked->state = READY;
	Kernel::running->forkValue = forked->threadID;
	Kernel::running->numOfChildren++;
	
	forked->myMaker = (PCB*)Kernel::running;
	
	static unsigned parentOffset = FP_OFF(Kernel::running->stack);
	static unsigned forkedOffset = FP_OFF(forked->stack);
	forked->ss = FP_SEG(forked->stack);
	
	static unsigned spacing;
	asm {
		mov spacing, bp
	}
	
	spacing = spacing - parentOffset;
	forked->sp = forkedOffset + spacing;
	forked->bp = forkedOffset + spacing;
	
	static unsigned* childBP, *parentBP = 0;
	static unsigned* firstBP = (unsigned*)MK_FP(Kernel::running->ss, Kernel::running->stack+Kernel::running->sizeOfStack-12);
	
	while(parentBP < firstBP){
	parentBP = (unsigned*)MK_FP(Kernel::running->ss, parentOffset + spacing);
	childBP = (unsigned*)MK_FP(forked->ss, forkedOffset + spacing);
	spacing = *parentBP - parentOffset;
	*childBP = forkedOffset + spacing;
	}
	
	Scheduler::put(forked);
}
