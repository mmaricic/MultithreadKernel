#ifndef _kersem_h_
#define _kersem_h_

class Semaphore;
class Queue;

class KernelSem{
public:	
	KernelSem(int initval);
	int wait(int maxTimeToWait);
	void signal();
	~KernelSem();

private:
	friend class Semaphore;
	friend class SleepingList;
	
	int val;
	Queue *waiting_list;
	
	void block(int waitingTime);
	void deblock();
	
};

#endif