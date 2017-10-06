#ifndef _kernel_h_
#define _kernel_h_
#include <iostream.h>
#include <dos.h>
#include "thread.h"


#define lock Kernel::lockFlag = 1; 

#define unlock Kernel::lockFlag = 0; if (Kernel::changeNeeded) {Kernel::changeNeeded = 0; dispatch();} 


void tick();

class PCBlist;
class PCB;
class Idle;
class SemaphoreList;
class SleepingList;
class IVTentry;
class EventList;

typedef void interrupt (*pInterrupt)(...);

class Kernel{

public:
static pInterrupt oldTimer;
static volatile unsigned int changeNeeded;
static volatile unsigned int lockFlag;
static volatile unsigned int counter;
static volatile unsigned int explicitContextChange;

static volatile PCBlist* PCB_head;
static volatile PCBlist* PCB_tail;
static PCB* oldRunning;
static SemaphoreList* semaphore_list;
static SleepingList* sleeping_list;
static EventList* events_list;
static volatile PCB* running;
static Idle *idle;

static IVTentry* IVT[256];

static void interrupt timer(...);

static void inic();

static void restore();

};
#endif
