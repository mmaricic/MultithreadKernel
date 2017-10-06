#ifndef _queue_h_
#define _queue_h_

class PCB;
class PCBlist;

class Queue{
private:
	struct Elem{
		PCB* thread;
		int sleeping;
		Elem* next;
		
		Elem(PCB* pcb, int sleepingTime){
			thread = pcb;
			sleeping = sleepingTime;
			next = 0;
		}
	};
	
	Elem *first, *last;

	public:
	Queue();
	PCB* get();
	void remove(PCB* pcb);
	void put(PCB* pcb, int sleepingTime);
	~Queue();
};

#endif