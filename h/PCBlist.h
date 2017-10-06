#ifndef _pcblist_h_
#define _pcblist_h_

class PCB;

class PCBlist{
public:
	PCB* elem;
	PCBlist* next;
	PCBlist(PCB* pcb);
};

#endif