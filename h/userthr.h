#ifndef _userthread_h_
#define userthread_h_

#include "thread.h"

class UserThread : public Thread {
	int argc;
	char** argv;

public:
	UserThread(int argc, char** argv);
	void run();
	Thread* clone() const;
	~UserThread();
};

#endif
