#ifndef _semaphorelist_h_
#define _semaphorelist_h_

class KernelSem;
class PCB;

class SemaphoreList{
private:
	struct Elem{
		
		KernelSem* elem;
		Elem* next;
		Elem(KernelSem* kersem){
			elem = kersem; 
			next = 0;
		}
	};
	
	Elem* first, *last;

	public:
	SemaphoreList();
	void add(KernelSem *sem);
	void remove(KernelSem *sem);
	~SemaphoreList();
};

#endif