#ifndef _idle_h_
#define _idle_h_

#include "thread.h"
class PCB;

class Idle: public Thread{
public:
Idle(StackSize SS=defaultStackSize, Time timeSlice=1):Thread(SS, timeSlice){}
void run();
void start();
PCB* getPCB();

};

#endif