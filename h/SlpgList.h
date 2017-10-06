#ifndef _sleepinglist_h_
#define _sleepinglist_h_

class KernelSem;
class PCB;

class SleepingList{
private:
	struct Element{
		PCB* elem;
		Element* next;
		KernelSem* mySem;
		int sleepingTime;
		
		Element(PCB* pcb, int time, KernelSem* kersem){
			elem = pcb;
			next = 0;
			mySem = kersem;
			sleepingTime = time;
		}
	};

	Element *first, *last;
	
public:
	SleepingList();
	void add(PCB* pcb, KernelSem* semaphore, int time);
	void remove(PCB* pcb);
	void reduceSleeping();
	~SleepingList();
};

#endif