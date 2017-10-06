#ifndef _kernelev_h_
#define _kernelev_h_

class Event;
class PCB;

class KernelEv{
public:	
	KernelEv(unsigned char IVTno);
	void wait();
	void signal();
	~KernelEv();

private:
	friend class Event;
	
	int value;
	PCB *blocked;
	PCB *maker;
	unsigned char IVTno;
	
};

#endif