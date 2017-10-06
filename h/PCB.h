#ifndef _pcb_h_
#define _pcb_h_

#include <iostream.h>
#include <dos.h>
#include "Thread.h"

class PCBlist;
class Semaphore;

enum States{NEW, READY, BLOCKED, FINISHED};

class PCB{
	static int stID;
	int threadID;

	friend class Thread;
	friend class Kernel;
	friend class Idle;
	friend class KernelSem;
	friend class SleepingList;
	friend class KernelEv;

	int returnType;
	unsigned *stack;
	unsigned ss;
	unsigned sp;
	unsigned bp;

	Time timeSlice;
	PCBlist *waitToCompl;
	States state;
	Thread* myThread;
	StackSize sizeOfStack;
	
	int isForked;
	Semaphore *waitChildren;
	int forkValue;
	int numOfChildren;
	PCB* myMaker;
	
public:
static void finishThread();
void initStack();
PCB(StackSize stackSize, Time tS, Thread* thr);
~PCB();
static void wrapper();
static void interrupt fork();
};

#endif