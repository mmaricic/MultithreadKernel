#include "idle.h"
#include "PCB.h"
#include "kernel.h"
#include <iostream.h>


void Idle::start(){
	if(myPCB->state == NEW){
		myPCB->initStack();
		myPCB->state = FINISHED;
	}
}

void Idle::run(){
	while(1) {
	}
}
PCB* Idle::getPCB(){
	return myPCB;
}
