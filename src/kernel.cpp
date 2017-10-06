#include <iostream.h>
#include <dos.h>
#include "kernel.h"
#include "SemList.h"
#include "schedule.h"
#include "PCB.h"
#include "idle.h"
#include "thread.h"
#include "SlpgList.h"
#include "evList.h"
#include "ivtentry.h"


volatile unsigned int Kernel::lockFlag = 0;
volatile unsigned int Kernel::changeNeeded = 0;

volatile unsigned int Kernel::counter = 0;
volatile unsigned int Kernel::explicitContextChange = 0;

volatile PCBlist* Kernel::PCB_head = 0;
volatile PCBlist* Kernel::PCB_tail = 0;

pInterrupt Kernel::oldTimer = 0;
volatile PCB* Kernel::running = 0;
Idle* Kernel::idle = 0;
PCB* Kernel::oldRunning = 0;

SemaphoreList* Kernel::semaphore_list = 0;
SleepingList* Kernel::sleeping_list = 0;
EventList* Kernel::events_list;
IVTentry* Kernel::IVT[256] = {0};

unsigned tsp;
unsigned tss;
unsigned tbp;


void interrupt Kernel::timer(...){

	if (!explicitContextChange)
		sleeping_list->reduceSleeping();

	if(explicitContextChange == 0 && running->timeSlice > 0 && counter < running->timeSlice)
		counter++;
	
	if((running->timeSlice > 0 && counter == running->timeSlice) || explicitContextChange) {
		
		if (lockFlag == 0) {
			
			oldRunning = (PCB*)running;

			asm{
				mov tbp, bp
				mov tsp, sp
				mov tss, ss
			}
			running->sp = tsp;
			running->ss = tss;
			running->bp = tbp;

			
			if (running->state != FINISHED && running->state != BLOCKED)
				Scheduler::put((PCB *)running);
			
			running = Scheduler::get();
			if (running == 0) running = idle->getPCB();
			
			
			tsp = running->sp;
			tss = running->ss;
			tbp = running->bp;
			counter = 0;
			asm{
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
			if(oldRunning->state == FINISHED && oldRunning->isForked == 1)
							delete oldRunning->myThread;
		}
		
		else changeNeeded = 1;
	}
	
	if(!explicitContextChange) 
		asm int 60h;
	
	explicitContextChange = 0;	                                             
}


void Kernel::inic(){
	lock
	
	semaphore_list = new SemaphoreList();
	sleeping_list = new SleepingList();
	events_list = new EventList();
	
	
	running = new PCB(0, 0, 0);
	running->state = READY;

	oldTimer = getvect(0x8);
	setvect(0x8,timer);
	setvect(0x60,oldTimer);
	idle = new Idle();
	idle->start();
	
	unlock
}


void Kernel::restore(){
	lock

	for(int i = 0; i < 256; i++){
		if(IVT[i] != 0){
			delete IVT[i];
			IVT[i] = 0;
		}
	}

	delete semaphore_list;
	delete sleeping_list;
	delete events_list;

	setvect(0x8, oldTimer);

	delete idle;
	delete running;
	unlock
}
